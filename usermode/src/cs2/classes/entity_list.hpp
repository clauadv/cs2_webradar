#pragma once

namespace src::cs2
{
	class c_entity_list
	{
	public:
		template<typename T>
		T get(const int idx)
		{
			const auto entry_list = m_memory.read_t<uintptr_t>(this + 8 * (idx >> 9) + 16);
			if (!entry_list)
				return nullptr;

			const auto controller = entry_list + 120 * (idx & 0x1ff);
			if (!controller)
				return nullptr;

			return m_memory.read_t<T>(controller);
		}
	};
}