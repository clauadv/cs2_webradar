#pragma once

namespace cs2
{
	class c_action_tracking_services
	{
	public:
		int get_kills()
		{
			return m_driver.read_t<int>(this + m_offsets.get_math_stats() + m_offsets.get_kills());
		}

		int get_deaths()
		{
			return m_driver.read_t<int>(this + m_offsets.get_math_stats() + m_offsets.get_deaths());
		}

		int get_assists()
		{
			return m_driver.read_t<int>(this + m_offsets.get_math_stats() + m_offsets.get_assists());
		}
	};
}