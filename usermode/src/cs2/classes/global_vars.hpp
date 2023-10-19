#pragma once

namespace cs2
{
	class c_global_vars
	{
	public:
		std::string get_map_name()
		{
			// @NOTE: https://www.unknowncheats.me/forum/3870682-post1183.html
			const auto current_map_name = m_driver.read_t<std::uintptr_t>(this + 0x188);
			if (!current_map_name)
				return "invalid";

			const auto map_name = m_driver.read_t<std::string>(current_map_name);
			if (map_name.empty() || map_name.contains("<empty>"))
				return "invalid";

			return map_name;
		}

		float get_curtime()
		{
			return m_driver.read_t<float>(this + 0x2c);
		}
	};
}