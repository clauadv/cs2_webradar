#pragma once

namespace cs2
{
	enum class e_item_definition_index : int
	{
		// @TODO
	};

	class c_base_player_weapon
	{
	public:
		cs2::c_base_player_weapon* get(cs2::c_entity_list* entity_list, const int idx)
		{
			const auto handle = m_driver.read_t<int32_t>(this + idx * 0x4);
			if (handle == -1)
				return nullptr;

			return entity_list->get<cs2::c_base_player_weapon*>(handle & 0x7fff);
		}

		cs2::e_item_definition_index get_item_definition_index()
		{
			return m_driver.read_t<cs2::e_item_definition_index>(this + m_offsets.get_attribute_manager() + m_offsets.get_item() + m_offsets.get_item_definition_index());
		}
	};
}