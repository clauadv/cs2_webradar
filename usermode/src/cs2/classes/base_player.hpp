#pragma once

constexpr std::ptrdiff_t m_iHealth = 0x32C;
constexpr std::ptrdiff_t m_iTeamNum = 0x3BF;
constexpr std::ptrdiff_t m_vecOrigin = 0x1214;

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
			return driver::m_process.read_t<int>(this + m_iHealth);
		}

		usermode::classes::e_team get_team()
		{
			return static_cast<usermode::classes::e_team>(driver::m_process.read_t<int>(this + m_iTeamNum));
		}

		usermode::math::fvector3 get_position()
		{
			return driver::m_process.read_t<usermode::math::fvector3>(this + m_vecOrigin);
		}
	};
}