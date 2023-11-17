#include "pch.hpp"

src::cs2::c_entity_list* src::cs2::c_entity_list::get()
{
	return m_memory.read_t<c_entity_list*>(m_entity_list);
}