#pragma once

namespace usermode
{
	class c_memory
	{
	private:
		bool m_initialized{ false };
		HANDLE m_handle{ nullptr };
		std::int64_t m_process_id{ 0 };

	public:
		~c_memory()
		{
			if (this->m_handle != nullptr)
			{
				CloseHandle(this->m_handle);
			}
		}

		bool is_initialized() { return this->m_initialized; }
		std::int64_t get_process_id() { return this->m_process_id; }

		bool set_process(const std::string& name)
		{
			const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (snapshot == INVALID_HANDLE_VALUE)
				return false;

			PROCESSENTRY32 entry = { 0 };
			entry.dwSize = sizeof(entry);

			while (Process32Next(snapshot, &entry) == true)
			{
				if (_stricmp(entry.szExeFile, name.data()) == 0)
				{
					this->m_process_id = entry.th32ProcessID;
					break;
				}
			}

			CloseHandle(snapshot);

			if (this->m_process_id != 0)
			{
				this->m_handle = OpenProcess(PROCESS_ALL_ACCESS, false, this->m_process_id);
				if (!this->m_handle)
					return false;

				this->m_initialized = true;
			}

			return this->m_initialized;
		}

		std::uintptr_t get_base(const std::string& name)
		{
			std::uintptr_t base{ 0 };

			if (!this->m_process_id)
				return 0;

			const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->m_process_id);
			if (snapshot == INVALID_HANDLE_VALUE)
				return 0;

			MODULEENTRY32 entry = { 0 };
			entry.dwSize = sizeof(entry);

			while (Module32Next(snapshot, &entry) == true)
			{
				if (_stricmp(entry.szModule, name.data()) == 0)
				{
					base = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
					break;
				}
			}

			CloseHandle(snapshot);
			return base;
		}

		void read(void* address, void* buffer, std::size_t size)
		{
			ReadProcessMemory(m_handle, address, buffer, size, nullptr);
		}

		template <typename t>
		t read_t(void* address)
		{
			t value{ 0 };
			this->read(address, &value, sizeof(t));

			return value;
		}

		template <typename t>
		t read_t(const std::uintptr_t& address)
		{
			t value{ 0 };
			this->read(reinterpret_cast<void*>(address), &value, sizeof(t));

			return value;
		}

		template <>
		std::string read_t<std::string>(const std::uintptr_t& address)
		{
			static const int length = 64;
			std::vector<char> buffer(length);

			this->read(reinterpret_cast<void*>(address), buffer.data(), length);

			const auto& it = std::find(buffer.begin(), buffer.end(), '\0');

			if (it != buffer.end())
				buffer.resize(std::distance(buffer.begin(), it));

			return std::string(buffer.begin(), buffer.end());
		}
	};
}
inline usermode::c_memory m_memory{};