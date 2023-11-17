#pragma once

namespace cs2
{
	class c_offsets
	{
	private:
		uintptr_t m_entity_list{ 0 };
		uintptr_t m_global_vars{ 0 };
		uintptr_t m_local_player_pawn{ 0 };
		uintptr_t m_player_pawn{ 0 };
		uintptr_t m_health{ 0 };
		uintptr_t m_armor{ 0 };
		uintptr_t m_team_num{ 0 };
		uintptr_t m_old_origin{ 0 };
		uintptr_t m_sanitized_player_name{ 0 };
		uintptr_t m_comp_teammate_color{ 0 };
		uintptr_t m_item_services{ 0 };
		uintptr_t m_has_defuser{ 0 };
		uintptr_t m_has_helmet{ 0 };
		uintptr_t m_ang_eye_angles{ 0 };
		uintptr_t m_weapon_services{ 0 };
		uintptr_t m_my_weapons{ 0 };
		uintptr_t m_active_weapon{ 0 };
		uintptr_t m_entity{ 0 };
		uintptr_t m_designer_name{ 0 };
		uintptr_t m_subclass_id{ 0 };
		uintptr_t m_weapon_type{ 0 };
		uintptr_t m_name{ 0 };
		uintptr_t m_in_game_money_services{ 0 };
		uintptr_t m_account{ 0 };
		uintptr_t m_game_scene_node{ 0 };
		uintptr_t m_vec_origin{ 0 };
		uintptr_t m_model_state{ 0 };
		uintptr_t m_model_name{ 0 };
		uintptr_t m_steam_id{ 0 };
		uintptr_t m_planted_c4{ 0 };
		uintptr_t m_c4_blow{ 0 };
		uintptr_t m_being_defused{ 0 };
		uintptr_t m_bomb_defused{ 0 };
		uintptr_t m_defuse_countdown{ 0 };
		uintptr_t m_owner_entity{ 0 };

	public:
		bool setup()
		{
			nlohmann::json data{};

			ifstream offsets_json{ "offsets.json" };
			if (!offsets_json.good())
			{
				LOG_ERROR("failed to open offsets.json");
				return false;
			}

			offsets_json >> data;

			this->m_entity_list = data["client_dll"]["data"]["dwEntityList"]["value"];
			this->m_global_vars = data["client_dll"]["data"]["dwGlobalVars"]["value"];
			this->m_local_player_pawn = data["client_dll"]["data"]["dwLocalPlayerPawn"]["value"];
			this->m_planted_c4 = data["client_dll"]["data"]["dwPlantedC4"]["value"];

			data.clear();

			ifstream client_dll_json{ "client.dll.json" };
			if (!client_dll_json.good())
			{
				LOG_ERROR("failed to open client.dll.json");
				return false;
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

			return true;
		}

		uintptr_t get_entity_list() { return this->m_entity_list; }
		uintptr_t get_global_vars() { return this->m_global_vars; }
		uintptr_t get_local_player_pawn() { return this->m_local_player_pawn; }
		uintptr_t get_planted_c4() { return this->m_planted_c4; }
		uintptr_t get_player_pawn() { return this->m_player_pawn; }
		uintptr_t get_health() { return this->m_health; }
		uintptr_t get_armor() { return this->m_armor; }
		uintptr_t get_team_num() { return this->m_team_num; }
		uintptr_t get_old_origin() { return this->m_old_origin; }
		uintptr_t get_sanitized_player_name() { return this->m_sanitized_player_name; }
		uintptr_t get_comp_teammate_color() { return this->m_comp_teammate_color; }
		uintptr_t get_item_services() { return this->m_item_services; }
		uintptr_t get_has_defuser() { return this->m_has_defuser; }
		uintptr_t get_has_helmet() { return this->m_has_helmet; }
		uintptr_t get_ang_eye_angles() { return this->m_ang_eye_angles; }
		uintptr_t get_weapon_services() { return this->m_weapon_services; }
		uintptr_t get_my_weapons() { return this->m_my_weapons; }
		uintptr_t get_active_weapon() { return this->m_active_weapon; }
		uintptr_t get_entity() { return this->m_entity; }
		uintptr_t get_designer_name() { return this->m_designer_name; }
		uintptr_t get_subclass_id() { return this->m_subclass_id; }
		uintptr_t get_weapon_type() { return this->m_weapon_type; }
		uintptr_t get_name() { return this->m_name; }
		uintptr_t get_in_game_money_services() { return this->m_in_game_money_services; }
		uintptr_t get_account() { return this->m_account; }
		uintptr_t get_game_scene_node() { return this->m_game_scene_node; }
		uintptr_t get_vec_origin() { return this->m_vec_origin; }
		uintptr_t get_model_state() { return this->m_model_state; }
		uintptr_t get_model_name() { return this->m_model_name; }
		uintptr_t get_steam_id() { return this->m_steam_id; }
		uintptr_t get_c4_blow() { return this->m_c4_blow; }
		uintptr_t get_being_defused() { return this->m_being_defused; }
		uintptr_t get_bomb_defused() { return this->m_bomb_defused; }
		uintptr_t get_defuse_countdown() { return this->m_defuse_countdown; }
		uintptr_t get_owner_entity() { return this->m_owner_entity; }
	};
}
inline cs2::c_offsets m_offsets{};