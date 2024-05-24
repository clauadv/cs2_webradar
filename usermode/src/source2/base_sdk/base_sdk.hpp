#pragma once

namespace src::source2
{
	class c_base_player;
	class c_planted_c4;
}

namespace src::source2
{
	class c_base_sdk
	{
	public:
		static bool setup()
		{
			if (!m_memory.attach())
			{
				LOG_ERROR("failed to attach to %s", GAME_NAME);
				return false;
			}

			if (!m_memory.get_id())
			{
				LOG_ERROR("failed to get a process id for %s", GAME_NAME);
				return false;
			}

			m_base_player = m_memory.find_pattern(CLIENT_DLL, GET_LOCAL_PLAYER_PAWN)->rip().add(0x148).as<source2::c_base_player*>();
			if (!m_base_player)
			{
				LOG_ERROR("failed to get an address for m_base_player");
				return false;
			}

			m_entity_list = m_memory.find_pattern(CLIENT_DLL, GET_ENTITY_LIST)->rip().as<source2::c_entity_list*>();
			if (!m_entity_list)
			{
				LOG_ERROR("failed to get an address for m_entity_list");
				return false;
			}

			m_global_vars = m_memory.find_pattern(CLIENT_DLL, GET_GLOBAL_VARS)->rip().as<source2::c_global_vars*>();
			if (!m_global_vars)
			{
				LOG_ERROR("failed to get an address for m_global_vars");
				return false;
			}

			m_planted_c4 = m_memory.find_pattern(CLIENT_DLL, GET_PLANTED_C4)->rip().as<source2::c_planted_c4*>();
			if (!m_planted_c4)
			{
				LOG_ERROR("failed to get an address for m_planted_c4");
				return false;
			}

			if (!source2::c_schema_system::setup())
			{
				LOG_ERROR("failed to setup schema system");
				return false;
			}

			LOG("m_process_id -> %d", m_memory.get_id());
			LOG("m_base_player -> 0x%llx", m_base_player);
			LOG("m_entity_list -> 0x%llx", m_entity_list);
			LOG("m_global_vars -> 0x%llx", m_global_vars);
			LOG("m_planted_c4 -> 0x%llx \n", m_planted_c4);

			return true;
		}
	};
}