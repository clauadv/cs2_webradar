#pragma once

namespace usermode
{
	class c_cs2
	{
	private:
		std::uint64_t m_client_dll{ 0 };

	public:
		c_cs2()
		{
			driver::m_process = driver::c_process("cs2.exe");
			if (!driver::m_process.is_valid())
			{
				LOG_ERROR("failed to get an address for cs2.exe");
				return;
			}

			this->m_client_dll = driver::m_process.get_base("client.dll");
			if (!this->m_client_dll)
			{
				LOG_ERROR("failed to get an address for client.dll");
				return;
			}

			if (!m_offsets.is_initialized())
			{
				LOG_ERROR("failed to initialize offsets");
				return;
			}
			
		#ifdef DEVELOPER
			LOG_INFO("driver::m_process -> %d", driver::m_process);
			LOG_INFO("c_cs2::m_client_dll -> 0x%llx \n", this->m_client_dll);
		#endif
		}

		std::uint64_t get_client()
		{
			return this->m_client_dll;
		}

		usermode::classes::c_base_player* get_local_player()
		{
			return reinterpret_cast<usermode::classes::c_base_player*>(driver::m_process.read_t<std::uint64_t>(this->m_client_dll + m_offsets.get_local_player_pawn()));
		}

		usermode::classes::c_global_vars* get_global_vars()
		{
			return reinterpret_cast<usermode::classes::c_global_vars*>(driver::m_process.read_t<std::uint64_t>(this->m_client_dll + m_offsets.get_global_vars()));
		}

		usermode::classes::c_entity_list* get_entity_list()
		{
			return reinterpret_cast<usermode::classes::c_entity_list*>(driver::m_process.read_t<std::uint64_t>(this->m_client_dll + m_offsets.get_entity_list()));
		}
	};
}