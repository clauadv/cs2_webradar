#include "pch.hpp"

bool sdk::setup()
{
	c_schema_system::m_schema_system = m_memory->find_pattern(SCHEMASYSTEM_DLL, GET_SCHEMA_SYSTEM)->rip().as<c_schema_system*>();
	if (!c_schema_system::m_schema_system)
	{
		LOG_ERROR("failed to get an address for c_schema_system::m_schema_system");
		return {};
	}

	c_game_entity_system::m_entity_list = m_memory->read_t<c_game_entity_system*>(m_memory->find_pattern(CLIENT_DLL, GET_ENTITY_LIST)->rip().as<c_game_entity_system*>());
	if (!c_game_entity_system::m_entity_list)
	{
		LOG_ERROR("failed to get an address for c_game_entity_system::m_entity_list");
		return {};
	}

	LOG("c_schema_system::m_schema_system -> 0x%llx", c_schema_system::m_schema_system);
	LOG("c_game_entity_system::m_entity_list -> 0x%llx \n", c_game_entity_system::m_entity_list);

	return true;
}

void sdk::update()
{
	c_cs_player_pawn::m_local_pawn = m_memory->read_t<c_cs_player_pawn*>(m_memory->find_pattern(CLIENT_DLL, GET_LOCAL_PLAYER_PAWN)->rip().add(0x148).as<c_cs_player_pawn*>());
	c_global_vars::m_global_vars = m_memory->read_t<c_global_vars*>(m_memory->find_pattern(CLIENT_DLL, GET_GLOBAL_VARS)->rip().as<c_global_vars*>());
}