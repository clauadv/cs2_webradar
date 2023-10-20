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
			const auto designer_name = m_memory.read_t<std::uintptr_t>(m_memory.read_t<std::uintptr_t>(this + m_offsets.get_entity()) + m_offsets.get_designer_name());
			if (!designer_name)
				return "invalid";

			const auto name = m_memory.read_t<std::string>(designer_name);
			if (name.empty())
				return "invalid";

			return name;
		}

		std::uintptr_t get_pawn()
		{
			return m_memory.read_t<std::uintptr_t>(this + m_offsets.get_player_pawn());
		}

		cs2::e_color get_color()
		{
			return m_memory.read_t<cs2::e_color>(this + m_offsets.get_comp_teammate_color());
		}

		int get_money()
		{
			const auto in_game_money_services = m_memory.read_t<std::uintptr_t>(this + m_offsets.get_in_game_money_services());
			if (!in_game_money_services)
				return false;

			return m_memory.read_t<int>(in_game_money_services + m_offsets.get_account());
		}

		std::string get_steam_id()
		{
			return std::to_string(m_memory.read_t<std::uintptr_t>(this + m_offsets.get_steam_id()));
		}

		math::fvector3 get_vec_origin()
		{
			const auto game_scene_node = m_memory.read_t<std::uintptr_t>(this + m_offsets.get_game_scene_node());
			if (!game_scene_node)
				return math::fvector3{};

			return m_memory.read_t<math::fvector3>(game_scene_node + m_offsets.get_vec_origin());
		}
	};
}