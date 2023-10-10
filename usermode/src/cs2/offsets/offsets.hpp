#pragma once

namespace cs2
{
	class c_offsets
	{
	private:
		bool m_initialized{ false };
		std::uint64_t m_entity_list{ 0 };
		std::uint64_t m_global_vars{ 0 };
		std::uint64_t m_local_player_pawn{ 0 };
		std::uint64_t m_player_pawn{ 0 };
		std::uint64_t m_health{ 0 };
		std::uint64_t m_armor{ 0 };
		std::uint64_t m_team_num{ 0 };
		std::uint64_t m_old_origin{ 0 };
		std::uint64_t m_sanitized_player_name{ 0 };
		std::uint64_t m_comp_teammate_color{ 0 };
		std::uint64_t m_has_defuser{ 0 };
		std::uint64_t m_has_helmet{ 0 };
		std::uint64_t m_ang_eye_angles{ 0 };
		std::uint64_t m_weapon_services{ 0 };
		std::uint64_t m_my_weapons{ 0 };
		std::uint64_t m_active_weapon{ 0 };
		std::uint64_t m_entity{ 0 };
		std::uint64_t m_designer_name{ 0 };
		std::uint64_t m_subclass_id{ 0 };
		std::uint64_t m_weapon_type{ 0 };
		std::uint64_t m_action_tracking_services{ 0 };
		std::uint64_t m_match_stats{ 0 };
		std::uint64_t m_kills{ 0 };
		std::uint64_t m_deaths{ 0 };
		std::uint64_t m_assists{ 0 };

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

			this->m_player_pawn = data["CCSPlayerController"]["m_hPlayerPawn"];
			this->m_health = data["C_BaseEntity"]["m_iHealth"];
			this->m_armor = data["C_CSPlayerPawnBase"]["m_ArmorValue"];
			this->m_team_num = data["C_BaseEntity"]["m_iTeamNum"];
			this->m_old_origin = data["C_BasePlayerPawn"]["m_vOldOrigin"];
			this->m_sanitized_player_name = data["CCSPlayerController"]["m_sSanitizedPlayerName"];
			this->m_comp_teammate_color = data["CCSPlayerController"]["m_iCompTeammateColor"];
			this->m_has_defuser = data["CCSPlayerController"]["m_bPawnHasDefuser"];
			this->m_has_helmet = data["CCSPlayerController"]["m_bPawnHasHelmet"];
			this->m_ang_eye_angles = data["C_CSPlayerPawnBase"]["m_angEyeAngles"];
			this->m_weapon_services = data["C_BasePlayerPawn"]["m_pWeaponServices"];
			this->m_my_weapons = data["CPlayer_WeaponServices"]["m_hMyWeapons"];
			this->m_active_weapon = data["CPlayer_WeaponServices"]["m_hActiveWeapon"];
			this->m_entity = data["CEntityInstance"]["m_pEntity"];
			this->m_designer_name = data["CEntityIdentity"]["m_designerName"];
			this->m_subclass_id = data["C_BaseEntity"]["m_nSubclassID"];
			this->m_weapon_type = data["CCSWeaponBaseVData"]["m_WeaponType"];
			this->m_action_tracking_services = data["CCSPlayerController"]["m_pActionTrackingServices"];
			this->m_match_stats = data["CCSPlayerController_ActionTrackingServices"]["m_matchStats"];
			this->m_kills = data["CSPerRoundStats_t"]["m_iKills"];
			this->m_deaths = data["CSPerRoundStats_t"]["m_iDeaths"];
			this->m_assists = data["CSPerRoundStats_t"]["m_iAssists"];

		#ifdef DEVELOPER
			LOG_INFO("this->m_player_pawn -> 0x%llx", this->m_player_pawn);
			LOG_INFO("this->m_health -> 0x%llx", this->m_health);
			LOG_INFO("this->m_armor -> 0x%llx", this->m_armor);
			LOG_INFO("this->m_team_num -> 0x%llx", this->m_team_num);
			LOG_INFO("this->m_old_origin -> 0x%llx", this->m_old_origin);
			LOG_INFO("this->m_sanitized_player_name -> 0x%llx", this->m_sanitized_player_name);
			LOG_INFO("this->m_comp_teammate_color -> 0x%llx", this->m_comp_teammate_color);
			LOG_INFO("this->m_has_defuser -> 0x%llx", this->m_has_defuser);
			LOG_INFO("this->m_has_helmet -> 0x%llx", this->m_has_helmet);
			LOG_INFO("this->m_ang_eye_angles -> 0x%llx", this->m_ang_eye_angles);
			LOG_INFO("this->m_weapon_services -> 0x%llx", this->m_weapon_services);
			LOG_INFO("this->m_my_weapons -> 0x%llx", this->m_my_weapons);
			LOG_INFO("this->m_entity -> 0x%llx", this->m_entity);
			LOG_INFO("this->m_active_weapon -> 0x%llx", this->m_active_weapon);
			LOG_INFO("this->m_designer_name -> 0x%llx", this->m_designer_name);
			LOG_INFO("this->m_subclass_id -> 0x%llx", this->m_subclass_id);
			LOG_INFO("this->m_weapon_type -> 0x%llx", this->m_weapon_type);
			LOG_INFO("this->m_action_tracking_services -> 0x%llx", this->m_action_tracking_services);
			LOG_INFO("this->m_match_stats -> 0x%llx", this->m_match_stats);
			LOG_INFO("this->m_kills -> 0x%llx", this->m_kills);
			LOG_INFO("this->m_deaths -> 0x%llx", this->m_deaths);
			LOG_INFO("this->m_assists -> 0x%llx \n", this->m_assists);
		#endif

			this->m_initialized = true;
		}

		bool is_initialized()
		{
			return this->m_initialized;
		}

		std::uint64_t get_entity_list() { return this->m_entity_list; }
		std::uint64_t get_global_vars() { return this->m_global_vars; }
		std::uint64_t get_local_player_pawn() { return this->m_local_player_pawn; }
		std::uint64_t get_player_pawn() { return this->m_player_pawn; }
		std::uint64_t get_health() { return this->m_health; }
		std::uint64_t get_armor() { return this->m_armor; }
		std::uint64_t get_team_num() { return this->m_team_num; }
		std::uint64_t get_old_origin() { return this->m_old_origin; }
		std::uint64_t get_sanitized_player_name() { return this->m_sanitized_player_name; }
		std::uint64_t get_comp_teammate_color() { return this->m_comp_teammate_color; }
		std::uint64_t get_has_defuser() { return this->m_has_defuser; }
		std::uint64_t get_has_helmet() { return this->m_has_helmet; }
		std::uint64_t get_ang_eye_angles() { return this->m_ang_eye_angles; }
		std::uint64_t get_weapon_services() { return this->m_weapon_services; }
		std::uint64_t get_my_weapons() { return this->m_my_weapons; }
		std::uint64_t get_active_weapon() { return this->m_active_weapon; }
		std::uint64_t get_entity() { return this->m_entity; }
		std::uint64_t get_designer_name() { return this->m_designer_name; }
		std::uint64_t get_subclass_id() { return this->m_subclass_id; }
		std::uint64_t get_weapon_type() { return this->m_weapon_type; }
		std::uint64_t get_action_tracking_services() { return this->m_action_tracking_services; }
		std::uint64_t get_math_stats() { return this->m_match_stats; }
		std::uint64_t get_kills() { return this->m_kills; }
		std::uint64_t get_deaths() { return this->m_deaths; }
		std::uint64_t get_assists() { return this->m_assists; }
	};
}
inline cs2::c_offsets m_offsets{};