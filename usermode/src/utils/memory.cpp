#include "pch.hpp"

 int c_memory::setup()
{
	 if (is_anticheat_running()) {
		 LOG_WARNING("Detected running Anti-Cheat software.");
		 return 1;
	 }

	auto process_id = this->get_process_id("cs2.exe");
	if (!process_id.has_value()) {
		LOG_WARNING("No CS2.exe process.")
		return 2;
	}

	this->m_id = process_id.value();

	this->m_handle = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, false, this->m_id);

	return this->m_handle != nullptr?0:3;
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