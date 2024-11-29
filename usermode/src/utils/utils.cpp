#include "pch.hpp"

std::string utils::get_ipv4_address(config_data_t& config_data)
{
	if (config_data.m_use_localhost)
		return "localhost";

	std::string ip_address;
	std::array<char, 128> buffer{};

	const auto pipe = _popen("ipconfig", "r");
	if (!pipe)
	{
		LOG_ERROR("failed to open a pipe to ipconfig");
		return {};
	}

	while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
	{
		std::string line(buffer.data());
		std::smatch match{};
		if (regex_search(line, match, std::regex(R"((192\.168\.\d+\.\d+))")) && match.size() == 2)
		{
			ip_address = match[1];
			break;
		}
	}
	_pclose(pipe);

	return ip_address;
}

static size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* userp)
{
    userp->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

bool utils::is_updated()
{
    const auto curl = curl_easy_init();
    if (!curl)
    {
        LOG_ERROR("failed to initialize curl");
        return {};
    }

    std::string read_buffer = "";
    const std::string url = "https://api.github.com/repos/clauadv/cs2_webradar/releases/latest";

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0l);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0l);

    const auto response = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (response != CURLE_OK)
    {
        LOG_ERROR("curl_easy_perform failed ('%s')", curl_easy_strerror(response));
        return {};
    }

    try
    {
        const auto json_response = nlohmann::json::parse(read_buffer);

        const auto& latest_release_name = json_response["name"].get<std::string>();
        if (latest_release_name != CS2_WEBRADAR_VERSION)
        {
            const auto output = std::format("version mismatch! current 'cs2_webradar' version is '{}'\n           please get the latest version of 'cs2_webradar'", latest_release_name.c_str());
            LOG_WARNING("%s", output.c_str());
            MessageBoxA(nullptr, output.c_str(), "usermode " CS2_WEBRADAR_VERSION, MB_OK);
            return {};
        }
    }
    catch (const nlohmann::json::parse_error& e)
    {
        LOG_ERROR("json parsing error ('%s')", e.what());
        return {};
    }

    return true;
}