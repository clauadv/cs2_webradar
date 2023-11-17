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

	inline int get_version()
	{
		int version;

		wchar_t filename[MAX_PATH] = {};
		if (!GetModuleFileNameExW(m_memory.get_handle(), NULL, filename, MAX_PATH))
			return {};

		const wstring name = filename;
		const auto last_backslash = name.find_last_of(L'\\');

		if (last_backslash == wstring::npos || last_backslash == 0)
			return {};

		const auto second_backslash = name.find_last_of(L'\\', last_backslash - 1);
		const auto third_backslash = name.find_last_of(L'\\', second_backslash - 1);

		if (second_backslash == wstring::npos || third_backslash == wstring::npos)
			return {};

		const auto path = name.substr(0, third_backslash) + L"\\csgo\\steam.inf";

		ifstream in(path);
		string line;

		if (getline(in, line) && !line.empty())
		{
			regex regex("=");
			sregex_token_iterator it(line.begin(), line.end(), regex, -1);
			sregex_token_iterator end;

			if (it != end)
			{
				++it;
				version = stoi(it->str());
			}
		}

		return version;
	}
}