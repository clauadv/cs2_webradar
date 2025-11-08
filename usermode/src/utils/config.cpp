#include "pch.hpp"

int cfg::setup(config_data_t& config_data)
{
	std::ifstream file("config.json");
	if (!file.is_open())
	{
		std::ofstream example_config("config.json");
		example_config << std::format("{}", R"({
    "m_use_localhost": true,
    "m_local_ip": "192.168.x.x",
    "m_public_ip": "x.x.x.x"
})");
		LOG_WARNING("Couldn't open config.json file, please check files and configure it.");
		return 1;
	}

	const auto parsed_data = nlohmann::json::parse(file);
	if (parsed_data.empty())
	{
		LOG_WARNING("Failed to parse config.json, please check syntax.");
		return 2;
	}

	try
	{
		config_data = parsed_data.get<config_data_t>();
	}
	catch (const std::exception& e)
	{
		LOG_WARNING("Failed to deserialize config.json.");
		return 3;
	}

	return 0;
}