#pragma once

namespace cs2
{
	class c_base_player_weapon
	{
	public:
		cs2::c_base_player_weapon* get(cs2::c_entity_list* entity_list, const int idx)
		{
			const auto handle = m_driver.read_t<int>(this + idx * 0x4);
			if (handle == -1)
				return nullptr;

			return entity_list->get<cs2::c_base_player_weapon*>(handle & 0x7fff);
		}

		std::string get_weapon_name()
		{
			const auto entity = m_driver.read_t<std::uint64_t>(this + m_offsets.get_entity());
			if (!entity)
				return "invalid";

			const auto designer_name = m_driver.read_t<std::uint64_t>(entity + m_offsets.get_designer_name());
			if (!designer_name)
				return "invalid";

			const auto name = m_driver.read_string(designer_name, 32);
			if (name.empty())
				return "invalid";

			return name;
		}
	};
}