#pragma once

namespace utils
{
	inline string get_ipv4_address()
	{
	#if USE_LOCALHOST
		return "localhost";
	#endif

		string ip_address;
		array<char, 128> buffer{};

		const auto pipe = _popen("ipconfig", "r");
		if (!pipe)
		{
			LOG_ERROR("failed to open a pipe to ipconfig");
			return string{};
		}

		while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
		{
			string line(buffer.data());
			smatch match{};
			if (regex_search(line, match, regex(R"((192\.168\.\d+\.\d+))")) && match.size() == 2)
			{
				ip_address = match[1];
				break;
			}
		}
		_pclose(pipe);

		return ip_address;
	}
}

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