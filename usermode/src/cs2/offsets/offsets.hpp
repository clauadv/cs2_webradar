#pragma once

namespace cs2
{
	class c_offsets
	{
	private:
		bool m_initialized{ false };
		std::uintptr_t m_entity_list{ 0 };
		std::uintptr_t m_global_vars{ 0 };
		std::uintptr_t m_local_player_pawn{ 0 };
		std::uintptr_t m_player_pawn{ 0 };
		std::uintptr_t m_health{ 0 };
		std::uintptr_t m_armor{ 0 };
		std::uintptr_t m_team_num{ 0 };
		std::uintptr_t m_old_origin{ 0 };
		std::uintptr_t m_sanitized_player_name{ 0 };
		std::uintptr_t m_comp_teammate_color{ 0 };
		std::uintptr_t m_item_services{ 0 };
		std::uintptr_t m_has_defuser{ 0 };
		std::uintptr_t m_has_helmet{ 0 };
		std::uintptr_t m_ang_eye_angles{ 0 };
		std::uintptr_t m_weapon_services{ 0 };
		std::uintptr_t m_my_weapons{ 0 };
		std::uintptr_t m_active_weapon{ 0 };
		std::uintptr_t m_entity{ 0 };
		std::uintptr_t m_designer_name{ 0 };
		std::uintptr_t m_subclass_id{ 0 };
		std::uintptr_t m_weapon_type{ 0 };
		std::uintptr_t m_name{ 0 };
		std::uintptr_t m_in_game_money_services{ 0 };
		std::uintptr_t m_account{ 0 };
		std::uintptr_t m_game_scene_node{ 0 };
		std::uintptr_t m_vec_origin{ 0 };
		std::uintptr_t m_model_state{ 0 };
		std::uintptr_t m_model_name{ 0 };
		std::uintptr_t m_steam_id{ 0 };
		std::uintptr_t m_planted_c4{ 0 };
		std::uintptr_t m_c4_blow{ 0 };
		std::uintptr_t m_being_defused{ 0 };
		std::uintptr_t m_bomb_defused{ 0 };
		std::uintptr_t m_defuse_countdown{ 0 };
		std::uintptr_t m_owner_entity{ 0 };

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

			this->m_entity_list = data["client_dll"]["data"]["dwEntityList"]["value"];
			this->m_global_vars = data["client_dll"]["data"]["dwGlobalVars"]["value"];
			this->m_local_player_pawn = data["client_dll"]["data"]["dwLocalPlayerPawn"]["value"];
			this->m_planted_c4 = data["client_dll"]["data"]["dwPlantedC4"]["value"];

		#ifdef DEVELOPER
			LOG_INFO("this->m_entity_list -> 0x%llx", this->m_entity_list);
			LOG_INFO("this->m_global_vars -> 0x%llx", this->m_global_vars);
			LOG_INFO("this->m_local_player_pawn -> 0x%llx", this->m_local_player_pawn);
			LOG_INFO("this->m_planted_c4 -> 0x%llx \n", this->m_planted_c4);
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

			this->m_player_pawn = data["CCSPlayerController"]["data"]["m_hPlayerPawn"]["value"];
			this->m_health = data["C_BaseEntity"]["data"]["m_iHealth"]["value"];
			this->m_armor = data["C_CSPlayerPawnBase"]["data"]["m_ArmorValue"]["value"];
			this->m_team_num = data["C_BaseEntity"]["data"]["m_iTeamNum"]["value"];
			this->m_old_origin = data["C_BasePlayerPawn"]["data"]["m_vOldOrigin"]["value"];
			this->m_sanitized_player_name = data["CCSPlayerController"]["data"]["m_sSanitizedPlayerName"]["value"];
			this->m_comp_teammate_color = data["CCSPlayerController"]["data"]["m_iCompTeammateColor"]["value"];
			this->m_item_services = data["C_BasePlayerPawn"]["data"]["m_pItemServices"]["value"];
			this->m_has_defuser = data["CCSPlayer_ItemServices"]["data"]["m_bHasDefuser"]["value"];
			this->m_has_helmet = data["CCSPlayer_ItemServices"]["data"]["m_bHasHelmet"]["value"];
			this->m_ang_eye_angles = data["C_CSPlayerPawnBase"]["data"]["m_angEyeAngles"]["value"];
			this->m_weapon_services = data["C_BasePlayerPawn"]["data"]["m_pWeaponServices"]["value"];
			this->m_my_weapons = data["CPlayer_WeaponServices"]["data"]["m_hMyWeapons"]["value"];
			this->m_active_weapon = data["CPlayer_WeaponServices"]["data"]["m_hActiveWeapon"]["value"];
			this->m_entity = data["CEntityInstance"]["data"]["m_pEntity"]["value"];
			this->m_designer_name = data["CEntityIdentity"]["data"]["m_designerName"]["value"];
			this->m_subclass_id = data["C_BaseEntity"]["data"]["m_nSubclassID"]["value"];
			this->m_weapon_type = data["CCSWeaponBaseVData"]["data"]["m_WeaponType"]["value"];
			this->m_name = data["CCSWeaponBaseVData"]["data"]["m_szName"]["value"];
			this->m_in_game_money_services = data["CCSPlayerController"]["data"]["m_pInGameMoneyServices"]["value"];
			this->m_account = data["CCSPlayerController_InGameMoneyServices"]["data"]["m_iAccount"]["value"];
			this->m_game_scene_node = data["C_BaseEntity"]["data"]["m_pGameSceneNode"]["value"];
			this->m_vec_origin = data["CGameSceneNode"]["data"]["m_vecAbsOrigin"]["value"];
			this->m_model_state = data["CSkeletonInstance"]["data"]["m_modelState"]["value"];
			this->m_model_name = data["CModelState"]["data"]["m_ModelName"]["value"];
			this->m_steam_id = data["CBasePlayerController"]["data"]["m_steamID"]["value"];
			this->m_c4_blow = data["C_PlantedC4"]["data"]["m_flC4Blow"]["value"];
			this->m_being_defused = data["C_PlantedC4"]["data"]["m_bBeingDefused"]["value"];
			this->m_bomb_defused = data["C_PlantedC4"]["data"]["m_bBombDefused"]["value"];
			this->m_defuse_countdown = data["C_PlantedC4"]["data"]["m_flDefuseCountDown"]["value"];
			this->m_owner_entity = data["C_BaseEntity"]["data"]["m_hOwnerEntity"]["value"];

