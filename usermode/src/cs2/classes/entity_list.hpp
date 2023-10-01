#pragma once

namespace usermode::classes
{
	class c_entity_list
	{
	public:
		classes::c_base_entity* get_entity(const int idx)
		{
			const auto entry_list = driver::m_process.read_t<std::uint64_t>(this + (0x8 * (idx & 0x7fff) >> 9) + 16);
			if (!entry_list)
				return nullptr;

			return reinterpret_cast<classes::c_base_entity*>(driver::m_process.read_t<std::uint64_t>(entry_list + 120 * (idx & 0x1ff)));
		}

		classes::c_base_player* get_player(const std::uint64_t entity_pawn)
		{
			const auto entry_list = driver::m_process.read_t<std::uint64_t>(this + 0x8 * ((entity_pawn & 0x7fff) >> 9) + 16);
			if (!entry_list)
				return nullptr;

			return reinterpret_cast<classes::c_base_player*>(driver::m_process.read_t<std::uint64_t>(entry_list + 120 * (entity_pawn & 0x1ff)));
		}
	};
}