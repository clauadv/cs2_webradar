#include "pch.hpp"

bool i::setup()
{
	bool success = true;

	const auto [client_base, client_size] = m_memory->get_module_info(CLIENT_DLL);
	if (!client_base.has_value() || !client_size.has_value())
		return {};

	m_schema_system = m_memory->find_pattern(SCHEMASYSTEM_DLL, GET_SCHEMA_SYSTEM)->rip().as<c_schema_system*>();
	success &= (m_schema_system != nullptr);

	m_global_vars = m_memory->read_t<c_global_vars*>(m_memory->find_pattern(CLIENT_DLL, GET_GLOBAL_VARS)->rip().as<c_global_vars*>());
	success &= (m_global_vars != nullptr);

	m_game_entity_system = m_memory->read_t<c_game_entity_system*>(m_memory->find_pattern(CLIENT_DLL, GET_ENTITY_LIST)->rip().as<c_game_entity_system*>());
	success &= (m_game_entity_system != nullptr);

	return success;
}