#pragma once

namespace cs2
{
	class c_entity_list
	{
	public:
		template<typename T>
		T get(const int idx)
		{
			const auto entry_list = m_driver.read_t<std::uint64_t>(this + 8 * (idx >> 9) + 16);
			if (!entry_list)
				return nullptr;

			const auto controller = entry_list + 120 * (idx & 0x1ff);
			if (!controller)
				return nullptr;

			return reinterpret_cast<T>(m_driver.read_t<std::uint64_t>(controller));
		}
	};
}