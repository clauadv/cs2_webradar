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
			return driver::m_process.read_t<int>(this + m_offsets.get_health());
		}

		usermode::classes::e_team get_team()
		{
			return static_cast<usermode::classes::e_team>(driver::m_process.read_t<int>(this + m_offsets.get_team_num()));
		}

		usermode::math::fvector3 get_position()
		{
			return driver::m_process.read_t<usermode::math::fvector3>(this + m_offsets.get_old_origin());
		}
	};
}