#pragma once

struct config_data_t
{
	bool m_use_localhost;
	std::string m_local_ip;
	std::string m_public_ip;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(config_data_t, m_use_localhost, m_local_ip, m_public_ip)
};

namespace cfg
{
	bool setup(config_data_t& config_data);
}