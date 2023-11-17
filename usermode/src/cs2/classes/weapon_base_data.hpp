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
		e_weapon_type get_id()
		{
			return m_memory.read_t<e_weapon_type>(this + m_offsets.get_weapon_type());
		}

		string get_name()
		{
			const auto name_ptr = m_memory.read_t<uint64_t>(this + m_offsets.get_name());
			if (!name_ptr)
				return "invalid";

			auto name = m_memory.read_t<string>(name_ptr);
			if (name.empty())
				return "invalid";

			// @note: remove "weapon_" from the string
			name.erase(name.begin(), name.begin() + 7);

			return name;
		}
	};
}