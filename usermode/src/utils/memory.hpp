#pragma once

#define NT_SUCCESS(status) (static_cast<long>(status) >= 0)

struct __system_handle_t
{
	unsigned long m_process_id;
	uint8_t m_object_type_number;
	uint8_t m_flags;
	uint16_t m_handle;
	void* m_object;
	ACCESS_MASK m_granted_access;
};

struct system_handle_info_t
{
	unsigned long m_handle_count;
	__system_handle_t m_handles[1];
};

class c_memory
{
public:
	~c_memory()
	{
		if (this->m_handle != nullptr)
			CloseHandle(this->m_handle);
	}

	uint32_t get_id()
	{
		return this->m_id;
	}

	std::optional<uint32_t> get_process_id(const std::string_view& process_name)
	{
		const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snapshot == INVALID_HANDLE_VALUE)
			return {};

		PROCESSENTRY32 process_entry = { 0 };
		process_entry.dwSize = sizeof(process_entry);

		for (Process32First(snapshot, &process_entry); Process32Next(snapshot, &process_entry);)
		{
			if (std::string_view(process_entry.szExeFile) == process_name)
				return process_entry.th32ProcessID;
		}

		CloseHandle(snapshot);
		return {};
	}

	std::optional<void*> hijack_handle()
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

		const auto nt_query_system_information = reinterpret_cast<long(__stdcall*)(unsigned long, void*, unsigned long, unsigned long*)>(GetProcAddress(ntdll, "NtQuerySystemInformation"));
		if (!nt_query_system_information)
			return {};

		const auto nt_duplicate_object = reinterpret_cast<long(__stdcall*)(void*, void*, void*, void**, unsigned long, unsigned long, unsigned long)>(GetProcAddress(ntdll, "NtDuplicateObject"));
		if (!nt_duplicate_object)
			return {};

		const auto nt_open_process = reinterpret_cast<long(__stdcall*)(void**, unsigned long, OBJECT_ATTRIBUTES*, CLIENT_ID*)>(GetProcAddress(ntdll, "NtOpenProcess"));
		if (!nt_open_process)
			return {};

		const auto rtl_adjust_privilege = reinterpret_cast<long(__stdcall*)(unsigned long, unsigned char, unsigned char, unsigned char*)>(GetProcAddress(ntdll, "RtlAdjustPrivilege"));
		if (!rtl_adjust_privilege)
			return {};

		uint8_t old_privilege{ 0 };
		rtl_adjust_privilege(0x14, 1, 0, &old_privilege);

		OBJECT_ATTRIBUTES object_attributes{};
		InitializeObjectAttributes(&object_attributes, nullptr, 0, nullptr, nullptr);

		std::vector<uint8_t> handle_info(sizeof(system_handle_info_t));
		std::pair<void*, void*> handle{ nullptr, nullptr };
		CLIENT_ID client_id{};

		unsigned long status{ 0 };
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
		for (auto i{ 0 }; i < system_handle_info->m_handle_count; ++i)
		{
			const auto system_handle = system_handle_info->m_handles[i];
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

	bool attach()
	{
		const auto process_id = this->get_process_id(GAME_NAME);
		if (!process_id.has_value())
		{
			LOG_ERROR("failed to get process id for %s", GAME_NAME);
			return false;
		}

		this->m_id = process_id.value();

		const auto handle = this->hijack_handle();
		if (!handle.has_value())
		{
			LOG_ERROR("failed to hijack a handle for %s", GAME_NAME);
			return false;
		}

		this->m_handle = handle.value();

		return this->m_handle != nullptr;
	}

	std::optional<c_address> find_pattern(const std::string_view& module_name, const std::string_view& pattern)
	{
		constexpr auto pattern_to_bytes = [](const std::string_view& pattern)
		{
			std::vector<int32_t> bytes;

			for (auto i{ 0 }; i < pattern.size(); ++i)
			{
				switch (pattern[i])
				{
					case '?':
						bytes.push_back(-1);
						break;

					case ' ':
						break;

					default:
					{
						if (i + 1 < pattern.size())
						{
							auto value{ 0 };

							if (const auto [ptr, ec] = std::from_chars(pattern.data() + i, pattern.data() + i + 2, value, 16); ec == std::errc())
							{
								bytes.push_back(value);
								++i;
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
		for (auto i{ 0 }; i < module_size.value() - pattern.size(); ++i)
		{
			auto found{ true };

			for (auto j{ 0 }; j < pattern_bytes.size(); ++j)
			{
				if (module_data[i + j] != pattern_bytes[j] && pattern_bytes[j] != -1)
				{
					found = false;
					break;
				}
			}

			if (found)
				return c_address(module_base.value() + i);
		}

		return {};
	}

	std::pair<std::optional<uintptr_t>, std::optional<uintptr_t>> get_module_info(const std::string_view& module_name)
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

	bool read_t(const uintptr_t address, void* buffer, uintptr_t size)
	{
		this->read_memory(reinterpret_cast<void*>(address), buffer, size);
		return true;
	}

	template <typename t>
	t read_t(void* address)
	{
		t value{ 0 };
		this->read_memory(address, &value, sizeof(t));

		return value;
	}

	template<typename T>
	T read_t(const uintptr_t address) noexcept
	{
		T buffer{};
		this->read_memory(reinterpret_cast<void*>(address), &buffer, sizeof(T));
		return buffer;
	}

	template <>
	std::string read_t<std::string>(const uintptr_t address)
	{
		static const int length = 64;
		std::vector<char> buffer(length);

		this->read_memory(reinterpret_cast<void*>(address), buffer.data(), length);

		const auto& it = find(buffer.begin(), buffer.end(), '\0');

		if (it != buffer.end())
			buffer.resize(distance(buffer.begin(), it));

		return std::string(buffer.begin(), buffer.end());
	}

private:
	bool m_initialized{ false };
	void* m_handle{ nullptr };
	uint32_t m_id{ 0 };

	bool read_memory(void* address, void* buffer, const size_t size)
	{
		return ReadProcessMemory(this->m_handle, reinterpret_cast<void*>(address), buffer, size, nullptr);
	}
};
inline const std::unique_ptr<c_memory> m_memory{ new c_memory() };

namespace mem
{
	bool setup();
}