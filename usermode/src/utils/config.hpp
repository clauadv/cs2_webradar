#pragma once

struct config_data_t
{
        bool m_use_localhost;
        std::string m_local_ip;
        std::string m_public_ip;
        int m_ws_port{ 22006 };

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(config_data_t, m_use_localhost, m_local_ip, m_public_ip, m_ws_port)
};

namespace cfg
{
	bool setup(config_data_t& config_data);
}