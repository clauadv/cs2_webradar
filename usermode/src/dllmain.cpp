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
			data["m_map"] = global_vars->get_map_name();

			nlohmann::json local_player_data{};
			local_player_data["m_team"] = local_team;
			data["local_player"].push_back(local_player_data);

			const auto entity_list = m_cs2.get_entity_list();
			if (!entity_list)
				continue;

			for (std::size_t idx{ 0 }; idx < 32; idx++)
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

				const auto weapon_services = player->get_weapon_services();
				if (!weapon_services)
					continue;

				const auto my_weapons = weapon_services->get_my_weapons();
				if (!my_weapons.first)
					continue;

				const auto name = reinterpret_cast<cs2::c_base_player*>(entity)->get_name();
				const auto color = entity->get_color();
				const auto health = player->get_health();
				const auto armor = player->get_armor();
				const auto position = player->get_position();
				const auto eye_angles = player->get_eye_angles().normalize();
				const auto team = player->get_team();
				const auto is_dead = player->is_dead();

				// CSMatchStats_t m_matchStats; // 0x90	

				nlohmann::json player_data{};
				player_data["m_idx"] = idx;
				player_data["data"]["m_name"] = name;
				player_data["data"]["m_color"] = color;
				player_data["data"]["m_health"] = health;
				player_data["data"]["m_armor"] = armor;
				player_data["data"]["m_position"]["x"] = position.x;
				player_data["data"]["m_position"]["y"] = position.y;
				player_data["data"]["m_eye_angle"] = eye_angles.y;
				player_data["data"]["m_team"] = team;
				player_data["data"]["m_is_dead"] = is_dead;

				for (std::size_t idx{ 0 }; idx < my_weapons.first; idx++)
				{
					const auto weapon = my_weapons.second->get(entity_list, idx);
					if (!weapon)
						continue;

					const auto weapon_data = m_driver.read_t<std::uint64_t>(weapon + 0x360);
					if (!weapon_data)
						continue;

					const auto weapon_type = m_driver.read_t<int>(weapon_data + 0x240);
					const auto weapon_name = weapon->get_name();

					switch (weapon_type)
					{
						case 0:
							player_data["data"]["m_weapons"]["melee"] = weapon_name;
							break;
						case 1:
							player_data["data"]["m_weapons"]["secondary"] = weapon_name;
							break;
						case 2:
						case 3:
						case 4:
						case 5:
						case 6:
							player_data["data"]["m_weapons"]["primary"] = weapon_name;
							break;
						case 7:
							player_data["data"]["m_weapons"]["bomb"] = weapon_name;
							break;
						case 9:
							player_data["data"]["m_weapons"]["utility"] = weapon_name;
							break;
					}

					// LOG_INFO("weapon_name -> %s %s", weapon_name.data(), ((idx == my_weapons.first - 1) ? "\n" : ""));
				}

				[&]()
				{
					const auto action_tracking_service = m_driver.read_t<std::uint64_t>(entity + 0x6e0);
					if (!action_tracking_service)
						return;

					const auto match_stats = m_driver.read_t<std::uint64_t>(action_tracking_service + 0x90);
					if (!match_stats)
						return;

					const auto kills = m_driver.read_t<int>(action_tracking_service + 0x90 + 0x30);
					const auto deaths = m_driver.read_t<int>(action_tracking_service + 0x90 + 0x34);
					const auto assists = m_driver.read_t<int>(action_tracking_service + 0x90 + 0x38);

					player_data["data"]["m_stats"]["kills"] = kills;
					player_data["data"]["m_stats"]["deaths"] = deaths;
					player_data["data"]["m_stats"]["assists"] = assists;

					// CCSPlayerController_ActionTrackingServices* m_pActionTrackingServices; // 0x6e0
					// CSMatchStats_t m_matchStats; // 0x90	

					// sLOG_INFO("kills: %d, deaths: %d, assists: %d", kills, deaths, assists);
				}();

				data["players"].push_back(player_data);

				// LOG_INFO("name -> %s | color: %d, position: (%f, %f, %f), eye_angle: %f, team: %d, is_dead: %d", name.data(), color, position.x, position.y, position.z, eye_angles.y, team, is_dead);
			}

			web_socket->send(data.dump());
		}

		web_socket->poll(16);
	}

	return true;
}