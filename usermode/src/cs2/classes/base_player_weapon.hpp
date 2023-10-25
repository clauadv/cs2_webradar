#pragma once

namespace cs2
{
	class c_base_player_weapon : public cs2::c_base_entity
	{
	public:
		cs2::c_base_player_weapon* get(const int idx)
		{
			const auto entity_list = m_cs2.get_entity_list();
			if (!entity_list)
				return nullptr;

			const auto handle = m_memory.read_t<int>(this + idx * 0x4);
			if (handle == -1)
				return nullptr;

			return entity_list->get<cs2::c_base_player_weapon*>(handle & 0x7fff);
		}

		cs2::c_weapon_base_data* get_data()
		{
			// @NOTE: https://www.unknowncheats.me/forum/3878143-post6.html
			return m_memory.read_t<cs2::c_weapon_base_data*>(this + m_offsets.get_subclass_id() + 0x08);
		}
	};
}