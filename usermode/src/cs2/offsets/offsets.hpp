#pragma once

namespace usermode
{
	class c_offsets
	{
	private:
		bool m_initialized{ false };
		std::uint64_t m_entity_list{ 0 };
		std::uint64_t m_global_vars{ 0 };
		std::uint64_t m_local_player_pawn{ 0 };
		std::uint64_t m_h_player_pawn{ 0 };
		std::uint64_t m_i_health{ 0 };
		std::uint64_t m_i_team_num{ 0 };
		std::uint64_t m_old_origin{ 0 };
	public:
		c_offsets()
		{
			nlohmann::json data{};

			std::ifstream offsets_json{ "offsets.json" };
			if (!offsets_json.good())
			{
				LOG_ERROR("failed to open offsets.json");
				this->m_initialized = false;
				return;
			}

			offsets_json >> data;

			this->m_entity_list = data["client_dll"]["entityList"];
			this->m_global_vars = data["client_dll"]["globalVars"];
			this->m_local_player_pawn = data["client_dll"]["localPlayerPawn"];

		#ifdef DEVELOPER
			LOG_INFO("this->m_entity_list -> 0x%llx", this->m_entity_list);
			LOG_INFO("this->m_global_vars -> 0x%llx", this->m_global_vars);
			LOG_INFO("this->m_local_player_pawn -> 0x%llx", this->m_local_player_pawn);
		#endif

			data.clear();

			std::ifstream client_dll_json{ "client.dll.json" };
			if (!client_dll_json.good())
			{
				LOG_ERROR("failed to open client.dll.json");
				this->m_initialized = false;
				return;
			}

			client_dll_json >> data;

			this->m_h_player_pawn = data["CCSPlayerController"]["m_hPlayerPawn"];
			this->m_i_health = data["C_BaseEntity"]["m_iHealth"];
			this->m_i_team_num = data["C_BaseEntity"]["m_iTeamNum"];
			this->m_old_origin = data["C_BasePlayerPawn"]["m_vOldOrigin"];

		#ifdef DEVELOPER
			LOG_INFO("this->m_h_player_pawn -> 0x%llx", this->m_h_player_pawn);
			LOG_INFO("this->m_i_health -> 0x%llx", this->m_i_health);
			LOG_INFO("this->m_i_team_num -> 0x%llx", this->m_i_team_num);
			LOG_INFO("this->m_old_origin -> 0x%llx", this->m_old_origin);
		#endif

			this->m_initialized = true;
		}

		bool is_initialized()
		{
			return this->m_initialized;
		}

		std::uint64_t get_entity_list()
		{
			return this->m_entity_list;
		}

		std::uint64_t get_global_vars()
		{
			return this->m_global_vars;
		}

		std::uint64_t get_local_player_pawn()
		{
			return this->m_local_player_pawn;
		}

		std::uint64_t get_h_player_pawn()
		{
			return this->m_h_player_pawn;
		}

		std::uint64_t get_health()
		{
			return this->m_i_health;
		}

		std::uint64_t get_team_num()
		{
			return this->m_i_team_num;
		}

		std::uint64_t get_old_origin()
		{
			return this->m_old_origin;
		}
	};
	inline usermode::c_offsets m_offsets{};
}