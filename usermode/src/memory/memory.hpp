#pragma once

namespace src
{
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

		HANDLE get_handle()
		{
			return this->m_handle;
		}

		optional<uint32_t> get_process_id(const string_view& process_name)
		{
			const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (snapshot == INVALID_HANDLE_VALUE)
				return {};

			PROCESSENTRY32 process_entry = { 0 };
			process_entry.dwSize = sizeof(process_entry);

			for (Process32First(snapshot, &process_entry); Process32Next(snapshot, &process_entry);)
			{
				if (string_view(process_entry.szExeFile) == process_name)
					return process_entry.th32ProcessID;
			}

			CloseHandle(snapshot);
			return {};
		}

		bool attach(const string_view& process_name)
		{
			const auto process_id = get_process_id(process_name);
			if (!process_id.has_value())
				return false;

			this->m_id = process_id.value();
			this->m_handle = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, false, this->m_id);

			return this->m_handle != nullptr;
		}

		optional<c_address> find_pattern(const string_view& module_name, const string_view& pattern)
		{
			constexpr auto pattern_to_bytes = [](const string_view& pattern)
			{
				vector<int32_t> bytes;

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

								if (const auto [ptr, ec] = from_chars(pattern.data() + i, pattern.data() + i + 2, value, 16); ec == errc())
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

			const auto module_data = make_unique<uint8_t[]>(module_size.value());
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

		pair<optional<uintptr_t>, optional<uintptr_t>> get_module_info(const string_view& module_name)
		{
			const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->m_id);
			if (snapshot == INVALID_HANDLE_VALUE)
				return {};

			MODULEENTRY32 module_entry = { 0 };
			module_entry.dwSize = sizeof(module_entry);

			for (Module32First(snapshot, &module_entry); Module32Next(snapshot, &module_entry);)
			{
				auto equals_ignore_case = [](const string_view str_1, const string_view str_2)
				{
					return (str_1.size() == str_2.size()) && equal(str_1.begin(), str_1.end(), str_2.begin(), [](const char a, const char b)
					{
						return tolower(a) == tolower(b);
					});
				};

				if (equals_ignore_case(module_entry.szModule, module_name))
					return make_pair(reinterpret_cast<uintptr_t>(module_entry.modBaseAddr), static_cast<uintptr_t>(module_entry.modBaseSize));
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
		string read_t<string>(const uintptr_t address)
		{
			static const int length = 64;
			vector<char> buffer(length);

			this->read_memory(reinterpret_cast<void*>(address), buffer.data(), length);

			const auto& it = find(buffer.begin(), buffer.end(), '\0');

			if (it != buffer.end())
				buffer.resize(distance(buffer.begin(), it));

			return string(buffer.begin(), buffer.end());
		}

	private:
		bool m_initialized{ false };
		HANDLE m_handle{ nullptr };
		int64_t m_id{ 0 };

		bool read_memory(void* address, void* buffer, const size_t size)
		{
			return ReadProcessMemory(this->m_handle, reinterpret_cast<void*>(address), buffer, size, nullptr);
		}
	};
}
inline src::c_memory m_memory{};