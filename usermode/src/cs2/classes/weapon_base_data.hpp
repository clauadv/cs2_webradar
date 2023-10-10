#pragma once

namespace cs2
{
	enum class e_weapon_type : int
	{
		knife,
		pistol,
		submachine_gun,
		rifle,
		shotgun,
		sniper_rifle,
		machine_gun,
		c4,
		taser,
		grenade,
		equipment = 10
	};

	class c_weapon_base_data
	{
	public:
		cs2::e_weapon_type get_id()
		{
			return m_driver.read_t<cs2::e_weapon_type>(this + m_offsets.get_weapon_type());
		}
	};
}