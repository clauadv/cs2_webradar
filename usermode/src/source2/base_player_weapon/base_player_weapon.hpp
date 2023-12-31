#pragma once

namespace src::source2
{
	class c_base_player_weapon : public c_base_entity
	{
	public:
		c_base_player_weapon* get(const int idx)
		{
			const auto entity_list = c_entity_list::get();
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
			return m_memory.read_t<c_weapon_base_data*>(this + GET_SCHEMA("c_baseentity->m_nsubclassid") + 0x08);
		}
	};
}