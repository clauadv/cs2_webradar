#pragma once

namespace src::cs2
{
	class c_global_vars
	{
	public:
		string get_map_name()
		{
			// @note: https://www.unknowncheats.me/forum/3870682-post1183.
			const auto current_map_name = m_memory.read_t<uintptr_t>(this + 0x188);
			if (!current_map_name)
				return "invalid";

			const auto map_name = m_memory.read_t<string>(current_map_name);
			if (map_name.empty() || map_name.contains("<empty>"))
				return "invalid";

			return map_name;
		}

		float get_curtime()
		{
			// @note: https://www.unknowncheats.me/forum/3870682-post1183.
			return m_memory.read_t<float>(this + 0x2c);
		}
	};
}