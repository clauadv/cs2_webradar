#pragma once

namespace cs2
{
	class c_in_game_money_services
	{
	public:
		int get_money()
		{
			return m_driver.read_t<int>(this + 0x40);
		}
	};
}