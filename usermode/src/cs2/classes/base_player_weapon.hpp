#pragma once

namespace cs2
{
	class c_base_player_weapon : public cs2::c_base_entity
	{
	public:
		cs2::c_base_player_weapon* get(cs2::c_entity_list* entity_list, const int idx)
		{
			const auto handle = m_driver.read_t<int>(this + idx * 0x4);
			if (handle == -1)
				return nullptr;

			return entity_list->get<cs2::c_base_player_weapon*>(handle & 0x7fff);
		}
	};
}