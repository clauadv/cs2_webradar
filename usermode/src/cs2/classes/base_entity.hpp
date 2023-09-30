#pragma once

constexpr std::ptrdiff_t m_hPlayerPawn = 0x7FC;

namespace usermode::classes
{
	class c_base_entity
	{
	public:
		std::uint64_t get_pawn()
		{
			return driver::m_process.read_t<std::uint64_t>(this + m_hPlayerPawn);
		}
	};
}