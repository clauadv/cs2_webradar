// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.hpp"

bool main()
{
	if (!usermode::m_driver.is_initialized())
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
		if (duration > std::chrono::milliseconds(200))
		{
			start = now;
			const auto local_player = usermode::m_cs2.get_local_player();
			if (!local_player)
				continue;

			const auto local_team = local_player->get_team();
			if (local_team == usermode::classes::e_team::none ||
				local_team == usermode::classes::e_team::spectator)
				continue;

			const auto global_vars = usermode::m_cs2.get_global_vars();
			if (!global_vars)
				continue;

			nlohmann::json data{};
			data["map"] = global_vars->get_map_name();

			nlohmann::json local_player_data{};
			local_player_data["team"] = local_team;
			data["local_player"].push_back(local_player_data);

			const auto entity_list = usermode::m_cs2.get_entity_list();
			if (!entity_list)
				continue;

			for (std::size_t idx{ 0 }; idx < 32; idx++)
			{
				const auto entity = entity_list->get_entity(idx);
				if (!entity)
					continue;

				const auto entity_pawn = entity->get_pawn();
				if (!entity_pawn)
					continue;

				const auto player = entity_list->get_player(entity_pawn);
				if (!player)
					continue;

				/* if (player == local_player)
					continue; */

				const auto name = entity->get_name();
				const auto has_defuser = entity->has_defuser();
				const auto has_helmet = entity->has_helmet();

				const auto team = player->get_team();
				const auto position = player->get_position();

				nlohmann::json player_data{};
				player_data["index"] = idx;
				player_data["name"] = "tactu";
				player_data["data"]["position"]["x"] = position.x;
				player_data["data"]["position"]["y"] = position.y;
				player_data["data"]["team"] = team;
				player_data["data"]["dead"] = player->is_dead();

				data["players"].push_back(player_data);

				LOG_INFO("name -> %s | team: %d, position: (x: %f, y: %f), has_defuser: %d, has_helmet: %d", name, team, position.x, position.y, has_defuser, has_helmet);
			}

			web_socket->send(data.dump());
		}

		web_socket->poll(16);
	}

	return true;
}