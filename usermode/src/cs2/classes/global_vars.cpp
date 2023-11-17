#include "pch.hpp"

src::cs2::c_global_vars* src::cs2::c_global_vars::get()
{
	return m_memory.read_t<c_global_vars*>(m_global_vars);
}