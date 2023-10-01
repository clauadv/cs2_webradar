// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.hpp"

bool main()
{
	if (!driver::initialize())
	{
		LOG_ERROR("failed to initialize driver communication");
		return true;
	}

	usermode::c_cs2 cs2{};

	// @easywsclient
	WSADATA wsa_data{};
	const auto wsa_startup = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (wsa_startup != 0)
	{
		LOG_ERROR("failed to initialize winsock");
		return true;
	}

	static auto web_socket = easywsclient::WebSocket::from_url("ws://localhost:22006/foo");
	auto start = std::chrono::system_clock::now();

	for (;;)
	{
		const auto now = std::chrono::system_clock::now();
		const auto duration = now - start;
		if (duration > std::chrono::milliseconds(100))
		{
			start = now;

			const auto local_player = cs2.get_local_player();
			if (!local_player)
				continue;

			const auto global_vars = cs2.get_global_vars();
			if (!global_vars)
				continue;

			nlohmann::json data{};
			data["map"] = global_vars->get_map_name();

			const auto entity_list = cs2.get_entity_list();
			if (!entity_list)
				continue;

			const auto max_clients = global_vars->get_max_clients();
			for (std::size_t idx{ 0 }; idx < max_clients; idx++)
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

				if (player == local_player)
					continue;

				const auto health = player->get_health();
				const auto team = player->get_team();
				const auto position = player->get_position();

				nlohmann::json player_data{};
				player_data["index"] = idx;
				player_data["name"] = "tactu";
				player_data["data"]["position"]["x"] = position.x;
				player_data["data"]["position"]["y"] = position.y;
				player_data["data"]["team"] = team;

				data["players"].push_back(player_data);

				LOG_INFO("entity -> 0x%llx | health: %d, team: %d, position: (x: %f, y: %f)", entity, health, team, position.x, position.y);
			}

			web_socket->send(data.dump());
		}

		web_socket->poll();
	}

	return true;
}