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