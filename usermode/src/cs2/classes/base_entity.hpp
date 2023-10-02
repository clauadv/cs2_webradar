#pragma once

namespace usermode::classes
{
	class c_base_entity
	{
	public:
		std::uint64_t get_pawn()
		{
			return m_driver.read_t<std::uint64_t>(this + m_offsets.get_h_player_pawn());
		}

		std::string get_name()
		{
			const auto sanitized_player_name = m_driver.read_t<std::uint64_t>(this + m_offsets.get_sanitized_player_name());
			if (!sanitized_player_name)
				return "invalid";

			const auto player_name = m_driver.read_string(sanitized_player_name, 64);
			if (player_name.empty())
				return "invalid";

			return player_name;
		}

		bool has_defuser()
		{
			return m_driver.read_t<bool>(this + m_offsets.get_has_defuser());
		}

		bool has_helmet()
		{
			return m_driver.read_t<bool>(this + m_offsets.get_has_helmet());
		}
	};
}