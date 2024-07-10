#include "pch.hpp"

void f::run()
{
	sdk::update();

	const auto local_pawn = c_cs_player_pawn::m_local_pawn;
	if (!local_pawn)
		return;

	const auto team = local_pawn->m_iTeamNum();
	if (team == e_cs_team::team_unassigned ||
		team == e_cs_team::team_spectator)
		return;

	m_data = nlohmann::json{};
	m_player_data = nlohmann::json{};

	m_data["m_local_team"] = team;

	get_map();
	get_player_info();
}

void f::get_map()
{
	const auto global_vars = c_global_vars::m_global_vars;
	if (!global_vars)
		return;

	const auto map_name = global_vars->m_map_name();
	if (map_name.empty() || map_name.find("<empty>") != std::string::npos)
	{
		m_data["m_map"] = "invalid";
		LOG_ERROR("failed to get map name");
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}

	m_data["m_map"] = map_name;
}

void f::get_player_info()
{
	m_data["m_players"] = nlohmann::json::array();

	const auto entity_list = c_game_entity_system::m_entity_list;
	if (!entity_list)
		return;

	const auto highest_idx = entity_list->m_highest_entity_idx();
	for (int32_t idx = 0; idx < highest_idx; idx++)
	{
		const auto entity = entity_list->get(idx);
		if (!entity)
			continue;

		const auto entity_handle = entity->get_ref_e_handle();
		if (!entity_handle.is_valid())
			continue;

		const auto class_name = entity->get_schema_class_name();
		if (class_name.empty())
			continue;

		const auto hashed_class_name = fnv1a::hash(class_name);

		if (hashed_class_name == fnv1a::hash("CCSPlayerController"))
		{
			const auto player = entity_list->get<c_cs_player_controller*>(entity_handle);
			if (!player)
				continue;

			const auto player_pawn = entity_list->get<c_cs_player_pawn*>(player->m_hPlayerPawn());
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
			if (!bomb)
				continue;

			f::bomb::get_carried_bomb(bomb);
		}
		else if (hashed_class_name == fnv1a::hash("C_PlantedC4"))
		{
			const auto planted_c4 = reinterpret_cast<c_planted_c4*>(entity);
			if (!planted_c4)
				continue;

			f::bomb::get_planted_bomb(planted_c4);
		}
	}
}