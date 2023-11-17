#include "pch.hpp"

src::source2::c_planted_c4* src::source2::c_planted_c4::get()
{
	return m_memory.read_t<c_planted_c4*>(m_memory.read_t<uintptr_t>(m_planted_c4));
}

bool src::source2::c_planted_c4::get_state()
{
	return m_memory.read_t<bool>(m_planted_c4 - 0x8);
}