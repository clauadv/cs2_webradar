#pragma once

namespace cs2
{
	enum class e_team : int
	{
		none,
		spectator,
		terrorist,
		counter_terrorist
	};

	class c_base_player : public cs2::c_base_entity
	{
	public:
		static void iterate(const std::function<void(cs2::c_base_player*, cs2::c_base_entity*, std::size_t)>& function)
		{
			const auto global_vars = m_cs2.get_global_vars();
			if (!global_vars)
				return;

			const auto entity_list = m_cs2.get_entity_list();
			if (!entity_list)
				return;

			const auto map_name = global_vars->get_map_name();

			for (std::size_t idx{ 0 }; idx < 64 && map_name.find("invalid") == std::string::npos; idx++)
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

				function(player, entity, idx);
			}
		}

		std::string get_name()
		{
			const auto sanitized_player_name = m_driver.read_t<std::uintptr_t>(this + m_offsets.get_sanitized_player_name());
			if (!sanitized_player_name)
				return "invalid";

			auto player_name = m_driver.read_t<std::string>(sanitized_player_name);
			if (player_name.empty())
				return "invalid";

			return player_name;
		}

		int get_health()
		{
			return m_driver.read_t<int>(this + m_offsets.get_health());
		}

		int get_armor()
		{
			return m_driver.read_t<int>(this + m_offsets.get_armor());
		}

		cs2::e_team get_team()
		{
			return m_driver.read_t<cs2::e_team>(this + m_offsets.get_team_num());
		}

		math::fvector3 get_position()
		{
			return m_driver.read_t<math::fvector3>(this + m_offsets.get_old_origin());
		}

		bool is_dead()
		{
			return this->get_health() <= 0;
		}

		math::fvector3 get_eye_angles()
		{
			return m_driver.read_t<math::fvector3>(this + m_offsets.get_ang_eye_angles());
		}

		cs2::c_player_weapon_services* get_weapon_services()
		{
			return m_driver.read_t<cs2::c_player_weapon_services*>(this + m_offsets.get_weapon_services());
		}

		std::string get_model_name()
		{
			const auto model_name_ptr = m_driver.read_t<std::uintptr_t>(m_driver.read_t<std::uintptr_t>(this + m_offsets.get_game_scene_node()) + m_offsets.get_model_state() + m_offsets.get_model_name());
			if (!model_name_ptr)
				return "invalid";

			const auto model_path = m_driver.read_t<std::string>(model_name_ptr);
			if (model_path.empty())
				return "invalid";

			return model_path.substr(model_path.rfind("/") + 1, model_path.rfind(".") - model_path.rfind("/") - 1);
		}
	};
}