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
		std::string get_name()
		{
			const auto sanitized_player_name = m_driver.read_t<std::uint64_t>(this + m_offsets.get_sanitized_player_name());
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

		std::string get_model_path()
		{
			const auto model_name_ptr = m_driver.read_t<std::uint64_t>(m_driver.read_t<std::uint64_t>(this + m_offsets.get_game_scene_node()) + m_offsets.get_model_state() + m_offsets.get_model_name());
			if (!model_name_ptr)
				return "invalid";

			const auto model_name = m_driver.read_t<std::string>(model_name_ptr);
			if (model_name.empty())
				return "invalid";

			return model_name;
		}
	};
}