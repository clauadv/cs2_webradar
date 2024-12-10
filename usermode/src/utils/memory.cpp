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

std::optional<void*> c_memory::hijack_handle()
{
	auto cleanup = [](std::vector<uint8_t>& handle_info, void*& process_handle)
	{
		handle_info.clear();

		if (process_handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(process_handle);
			process_handle = nullptr;
		}
	};

	const auto ntdll = GetModuleHandleA("ntdll.dll");
	if (!ntdll)
		return {};

	using fn_nt_query_system_information = long(__stdcall*)(unsigned long, void*, unsigned long, unsigned long*);
	const auto nt_query_system_information = reinterpret_cast<fn_nt_query_system_information>(GetProcAddress(ntdll, "NtQuerySystemInformation"));
	if (!nt_query_system_information)
		return {};

	using fn_nt_duplicate_object = long(__stdcall*)(void*, void*, void*, void**, unsigned long, unsigned long, unsigned long);
	const auto nt_duplicate_object = reinterpret_cast<fn_nt_duplicate_object>(GetProcAddress(ntdll, "NtDuplicateObject"));
	if (!nt_duplicate_object)
		return {};

	using fn_nt_open_process = long(__stdcall*)(void**, unsigned long, OBJECT_ATTRIBUTES*, CLIENT_ID*);
	const auto nt_open_process = reinterpret_cast<fn_nt_open_process>(GetProcAddress(ntdll, "NtOpenProcess"));
	if (!nt_open_process)
		return {};

	using fn_rtl_adjust_privilege = long(__stdcall*)(unsigned long, unsigned char, unsigned char, unsigned char*);
	const auto rtl_adjust_privilege = reinterpret_cast<fn_rtl_adjust_privilege>(GetProcAddress(ntdll, "RtlAdjustPrivilege"));
	if (!rtl_adjust_privilege)
		return {};

	uint8_t old_privilege = 0;
	rtl_adjust_privilege(0x14, 1, 0, &old_privilege);

	OBJECT_ATTRIBUTES object_attributes{};
	InitializeObjectAttributes(&object_attributes, nullptr, 0, nullptr, nullptr);

	std::vector<uint8_t> handle_info(sizeof(system_handle_info_t));
	std::pair<void*, void*> handle{ nullptr, nullptr };
	CLIENT_ID client_id{};

	unsigned long status = 0;
	do
	{
		handle_info.resize(handle_info.size() * 2);
		status = nt_query_system_information(0x10, handle_info.data(), static_cast<unsigned long>(handle_info.size()), nullptr);
	} while (status == 0xc0000004);

	if (!NT_SUCCESS(status))
	{
		cleanup(handle_info, handle.first);
		return {};
	}

	const auto system_handle_info = reinterpret_cast<system_handle_info_t*>(handle_info.data());
	for (uint32_t idx = 0; idx < system_handle_info->m_handle_count; ++idx)
	{
		const auto system_handle = system_handle_info->m_handles[idx];
		if (reinterpret_cast<void*>(system_handle.m_handle) == INVALID_HANDLE_VALUE || system_handle.m_object_type_number != 0x07)
			continue;

		client_id.UniqueProcess = reinterpret_cast<void*>(system_handle.m_process_id);

		if (handle.first != nullptr && handle.first == INVALID_HANDLE_VALUE)
		{
			CloseHandle(handle.first);
			continue;
		}

		const auto open_process = nt_open_process(&handle.first, PROCESS_DUP_HANDLE, &object_attributes, &client_id);
		if (!NT_SUCCESS(open_process))
			continue;

		const auto duplicate_object = nt_duplicate_object(handle.first, reinterpret_cast<void*>(system_handle.m_handle), GetCurrentProcess(), &handle.second, PROCESS_ALL_ACCESS, 0, 0);
		if (!NT_SUCCESS(duplicate_object))
			continue;

		if (GetProcessId(handle.second) == this->m_id)
		{
			cleanup(handle_info, handle.first);
			return handle.second;
		}

		CloseHandle(handle.second);
	}

	cleanup(handle_info, handle.first);
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

	const auto module_data = std::make_unique<uint8_t[]>(module_size.value());
	if (!this->read_t(module_base.value(), module_data.get(), module_size.value()))
		return {};

	const auto pattern_bytes = pattern_to_bytes(pattern);
	for (uint32_t idx = 0; idx < module_size.value() - pattern.size(); ++idx)
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
			return std::make_pair(reinterpret_cast<uintptr_t>(module_entry.modBaseAddr), static_cast<uintptr_t>(module_entry.modBaseSize));
	}

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
		const auto process_id = m_memory->get_process_id(process_name);
		if (!process_id.has_value())
			continue;

		LOG_INFO("possible anti-cheat process detected ('%s')", process_name.data());
		return true;
	}

	return false;
}