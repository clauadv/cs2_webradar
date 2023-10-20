#pragma once

namespace cs2
{
	class c_base_player;
	class c_planted_c4;
}

namespace usermode
{
	class c_cs2
	{
	private:
		std::uintptr_t m_client_dll{ 0 };
		std::uintptr_t m_engine2_dll{ 0 };

	public:
		c_cs2()
		{
			m_memory.set_process("cs2.exe");
			if (!m_memory.get_process_id())
			{
				LOG_ERROR("failed to get an address for cs2.exe");
				return;
			}

			this->m_client_dll = m_memory.get_base("client.dll");
			if (!this->m_client_dll)
			{
				LOG_ERROR("failed to get an address for client.dll");
				return;
			}

			this->m_engine2_dll = m_memory.get_base("engine2.dll");
			if (!this->m_engine2_dll)
			{
				LOG_ERROR("failed to get an address for engine2.dll");
				return;
			}

			if (!m_offsets.is_initialized())
			{
				LOG_ERROR("failed to initialize offsets");
				return;
			}
			
		#ifdef DEVELOPER
			LOG_INFO("m_memory::m_process_id -> %d", m_memory.get_process_id());
			LOG_INFO("this->m_client_dll -> 0x%llx", this->m_client_dll);
			LOG_INFO("this->m_engine2_dll -> 0x%llx \n", this->m_engine2_dll);
		#endif
		}

		std::uintptr_t get_client() { return this->m_client_dll;  }
		std::uintptr_t get_engine2() { return this->m_engine2_dll; }

		cs2::c_base_player* get_local_player()
		{
			return m_memory.read_t<cs2::c_base_player*>(this->m_client_dll + m_offsets.get_local_player_pawn());
		}

		cs2::c_global_vars* get_global_vars()
		{
			return m_memory.read_t<cs2::c_global_vars*>(this->m_client_dll + m_offsets.get_global_vars());
		}

		cs2::c_entity_list* get_entity_list()
		{
			return m_memory.read_t<cs2::c_entity_list*>(this->m_client_dll + m_offsets.get_entity_list());
		}

		std::string get_game_build()
		{
			const auto engine_build_info = m_memory.read_t<std::uintptr_t>(this->m_engine2_dll + m_offsets.get_game_build());
			if (!engine_build_info)
				return "invalid";

			return m_memory.read_t<std::string>(engine_build_info);
		}

		cs2::c_planted_c4* get_planted_c4()
		{
			return m_memory.read_t<cs2::c_planted_c4*>(m_memory.read_t<uintptr_t>(this->m_client_dll + m_offsets.get_planted_c4()));
		}
	};
}
inline usermode::c_cs2 m_cs2{};