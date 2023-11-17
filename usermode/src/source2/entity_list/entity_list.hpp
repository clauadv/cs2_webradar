#pragma once

namespace src::source2
{
	class c_entity_list
	{
	public:
		static c_entity_list* get();

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
inline src::source2::c_entity_list* m_entity_list{};