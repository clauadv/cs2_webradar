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

			auto player_name = m_driver.read_string(sanitized_player_name, 32);
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
			return static_cast<cs2::e_team>(m_driver.read_t<int>(this + m_offsets.get_team_num()));
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
			return reinterpret_cast<cs2::c_player_weapon_services*>(m_driver.read_t<std::uint64_t>(this + m_offsets.get_weapon_services()));
		}
	};
}