#pragma once

namespace utils
{
	inline std::string get_ipv4_address()
	{
	#if USE_LOCALHOST
		return "localhost";
	#endif

		std::string ip_address;
		std::array<char, 128> buffer{};

		const auto pipe = _popen("ipconfig", "r");
		if (!pipe)
			return std::string{};

		while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
		{
			std::string line(buffer.data());
			std::smatch match{};
			if (std::regex_search(line, match, std::regex(R"(IPv4 Address[^\d]+(\d+\.\d+\.\d+\.\d+))")) && match.size() == 2)
			{
				ip_address = match[1];
				break;
			}
		}
		_pclose(pipe);

		return ip_address;
	}
}