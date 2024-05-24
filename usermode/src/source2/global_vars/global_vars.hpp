#pragma once

namespace src::source2
{
	class c_global_vars
	{
	public:
		static c_global_vars* get();

		string get_map_name()
		{
			const auto current_map_name = m_memory.read_t<uintptr_t>(this + 0x1b8);
			if (!current_map_name)
				return "invalid";

			const auto map_name = m_memory.read_t<string>(current_map_name);
			if (map_name.empty() || map_name.find("<empty>") != string::npos)
				return "invalid";

			return map_name;
		}

		float get_curtime()
		{
			return m_memory.read_t<float>(this + 0x34);
		}
	};
}
inline src::source2::c_global_vars* m_global_vars{};