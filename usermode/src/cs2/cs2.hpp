#pragma once

namespace src::cs2
{
	class c_base_player;
	class c_planted_c4;
}

namespace src
{
	class c_cs2
	{
	public:
		bool setup()
		{
			if (!m_memory.attach("cs2.exe"))
			{
				LOG_ERROR("failed to attach to cs2.exe");
				return false;
			}

			if (!m_memory.get_id())
			{
				LOG_ERROR("failed to get an address for cs2.exe");
				return false;
			}

			if (utils::get_version() != GAME_VERSION)
			{
				LOG_ERROR("game has been updated, update the offsets");
				return false;
			}

			this->m_client_dll = m_memory.get_module_base("client.dll");
			if (!this->m_client_dll)
			{
				LOG_ERROR("failed to get an address for client.dll");
				return false;
			}

			this->m_engine2_dll = m_memory.get_module_base("engine2.dll");
			if (!this->m_engine2_dll)
			{
				LOG_ERROR("failed to get an address for engine2.dll");
				return false;
			}

			if (!cs2::c_schema_system::setup())
			{
				LOG_ERROR("failed to setup schema system");
				return false;
			}

			if (!m_offsets.setup())
			{
				LOG_ERROR("failed to setup offsets");
				// return false;
			}

			LOG("m_process_id -> %d", m_memory.get_id());
			LOG("m_client_dll -> 0x%llx", this->get_client());
			LOG("m_engine2_dll -> 0x%llx \n", this->get_engine2());

			LOG("m_player_pawn -> 0x%llx", GET_SCHEMA("ccsplayercontroller->m_hplayerpawn"));

			return true;
		}

		uintptr_t get_client()
		{
			return this->m_client_dll.value();
		}

		uintptr_t get_engine2()
		{
			return this->m_engine2_dll.value();
		}

		cs2::c_base_player* get_local_player()
		{
			return m_memory.read_t<cs2::c_base_player*>(this->get_client() + m_offsets.get_local_player_pawn());
		}

		cs2::c_global_vars* get_global_vars()
		{
			return m_memory.read_t<cs2::c_global_vars*>(this->get_client() + m_offsets.get_global_vars());
		}

		cs2::c_entity_list* get_entity_list()
		{
			return m_memory.read_t<cs2::c_entity_list*>(this->get_client() + m_offsets.get_entity_list());
		}

		cs2::c_planted_c4* get_planted_c4()
		{
			return m_memory.read_t<cs2::c_planted_c4*>(m_memory.read_t<uintptr_t>(this->get_client() + m_offsets.get_planted_c4()));
		}

	private:
		optional<uintptr_t> m_client_dll{ 0 };
		optional<uintptr_t> m_engine2_dll{ 0 };
	};
}
inline src::c_cs2 m_cs2{};