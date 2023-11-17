#pragma once

namespace src::cs2
{
	class c_base_player_weapon : public c_base_entity
	{
	public:
		c_base_player_weapon* get(const int idx)
		{
			const auto entity_list = m_cs2.get_entity_list();
			if (!entity_list)
				return nullptr;

			const auto handle = m_memory.read_t<int>(this + idx * 0x4);
			if (handle == -1)
				return nullptr;

			return entity_list->get<c_base_player_weapon*>(handle & 0x7fff);
		}

		c_weapon_base_data* get_data()
		{
			// @note: https://www.unknowncheats.me/forum/3878143-post6.html
			return m_memory.read_t<c_weapon_base_data*>(this + m_offsets.get_subclass_id() + 0x08);
		}
	};
}