#pragma once

namespace cs2
{
	class c_player_weapon_services
	{
	public:
		std::pair<int, cs2::c_base_player_weapon*> get_my_weapons()
		{
			// @NOTE: num weapons is at + 0x0, data is at + 0x8 (https://www.unknowncheats.me/forum/3711351-post132.html)
			return
			{
				m_driver.read_t<int>(this + m_offsets.get_h_my_weapons()),
				reinterpret_cast<cs2::c_base_player_weapon*>(m_driver.read_t<std::uint64_t>(this + m_offsets.get_h_my_weapons() + 0x08))
			};
		}
	};
}