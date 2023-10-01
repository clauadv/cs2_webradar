#pragma once

namespace usermode::classes
{
	enum class e_team : int
	{
		none,
		spectator,
		terrorist,
		counter_terrorist
	};

	class c_base_player
	{
	public:
		int get_health()
		{
			return m_driver.read_t<int>(this + m_offsets.get_health());
		}

		classes::e_team get_team()
		{
			return static_cast<classes::e_team>(m_driver.read_t<int>(this + m_offsets.get_team_num()));
		}

		math::fvector3 get_position()
		{
			return m_driver.read_t<math::fvector3>(this + m_offsets.get_old_origin());
		}

		bool is_dead()
		{
			return this->get_health() <= 0;
		}
	};
}