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
		std::uint64_t get_pawn()
		{
			return m_driver.read_t<std::uint64_t>(this + m_offsets.get_h_player_pawn());
		}

		std::string get_name()
		{
			const auto sanitized_player_name = m_driver.read_t<std::uint64_t>(this + m_offsets.get_sanitized_player_name());
			if (!sanitized_player_name)
				return "invalid";

			auto player_name = m_driver.read_string(sanitized_player_name, 64);
			if (player_name.empty())
				return "invalid";

			auto is_ascii = [](char c)
			{
				return (c >= 0 && c <= 127);
			};

			player_name.erase(std::remove_if(player_name.begin(), player_name.end(), [&is_ascii](char c)
			{
				return !is_ascii(c); 
			}), player_name.end());

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

		cs2::e_color get_color()
		{
			return m_driver.read_t<cs2::e_color>(this + m_offsets.get_comp_teammate_color());
		}
	};
}