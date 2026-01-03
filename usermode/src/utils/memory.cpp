#include "pch.hpp"

bool c_memory::setup()
{
	if (is_anticheat_running())
		return {};

	const auto process_id = this->get_process_id("cs2.exe");
	if (!process_id.has_value())
	{
		LOG_ERROR("failed to get process id for 'cs2.exe'\n			  make sure the game is running");
		return {};
	}

	this->m_id = process_id.value();

	auto handle = this->hijack_handle();
	if (!handle.has_value())
	{
		LOG_WARNING("failed to hijack a handle for 'cs2.exe', we will continue using the classic method");
		this->m_handle = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, false, this->m_id);
	}
	else
		this->m_handle = handle.value();

	return this->m_handle != nullptr;
}

std::optional<uint32_t> c_memory::get_process_id(const std::string_view& process_name)
{
	const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE)
		return {};

	PROCESSENTRY32 process_entry = { 0 };
	process_entry.dwSize = sizeof(process_entry);

	for (Process32First(snapshot, &process_entry); Process32Next(snapshot, &process_entry);)
	{
		if (std::string_view(process_entry.szExeFile) == process_name)
		{
			CloseHandle(snapshot);
			return process_entry.th32ProcessID;
		}
	}

	CloseHandle(snapshot);
	return {};
}