		#ifdef DEVELOPER
			LOG_INFO("this->m_player_pawn -> 0x%llx", this->m_player_pawn);
			LOG_INFO("this->m_health -> 0x%llx", this->m_health);
			LOG_INFO("this->m_armor -> 0x%llx", this->m_armor);
			LOG_INFO("this->m_team_num -> 0x%llx", this->m_team_num);
			LOG_INFO("this->m_old_origin -> 0x%llx", this->m_old_origin);
			LOG_INFO("this->m_sanitized_player_name -> 0x%llx", this->m_sanitized_player_name);
			LOG_INFO("this->m_comp_teammate_color -> 0x%llx", this->m_comp_teammate_color);
			LOG_INFO("this->m_item_services -> 0x%llx", this->m_item_services);
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
			LOG_INFO("this->m_name -> 0x%llx", this->m_name);
			LOG_INFO("this->m_in_game_money_services -> 0x%llx", this->m_in_game_money_services);
			LOG_INFO("this->m_account -> 0x%llx", this->m_account);
			LOG_INFO("this->m_game_scene_node -> 0x%llx", this->m_game_scene_node);
			LOG_INFO("this->m_vec_origin -> 0x%llx", this->m_vec_origin);
			LOG_INFO("this->m_model_state -> 0x%llx", this->m_model_state);
			LOG_INFO("this->m_model_name -> 0x%llx", this->m_model_name);
			LOG_INFO("this->m_steam_id -> 0x%llx", this->m_steam_id);
			LOG_INFO("this->m_c4_blow -> 0x%llx", this->m_c4_blow);
			LOG_INFO("this->m_being_defused -> 0x%llx", this->m_being_defused);
			LOG_INFO("this->m_bomb_defused -> 0x%llx", this->m_bomb_defused);
			LOG_INFO("this->m_defuse_countdown -> 0x%llx", this->m_defuse_countdown);
			LOG_INFO("this->m_owner_entity -> 0x%llx \n", this->m_owner_entity);
		#endif

			this->m_initialized = true;
		}

		bool is_initialized()
		{
			return this->m_initialized;
		}

		std::uintptr_t get_entity_list() { return this->m_entity_list; }
		std::uintptr_t get_global_vars() { return this->m_global_vars; }
		std::uintptr_t get_local_player_pawn() { return this->m_local_player_pawn; }
		std::uintptr_t get_planted_c4() { return this->m_planted_c4; }
		std::uintptr_t get_player_pawn() { return this->m_player_pawn; }
		std::uintptr_t get_health() { return this->m_health; }
		std::uintptr_t get_armor() { return this->m_armor; }
		std::uintptr_t get_team_num() { return this->m_team_num; }
		std::uintptr_t get_old_origin() { return this->m_old_origin; }
		std::uintptr_t get_sanitized_player_name() { return this->m_sanitized_player_name; }
		std::uintptr_t get_comp_teammate_color() { return this->m_comp_teammate_color; }
		std::uintptr_t get_item_services() { return this->m_item_services; }
		std::uintptr_t get_has_defuser() { return this->m_has_defuser; }
		std::uintptr_t get_has_helmet() { return this->m_has_helmet; }
		std::uintptr_t get_ang_eye_angles() { return this->m_ang_eye_angles; }
		std::uintptr_t get_weapon_services() { return this->m_weapon_services; }
		std::uintptr_t get_my_weapons() { return this->m_my_weapons; }
		std::uintptr_t get_active_weapon() { return this->m_active_weapon; }
		std::uintptr_t get_entity() { return this->m_entity; }
		std::uintptr_t get_designer_name() { return this->m_designer_name; }
		std::uintptr_t get_subclass_id() { return this->m_subclass_id; }
		std::uintptr_t get_weapon_type() { return this->m_weapon_type; }
		std::uintptr_t get_name() { return this->m_name; }
		std::uintptr_t get_in_game_money_services() { return this->m_in_game_money_services; }
		std::uintptr_t get_account() { return this->m_account; }
		std::uintptr_t get_game_scene_node() { return this->m_game_scene_node; }
		std::uintptr_t get_vec_origin() { return this->m_vec_origin; }
		std::uintptr_t get_model_state() { return this->m_model_state; }
		std::uintptr_t get_model_name() { return this->m_model_name; }
		std::uintptr_t get_steam_id() { return this->m_steam_id; }
		std::uintptr_t get_c4_blow() { return this->m_c4_blow; }
		std::uintptr_t get_being_defused() { return this->m_being_defused; }
		std::uintptr_t get_bomb_defused() { return this->m_bomb_defused; }
		std::uintptr_t get_defuse_countdown() { return this->m_defuse_countdown; }
		std::uintptr_t get_owner_entity() { return this->m_owner_entity; }
	};
}
inline cs2::c_offsets m_offsets{};