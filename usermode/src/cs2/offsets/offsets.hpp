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
		std::uint64_t m_sanitized_player_name{ 0 };
		std::uint64_t m_comp_teammate_color{ 0 };
		std::uint64_t m_has_defuser{ 0 };
		std::uint64_t m_has_helmet{ 0 };
		std::uint64_t m_ang_eye_angles{ 0 };
		std::uint64_t m_weapon_services{ 0 };
		std::uint64_t m_h_my_weapons{ 0 };
		std::uint64_t m_attribute_manager{ 0 };
		std::uint64_t m_item{ 0 };
		std::uint64_t m_item_definition_index{ 0 };

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
			this->m_sanitized_player_name = data["CCSPlayerController"]["m_sSanitizedPlayerName"];
			this->m_comp_teammate_color = data["CCSPlayerController"]["m_iCompTeammateColor"];
			this->m_has_defuser = data["CCSPlayerController"]["m_bPawnHasDefuser"];
			this->m_has_helmet = data["CCSPlayerController"]["m_bPawnHasHelmet"];
			this->m_ang_eye_angles = data["C_CSPlayerPawnBase"]["m_angEyeAngles"];
			this->m_weapon_services = data["C_BasePlayerPawn"]["m_pWeaponServices"];
			this->m_h_my_weapons = data["CPlayer_WeaponServices"]["m_hMyWeapons"];
			this->m_attribute_manager = data["C_EconEntity"]["m_AttributeManager"];
			this->m_item = data["C_AttributeContainer"]["m_Item"];
			this->m_item_definition_index = data["C_EconItemView"]["m_iItemDefinitionIndex"];

		#ifdef DEVELOPER
			LOG_INFO("this->m_h_player_pawn -> 0x%llx", this->m_h_player_pawn);
			LOG_INFO("this->m_i_health -> 0x%llx", this->m_i_health);
			LOG_INFO("this->m_i_team_num -> 0x%llx", this->m_i_team_num);
			LOG_INFO("this->m_old_origin -> 0x%llx", this->m_old_origin);
			LOG_INFO("this->m_sanitized_player_name -> 0x%llx", this->m_sanitized_player_name);
			LOG_INFO("this->m_comp_teammate_color -> 0x%llx", this->m_comp_teammate_color);
			LOG_INFO("this->m_has_defuser -> 0x%llx", this->m_has_defuser);
			LOG_INFO("this->m_has_helmet -> 0x%llx", this->m_has_helmet);
			LOG_INFO("this->m_ang_eye_angles -> 0x%llx", this->m_ang_eye_angles);
			LOG_INFO("this->m_weapon_services -> 0x%llx", this->m_weapon_services);
			LOG_INFO("this->m_h_my_weapons -> 0x%llx", this->m_h_my_weapons);
			LOG_INFO("this->m_attribute_manager -> 0x%llx", this->m_attribute_manager);
			LOG_INFO("this->m_item -> 0x%llx", this->m_item);
			LOG_INFO("this->m_item_definition_index -> 0x%llx \n", this->m_item_definition_index);
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
		std::uint64_t get_h_player_pawn() { return this->m_h_player_pawn; }
		std::uint64_t get_health() { return this->m_i_health; }
		std::uint64_t get_team_num() { return this->m_i_team_num; }
		std::uint64_t get_old_origin() { return this->m_old_origin; }
		std::uint64_t get_sanitized_player_name() { return this->m_sanitized_player_name; }
		std::uint64_t get_comp_teammate_color() { return this->m_comp_teammate_color; }
		std::uint64_t get_has_defuser() { return this->m_has_defuser; }
		std::uint64_t get_has_helmet() { return this->m_has_helmet; }
		std::uint64_t get_ang_eye_angles() { return this->m_ang_eye_angles; }
		std::uint64_t get_weapon_services() { return this->m_weapon_services; }
		std::uint64_t get_h_my_weapons() { return this->m_h_my_weapons; }
		std::uint64_t get_attribute_manager() { return this->m_attribute_manager; }
		std::uint64_t get_item() { return this->m_item; }
		std::uint64_t get_item_definition_index() { return this->m_item_definition_index; }
	};
	inline usermode::c_offsets m_offsets{};
}