#include "pch.hpp"


bool f::run()
{
	const auto local_team = sdk::m_local_controller->m_iTeamNum();
	if (local_team == e_team::none || local_team == e_team::spec)
		return false;

	m_data = nlohmann::json{};
	m_player_data = nlohmann::json{};
	m_grenade_data = nlohmann::json{};
	m_grenade_thrown_data = nlohmann::json{};
	m_dropped_weapon_data = nlohmann::json{};

	m_data["m_local_team"] = local_team;

	get_map();
	get_player_info();
	return true;
}

void f::get_map()
{
	const auto map_name = i::m_global_vars->m_map_name();
	if (map_name.empty() || map_name.find("<empty>") != std::string::npos || map_name == "SetVelocityLimit")
	{
		m_data["m_map"] = "invalid";

		LOG_WARNING("failed to get map name! updating m_global_vars");
		i::m_global_vars = m_memory->read_t<c_global_vars*>(m_memory->find_pattern(CLIENT_DLL, GET_GLOBAL_VARS)->rip().as<c_global_vars*>());
	}

	m_data["m_map"] = map_name;
}

void f::get_player_info()
{
	m_data["m_players"] = nlohmann::json::array();
	m_data["m_grenades"]["landed"] = nlohmann::json::array();
	m_data["m_grenades"]["thrown"] = nlohmann::json::array();
	m_data["m_dropped_weapons"] = nlohmann::json::array();

	const auto highest_idx = 1024;
	for (int32_t idx = 0; idx < highest_idx; idx++)
	{
		const auto entity = i::m_game_entity_system->get(idx);
		if (!entity)
			continue;

		const auto entity_handle = entity->get_ref_e_handle();
		if (!entity_handle.is_valid())
			continue;

		const auto class_name = entity->get_schema_class_name();
		if (class_name.empty())
			continue;

		const auto hashed_class_name = fnv1a::hash(class_name);

		std::string designer_name = entity->m_pEntity()->m_designerName();

		if (hashed_class_name == fnv1a::hash("CCSPlayerController"))
		{
			const auto player = i::m_game_entity_system->get<c_cs_player_controller*>(entity_handle);
			if (!player)
				continue;

			const auto player_pawn = player->get_player_pawn();
			if (!player_pawn)
				continue;

			if (!f::players::get_data(idx, player, player_pawn))
				continue;

			f::players::get_weapons(player_pawn);
			f::players::get_active_weapon(player_pawn);

			m_data["m_players"].push_back(m_player_data);
		}
		else if (hashed_class_name == fnv1a::hash("C_C4"))
		{
			const auto bomb = entity;
			f::bomb::get_carried_bomb(bomb);
		}
		else if (hashed_class_name == fnv1a::hash("C_PlantedC4"))
		{
			const auto planted_c4 = reinterpret_cast<c_planted_c4*>(entity);
			f::bomb::get_planted_bomb(planted_c4);
		}
		else if (hashed_class_name == fnv1a::hash("C_SmokeGrenadeProjectile")) {

			m_grenade_data = {};
			m_grenade_thrown_data = {};

			const auto smoke = reinterpret_cast<c_smoke_grenade*>(entity);
			f::grenades::get_smoke(smoke);

			if (m_grenade_data.empty()) {
				f::grenades::get_thrown(reinterpret_cast<c_base_grenade*>(entity), designer_name);

				if (!m_grenade_thrown_data.empty()) {

					m_grenade_thrown_data["m_idx"] = idx;
					m_data["m_grenades"]["thrown"].push_back(m_grenade_thrown_data);

				}
				
				continue;
			}

			m_data["m_grenades"]["landed"].push_back(m_grenade_data);
		}
		else if (hashed_class_name == fnv1a::hash("C_Inferno")) {

			m_grenade_data = {};

			const auto molo = reinterpret_cast<c_molo_grenade*>(entity);
			f::grenades::get_molo(molo);

			if (m_grenade_data.empty())
				continue;

			m_data["m_grenades"]["landed"].push_back(m_grenade_data);
		}
		else if (hashed_class_name == fnv1a::hash("C_HEGrenadeProjectile") || hashed_class_name == fnv1a::hash("C_FlashbangProjectile") || hashed_class_name == fnv1a::hash("C_DecoyProjectile") || hashed_class_name == fnv1a::hash("C_MolotovProjectile"))
		{
			m_grenade_thrown_data = {};

			f::grenades::get_thrown(reinterpret_cast<c_base_grenade*>(entity), designer_name);

			if (m_grenade_thrown_data.empty())
				continue;

			m_grenade_thrown_data["m_idx"] = idx;

			m_data["m_grenades"]["thrown"].push_back(m_grenade_thrown_data);
		}
		else if (!designer_name.empty()) {
			if (f::dropped_weapons::is_weapon(designer_name)) {

				m_dropped_weapon_data = {};

				const auto weapon = reinterpret_cast<c_base_entity*>(entity);
				f::dropped_weapons::get_weapon(weapon);

				if (m_dropped_weapon_data.empty())
					continue;

				m_dropped_weapon_data["m_idx"] = idx;

				m_data["m_dropped_weapons"].push_back(m_dropped_weapon_data);
			}
		}
	}
}