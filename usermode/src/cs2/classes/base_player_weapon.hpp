#pragma once

namespace usermode::classes
{
	enum class e_item_definition_index : int
	{
		// @TODO
	};

	class c_base_player_weapon
	{
	public:
		usermode::classes::c_base_player_weapon* get(usermode::classes::c_entity_list* entity_list, const int idx)
		{
			const auto handle = usermode::m_driver.read_t<int32_t>(this + idx * 0x4);
			if (handle == -1)
				return nullptr;

			return entity_list->get<usermode::classes::c_base_player_weapon*>(handle & 0x7fff);
		}

		usermode::classes::e_item_definition_index get_item_definition_index()
		{
			return usermode::m_driver.read_t<usermode::classes::e_item_definition_index>(this + m_offsets.get_attribute_manager() + m_offsets.get_item() + m_offsets.get_item_definition_index());
		}
	};
}