/// <summary>
/// Attempts to obtain a process handle by duplicating an existing handle from another process.
/// </summary>
/// <returns>
/// An optional containing the duplicated process handle if successful, std::nullopt otherwise.
/// The returned handle has PROCESS_VM_READ and PROCESS_QUERY_INFORMATION access rights.
/// </returns>
/// <remarks>
/// Requires SeDebugPrivilege to be enabled for the current process.
/// Falls back to standard OpenProcess if this method fails.
/// Properly cleans up any opened handles and restores privileges before returning.
/// </remarks>
std::optional<void*> c_memory::hijack_handle()
{
	// validate that we have a target PID
	if (this->m_id == 0)
		return {};

	// helper to close and clean up process handles
	auto cleanup_process_handle = [](void*& process_handle)
	{
		if (process_handle != nullptr && process_handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(process_handle);
			process_handle = nullptr;
		}
	};

	// Get ntdll.dll module handle to access native API functions
	const auto ntdll = GetModuleHandleA("ntdll.dll");
	if (!ntdll)
		return {};

	// Resolve NtQuerySystemInformation function to enumerate system handles
	using fn_nt_query_system_information = long(__stdcall*)(unsigned long, void*, unsigned long, unsigned long*);
	const auto nt_query_system_information = reinterpret_cast<fn_nt_query_system_information>(GetProcAddress(ntdll, "NtQuerySystemInformation"));
	if (!nt_query_system_information)
		return {};

	// Resolve NtDuplicateObject function to duplicate handles from other processes
	using fn_nt_duplicate_object = long(__stdcall*)(void*, void*, void*, void**, unsigned long, unsigned long, unsigned long);
	const auto nt_duplicate_object = reinterpret_cast<fn_nt_duplicate_object>(GetProcAddress(ntdll, "NtDuplicateObject"));
	if (!nt_duplicate_object)
		return {};

	// Resolve NtOpenProcess function to open handles to other processes with handles we want to duplicate
	using fn_nt_open_process = long(__stdcall*)(void**, unsigned long, OBJECT_ATTRIBUTES*, CLIENT_ID*);
	const auto nt_open_process = reinterpret_cast<fn_nt_open_process>(GetProcAddress(ntdll, "NtOpenProcess"));
	if (!nt_open_process)
		return {};

	// Resolve RtlAdjustPrivilege function to enable SeDebugPrivilege
	using fn_rtl_adjust_privilege = long(__stdcall*)(unsigned long, unsigned char, unsigned char, unsigned char*);
	const auto rtl_adjust_privilege = reinterpret_cast<fn_rtl_adjust_privilege>(GetProcAddress(ntdll, "RtlAdjustPrivilege"));
	if (!rtl_adjust_privilege)
		return {};

	// Windows privilege and status code constants
	constexpr unsigned long SE_DEBUG_PRIVILEGE = 0x14;
	constexpr unsigned long SYSTEM_HANDLE_INFORMATION = 0x10;
	constexpr unsigned long STATUS_INFO_LENGTH_MISMATCH = 0xc0000004;

	// Attempt to enable SeDebugPrivilege and store the old privilege state
	// This is so that we can restore it later to avoid leaving the process in an elevated state
	uint8_t old_privilege = 0;
	const auto privilege_status = rtl_adjust_privilege(SE_DEBUG_PRIVILEGE, 1, 0, &old_privilege);
	const bool privilege_enabled = NT_SUCCESS(privilege_status);

	// Initialize OBJECT_ATTRIBUTES structure for NtOpenProcess calls
	OBJECT_ATTRIBUTES object_attributes{};
	InitializeObjectAttributes(&object_attributes, nullptr, 0, nullptr, nullptr);

	// Allocate initial buffer for system handle information, which will be dynamically resized as needed
	std::vector<uint8_t> handle_info(4096);
	void* owner_process_handle = nullptr;
	CLIENT_ID client_id{};
	uint32_t current_owner_pid = 0;

	// Query system handle information, resizing buffer until successful
	// if the buffer is too small, NtQuerySystemInformation returns STATUS_INFO_LENGTH_MISMATCH (0xc0000004)
	constexpr size_t MAX_BUFFER_SIZE = 64 * 1024 * 1024;
	constexpr uint32_t MAX_RESIZE_ATTEMPTS = 20;
	unsigned long status = 0;
	uint32_t resize_attempts = 0;
	do
	{
		handle_info.resize(handle_info.size() * 2);
		// SystemHandleInformation (0x10) requests information about all handles in the system
		status = nt_query_system_information(SYSTEM_HANDLE_INFORMATION, handle_info.data(), static_cast<unsigned long>(handle_info.size()), nullptr);

		// If we've exceeded max resize attempts or buffer size, clean up and return
		if (++resize_attempts >= MAX_RESIZE_ATTEMPTS || handle_info.size() >= MAX_BUFFER_SIZE)
		{
			cleanup_process_handle(owner_process_handle);
			if (privilege_enabled)
			{
				// Restore the original privilege state before exiting
				uint8_t restore_privilege = 0;
				rtl_adjust_privilege(SE_DEBUG_PRIVILEGE, old_privilege, 0, &restore_privilege);
			}
			return {};
		}

	} while (status == STATUS_INFO_LENGTH_MISMATCH);

	// if we failed to get the handle information, clean up and return
	if (!NT_SUCCESS(status))
	{
		cleanup_process_handle(owner_process_handle);
		if (privilege_enabled)
		{
			// Restore the original privilege state before exiting
			uint8_t restore_privilege = 0;
			rtl_adjust_privilege(SE_DEBUG_PRIVILEGE, old_privilege, 0, &restore_privilege);
		}
		return {};
	}

	const auto system_handle_info = reinterpret_cast<system_handle_info_t*>(handle_info.data());

	// Iterate through all system handles looking for handles to our target process
	for (uint32_t idx = 0; idx < system_handle_info->m_handle_count; ++idx)
	{
		const auto& system_handle = system_handle_info->m_handles[idx];

		// Skip invalid handles
		if (reinterpret_cast<void*>(system_handle.m_handle) == INVALID_HANDLE_VALUE)
			continue;

		// Skip handles owned by our target process
		if (system_handle.m_process_id == this->m_id)
			continue;

		// Open a handle to the process owning the current handle if we haven't already
		if (system_handle.m_process_id != current_owner_pid)
		{
			// Clean up the previous owner process handle before opening a new one
			cleanup_process_handle(owner_process_handle);

			// Setup CLIENT_ID for the owner process
			client_id.UniqueProcess = reinterpret_cast<void*>(static_cast<uintptr_t>(system_handle.m_process_id));
			client_id.UniqueThread = nullptr;

			// Open the owner process with PROCESS_DUP_HANDLE access
			status = nt_open_process(&owner_process_handle, PROCESS_DUP_HANDLE, &object_attributes, &client_id);
			if (!NT_SUCCESS(status))
				continue;

			// cache the current owner PID to avoid reopening the same process
			current_owner_pid = system_handle.m_process_id;
		}

		// Attempt to duplicate the handle from the owner process into our own process
		void* duplicated_handle = nullptr;
		status = nt_duplicate_object(owner_process_handle, reinterpret_cast<void*>(static_cast<uintptr_t>(system_handle.m_handle)), GetCurrentProcess(), &duplicated_handle, PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, 0, 0);
		if (!NT_SUCCESS(status))
			continue;

		// Verify that the duplicated handle points to our target process by checking its PID
		const auto duplicated_pid = GetProcessId(duplicated_handle);
		if (duplicated_pid == this->m_id)
		{
			cleanup_process_handle(owner_process_handle);
			handle_info.clear();

			// Restore the original privilege state before returning
			if (privilege_enabled)
			{
				uint8_t restore_privilege = 0;
				rtl_adjust_privilege(SE_DEBUG_PRIVILEGE, old_privilege, 0, &restore_privilege);
			}

			return duplicated_handle;
		}

		// Not our target process, close the duplicated handle and continue searching
		CloseHandle(duplicated_handle);
	}
	
	// Enumerated all handles but didn't find one. Clean up and restore privileges before returning.
	cleanup_process_handle(owner_process_handle);
	handle_info.clear();

	if (privilege_enabled)
	{
		uint8_t restore_privilege = 0;
		rtl_adjust_privilege(SE_DEBUG_PRIVILEGE, old_privilege, 0, &restore_privilege);
	}

	return {};
}

