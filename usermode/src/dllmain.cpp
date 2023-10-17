// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.hpp"

bool main()
{
	if (!m_driver.is_initialized())
	{
		LOG_ERROR("failed to initialize driver communication");
		std::this_thread::sleep_for(std::chrono::seconds(5));
		return true;
	}

	// @easywsclient
	WSADATA wsa_data{};
	const auto wsa_startup = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (wsa_startup != 0)
	{
		LOG_ERROR("failed to initialize winsock");
		std::this_thread::sleep_for(std::chrono::seconds(5));
		return true;
	}

	static auto web_socket = easywsclient::WebSocket::from_url("ws://188.24.175.93:22006/cs2_webradar");
	if (!web_socket)
	{
		LOG_ERROR("failed to connect to the web socket");
		std::this_thread::sleep_for(std::chrono::seconds(5));
		return true;
	}

	auto start = std::chrono::system_clock::now();

	for (;;)
	{
		const auto now = std::chrono::system_clock::now();
		const auto duration = now - start;
		if (duration > std::chrono::milliseconds(100))
		{
			start = now;

			const auto game_build = m_cs2.get_game_build();
			if (game_build != "8418039")
			{
				LOG_ERROR("game build mismatch (new: %s)", game_build.data());
				break;
			}

			const auto local_player = m_cs2.get_local_player();
			if (!local_player)
				continue;

			const auto local_team = local_player->get_team();
			if (local_team == cs2::e_team::none || local_team == cs2::e_team::spectator)
				continue;

			const auto global_vars = m_cs2.get_global_vars();
			if (!global_vars)
				continue;

			nlohmann::json data{};
			data["players"] = nlohmann::json::array();
			data["m_local_team"] = local_team;
			data["m_map"] = global_vars->get_map_name();

			const auto entity_list = m_cs2.get_entity_list();
			if (!entity_list)
				continue;

			for (std::size_t idx{ 0 }; idx < 1024 && global_vars->get_map_name().compare("invalid"); idx++)
			{
				const auto entity = entity_list->get<cs2::c_base_entity*>(idx);
				if (!entity)
					continue;

				const auto name = entity->get_name();
				if (!name.contains("c4"))
					continue;

				const auto game_scene_node = m_driver.read_t<std::uint64_t>(entity + 0x310);
				const auto vec_abs_origin = m_driver.read_t<math::fvector3>(game_scene_node + 0xc8);

				// LOG_INFO("vec_abs_origin -> (%f, %f)", vec_abs_origin.x, vec_abs_origin.y);

				data["bomb"]["x"] = vec_abs_origin.x;
				data["bomb"]["y"] = vec_abs_origin.y;
			}

			for (std::size_t idx{ 0 }; idx < 64 && global_vars->get_map_name().compare("invalid"); idx++)
			{
				const auto entity = entity_list->get<cs2::c_base_entity*>(idx);
				if (!entity)
					continue;

				const auto entity_pawn = entity->get_pawn();
				if (!entity_pawn)
					continue;

				const auto player = entity_list->get<cs2::c_base_player*>(entity_pawn & 0x7fff);
				if (!player)
					continue;

				const auto item_services = player->get_item_services();
				if (!item_services)
					continue;

				const auto name = reinterpret_cast<cs2::c_base_player*>(entity)->get_name();
				const auto color = entity->get_color();
				const auto health = player->get_health();
				const auto armor = player->get_armor();
				const auto has_helmet = item_services->has_helmet();
				const auto has_defuser = item_services->has_defuser();
				const auto position = player->get_position();

				const auto eye_angles = player->get_eye_angles().normalize();
				const auto team = player->get_team();
				const auto is_dead = player->is_dead();
				const auto model_name = player->get_model_name();;

				nlohmann::json player_data{};
				player_data["data"]["m_idx"] = idx;
				player_data["data"]["m_name"] = name;
				player_data["data"]["m_color"] = color;
				player_data["data"]["m_health"] = health;
				player_data["data"]["m_armor"] = armor;
				player_data["data"]["m_has_helmet"] = has_helmet;
				player_data["data"]["m_has_defuser"] = has_defuser;
				player_data["data"]["m_position"]["x"] = position.x;
				player_data["data"]["m_position"]["y"] = position.y;
				player_data["data"]["m_position"]["z"] = position.z;
				player_data["data"]["m_eye_angle"] = eye_angles.y;
				player_data["data"]["m_team"] = team;
				player_data["data"]["m_is_dead"] = is_dead;
				player_data["data"]["m_model_name"] = model_name;

				[&]()
				{
					const auto in_game_money_services = entity->get_in_game_money_services();
					if (!in_game_money_services)
						return;

					const auto money = in_game_money_services->get_money();
					player_data["data"]["m_money"] = money;
				}();

				[&]()
				{
					const auto weapon_services = player->get_weapon_services();
					if (!weapon_services)
						return;

					const auto active_weapon = weapon_services->get_active_weapon(entity_list);
					if (!active_weapon)
						return;

					auto weapon_name = active_weapon->get_name();
					weapon_name.erase(weapon_name.begin(), weapon_name.begin() + 7);

					player_data["data"]["m_weapons"]["m_active"] = weapon_name;
				}();

				[&]()
				{
					const auto weapon_services = player->get_weapon_services();
					if (!weapon_services)
						return;

					const auto my_weapons = weapon_services->get_my_weapons();
					if (!my_weapons.first)
						return;

					for (std::size_t idx{ 0 }; idx < my_weapons.first; idx++)
					{
						const auto weapon = my_weapons.second->get(entity_list, idx);
						if (!weapon)
							continue;

						const auto weapon_data = weapon->get_data();
						if (!weapon_data)
							continue;

						const auto weapon_type = weapon_data->get_id();
						auto weapon_name = weapon->get_name();
						weapon_name.erase(weapon_name.begin(), weapon_name.begin() + 7);

						if (weapon_name.find("c4") != std::string::npos)
						{
							player_data["data"]["bomb_carrier"] = true;
						}
						else
						{
							player_data["data"]["bomb_carrier"] = false;
						}

						if (weapon_name.contains("knife") && weapon_name.find("taser") == std::string::npos)
						{
							player_data["data"]["m_weapons"]["knife"] = weapon_name;
						}

						switch (weapon_type)
						{
							case cs2::e_weapon_type::submachine_gun:
							case cs2::e_weapon_type::rifle:
							case cs2::e_weapon_type::shotgun:
							case cs2::e_weapon_type::sniper_rifle:
							case cs2::e_weapon_type::machine_gun:
								player_data["data"]["m_weapons"]["primary"] = weapon_name;
								break;

							case cs2::e_weapon_type::pistol:
								player_data["data"]["m_weapons"]["secondary"] = weapon_name;
								break;

							case cs2::e_weapon_type::c4:
							case cs2::e_weapon_type::taser:
							case cs2::e_weapon_type::grenade:
								player_data["data"]["m_weapons"]["utility"].push_back(weapon_name);
								break;
						}

						// LOG_INFO("weapon_name -> %s (%d) %s", weapon_name.data(), weapon_type, ((idx == my_weapons.first - 1) ? "\n" : ""));
					}
				}();

				[&]()
				{
					const auto action_tracking_service = entity->get_action_tracking_services();
					if (!action_tracking_service)
						return;

					const auto kills = action_tracking_service->get_kills();
					const auto deaths = action_tracking_service->get_deaths();
					const auto assists = action_tracking_service->get_assists();

					player_data["data"]["m_stats"]["kills"] = kills;
					player_data["data"]["m_stats"]["deaths"] = deaths;
					player_data["data"]["m_stats"]["assists"] = assists;

					// LOG_INFO("kills: %d, deaths: %d, assists: %d", kills, deaths, assists);
				}();

				if (!player_data.empty())
				{
					data["players"].push_back(player_data);
				}

				 // LOG_INFO("name -> %s | color: %d, position: (%f, %f, %f), eye_angle: %f, team: %d, is_dead: %d", name.data(), color, position.x, position.y, position.z, eye_angles.y, team, is_dead);
			}

			// LOG_INFO("%s", data.dump().c_str());
			web_socket->send(data.dump());
		}

		web_socket->poll(16);
	}

	return true;
}