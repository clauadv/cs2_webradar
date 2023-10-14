#pragma once

namespace cs2
{
	enum class e_color : int
	{
		blue,
		green,
		yellow,
		orange,
		purple
	};

	class c_base_entity
	{
	public:
		std::string get_name()
		{
			const auto designer_name = m_driver.read_t<std::uint64_t>(m_driver.read_t<std::uint64_t>(this + m_offsets.get_entity()) + m_offsets.get_designer_name());
			if (!designer_name)
				return "invalid";

			const auto name = m_driver.read_string(designer_name, 32);
			if (name.empty())
				return "invalid";

			return name;
		}

		cs2::c_action_tracking_services* get_action_tracking_services()
		{
			return m_driver.read_t<cs2::c_action_tracking_services*>(this + m_offsets.get_action_tracking_services());
		}

		std::uint64_t get_pawn()
		{
			return m_driver.read_t<std::uint64_t>(this + m_offsets.get_player_pawn());
		}

		cs2::e_color get_color()
		{
			return m_driver.read_t<cs2::e_color>(this + m_offsets.get_comp_teammate_color());
		}

		cs2::c_in_game_money_services* get_in_game_money_services()
		{
			return m_driver.read_t<cs2::c_in_game_money_services*>(this + m_offsets.get_in_game_money_services());
		}

		cs2::c_item_services* get_item_services()
		{
			return m_driver.read_t<cs2::c_item_services*>(this + m_offsets.get_item_services());
		}
	};
}