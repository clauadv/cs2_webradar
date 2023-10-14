#pragma once

namespace cs2
{
	class c_item_services
	{
	public:
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