std::optional<c_address> c_memory::find_pattern(const std::string_view& module_name, const std::string_view& pattern)
{
	constexpr auto pattern_to_bytes = [](const std::string_view& pattern)
	{
		std::vector<int32_t> bytes;

		for (uint32_t idx = 0; idx < pattern.size(); ++idx)
		{
			switch (pattern[idx])
			{
				case '?':
					bytes.push_back(-1);
					break;

				case ' ':
					break;

				default:
				{
					if (idx + 1 < pattern.size())
					{
						uint32_t value = 0;

						if (const auto [ptr, ec] = std::from_chars(pattern.data() + idx, pattern.data() + idx + 2, value, 16); ec == std::errc())
						{
							bytes.push_back(value);
							++idx;
						}
					}

					break;
				}
			}
		}

		return bytes;
	};

	const auto [module_base, module_size] = this->get_module_info(module_name);
	if (!module_base.has_value() || !module_size.has_value())
		return {};

	const auto pattern_bytes = pattern_to_bytes(pattern);
	if (pattern_bytes.empty() || pattern_bytes.size() > module_size.value())
		return {};

	const auto module_data = std::make_unique<uint8_t[]>(module_size.value());
	if (!this->read_t(module_base.value(), module_data.get(), module_size.value()))
		return {};

	for (uint32_t idx = 0; idx < module_size.value() - pattern_bytes.size(); ++idx)
	{
		bool found = true;

		for (uint32_t b_idx = 0; b_idx < pattern_bytes.size(); ++b_idx)
		{
			if (module_data[idx + b_idx] != pattern_bytes[b_idx] && pattern_bytes[b_idx] != -1)
			{
				found = false;
				break;
			}
		}

		if (found)
			return c_address(module_base.value() + idx);
	}

	return {};
}

std::pair<std::optional<uintptr_t>, std::optional<uintptr_t>> c_memory::get_module_info(const std::string_view& module_name)
{
	const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->m_id);
	if (snapshot == INVALID_HANDLE_VALUE)
		return {};

	MODULEENTRY32 module_entry = { 0 };
	module_entry.dwSize = sizeof(module_entry);

	for (Module32First(snapshot, &module_entry); Module32Next(snapshot, &module_entry);)
	{
		auto equals_ignore_case = [](const std::string_view str_1, const std::string_view str_2)
		{
			return (str_1.size() == str_2.size()) && equal(str_1.begin(), str_1.end(), str_2.begin(), [](const char a, const char b)
			{
				return tolower(a) == tolower(b);
			});
		};

		if (equals_ignore_case(module_entry.szModule, module_name))
		{
			CloseHandle(snapshot);
			return std::make_pair(reinterpret_cast<uintptr_t>(module_entry.modBaseAddr), static_cast<uintptr_t>(module_entry.modBaseSize));
		}
	}

	CloseHandle(snapshot);
	return {};
}

bool c_memory::is_anticheat_running()
{
	constexpr std::array<std::string_view, 7> m_process_list = {
		"faceitclient.exe",	 // faceit client
		"faceitservice.exe", // faceit service
		"faceit.exe",		 // faceit process
		"esportal.exe",		 // esportal client
		"perfectworld.exe"   // perfect world (?)
	};

	for (const auto& process_name : m_process_list)
	{
		const auto process_id = get_process_id(process_name);
		if (!process_id.has_value())
			continue;

		LOG_INFO("possible anti-cheat process detected ('%s')", process_name.data());
		return true;
	}

	return false;
}