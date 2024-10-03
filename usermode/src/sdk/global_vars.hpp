#pragma once

class c_global_vars
{
public:
	static inline c_global_vars* m_global_vars = nullptr;

	SCHEMA_ADD_OFFSET(float, m_curtime, 0x34);
	SCHEMA_ADD_STRING_OFFSET(m_map_name, 0x180);
};