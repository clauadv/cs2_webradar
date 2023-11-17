#pragma once

namespace src
{
	class c_features
	{
	private:
		nlohmann::json m_data{};
		nlohmann::json m_player_data{};
		size_t m_bomb_idx{ 0 };

	public:
		nlohmann::json get_data() { return this->m_data; }

		void run()
		{
			const auto local_player = cs2::c_base_player::get();
			if (!local_player)
				return;

			const auto local_team = local_player->get_team();
			if (local_team == cs2::e_team::none || local_team == cs2::e_team::spectator)
				return;

			this->m_data = nlohmann::json{};
			this->m_player_data = nlohmann::json{};

			this->m_data["m_local_team"] = local_team;

			this->get_map();
			this->get_player_info();
			this->get_bomb_info();
		}

		void get_map()
		{
			const auto global_vars = m_global_vars->get();
			if (!global_vars)
				return;

			this->m_data["m_map"] = global_vars->get_map_name();
		}

		void get_player_info()
		{
			this->m_data["m_players"] = nlohmann::json::array();

			cs2::c_base_player::iterate([&](cs2::c_base_player* player, cs2::c_base_entity* entity, size_t idx)
			{
				this->m_player_data["m_idx"] = idx;
				this->m_player_data["m_name"] = reinterpret_cast<cs2::c_base_player*>(entity)->get_name();
				this->m_player_data["m_color"] = entity->get_color();
				this->m_player_data["m_team"] = player->get_team();
				this->m_player_data["m_health"] = player->get_health();
				this->m_player_data["m_armor"] = player->get_armor();
				this->m_player_data["m_position"]["x"] = player->get_position().x;
				this->m_player_data["m_position"]["y"] = player->get_position().y;
				this->m_player_data["m_eye_angle"] = player->get_eye_angles().normalize().y;
				this->m_player_data["m_is_dead"] = player->is_dead();
				this->m_player_data["m_model_name"] = player->get_model_name();
				this->m_player_data["m_steam_id"] = entity->get_steam_id();
				this->m_player_data["m_money"] = entity->get_money();
				this->m_player_data["m_has_helmet"] = player->has_helmet();
				this->m_player_data["m_has_defuser"] = player->has_defuser();
				this->m_player_data["m_weapons"] = nlohmann::json{};
				this->m_player_data["m_has_bomb"] = this->m_bomb_idx == (entity->get_pawn() & 0xffff);

				/* get active weapon */ [&]()
				{
					const auto weapon_services = player->get_weapon_services();
					if (!weapon_services)
						return;

					const auto active_weapon = weapon_services->get_active_weapon();
					if (!active_weapon)
						return;

					const auto weapon_data = active_weapon->get_data();
					if (!weapon_data)
						return;

					const auto weapon_name = weapon_data->get_name();

					this->m_player_data["m_weapons"]["m_active"] = weapon_name;
				}();

				/* get all player weapons */ [&]()
				{
					const auto weapon_services = player->get_weapon_services();
					if (!weapon_services)
						return;

					const auto my_weapons = weapon_services->get_my_weapons();
					if (!my_weapons.first)
						return;

					set<string> utilities_set{};
					set<string> melee_set{};

					for (size_t idx{ 0 }; idx < my_weapons.first; idx++)
					{
						const auto weapon = my_weapons.second->get(idx);
						if (!weapon)
							continue;

						const auto weapon_data = weapon->get_data();
						if (!weapon_data)
							continue;

						const auto weapon_name = weapon_data->get_name();

						const auto weapon_type = weapon_data->get_id();
						switch (weapon_type)
						{
							case cs2::e_weapon_type::submachine_gun:
							case cs2::e_weapon_type::rifle:
							case cs2::e_weapon_type::shotgun:
							case cs2::e_weapon_type::sniper_rifle:
							case cs2::e_weapon_type::machine_gun:
								this->m_player_data["m_weapons"]["m_primary"] = weapon_name;
								break;

							case cs2::e_weapon_type::pistol:
								this->m_player_data["m_weapons"]["m_secondary"] = weapon_name;
								break;

							case cs2::e_weapon_type::knife:
								melee_set.insert(weapon_name);
								break;

							case cs2::e_weapon_type::grenade:
								utilities_set.insert(weapon_name);
								break;
						}
					}

					this->m_player_data["m_weapons"]["m_melee"] = vector<string>(melee_set.begin(), melee_set.end());
					this->m_player_data["m_weapons"]["m_utilities"] = vector<string>(utilities_set.begin(), utilities_set.end());
				}();

				this->m_data["m_players"].push_back(this->m_player_data);
			});
		}

		void get_bomb_info()
		{
			this->m_data["m_bomb"] = nlohmann::json{};
			
			/* get dropped/carried bomb */ [&]()
			{
				cs2::c_base_entity::iterate("weapon_c4", [&](cs2::c_base_entity* entity)
				{
					this->m_bomb_idx = reinterpret_cast<uintptr_t>(entity->get_owner()) & 0xffff;

					const auto vec_origin = entity->get_vec_origin();
					if (vec_origin.zero())
						return;

					this->m_data["m_bomb"]["x"] = entity->get_vec_origin().x;
					this->m_data["m_bomb"]["y"] = entity->get_vec_origin().y;
				});
			}();

			/* get planted bomb */ [&]()
			{
				const auto planted_c4 = m_planted_c4->get();
				if (!planted_c4)
					return;

				const auto state = planted_c4->get_state();
				if (!state)
					return;

				const auto blow_time = planted_c4->get_blow_time();
				if (blow_time <= 0.f)
					return;

				const auto vec_origin = planted_c4->get_vec_origin();
				if (vec_origin.zero())
					return;

				const auto is_defused = planted_c4->get_defused();
				const auto is_defusing = planted_c4->get_being_defused();
				const auto defuse_time = planted_c4->get_defuse_time();

				this->m_data["m_bomb"]["x"] = vec_origin.x;
				this->m_data["m_bomb"]["y"] = vec_origin.y;
				this->m_data["m_bomb"]["m_blow_time"] = blow_time;
				this->m_data["m_bomb"]["m_is_defused"] = is_defused;
				this->m_data["m_bomb"]["m_is_defusing"] = is_defusing;
				this->m_data["m_bomb"]["m_defuse_time"] = defuse_time;
			}();
		}
	};
}
inline src::c_features m_features{};