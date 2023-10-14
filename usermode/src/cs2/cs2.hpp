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
			m_driver.set_process("cs2.exe");
			if (!m_driver.get_process_id())
			{
				LOG_ERROR("failed to get an address for cs2.exe");
				return;
			}

			this->m_client_dll = m_driver.get_base("client.dll");
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
			LOG_INFO("driver::m_process -> %d", m_driver.get_process_id());
			LOG_INFO("this->m_client_dll -> 0x%llx \n", this->m_client_dll);
		#endif
		}

		std::uint64_t get_client() { return this->m_client_dll; }

		cs2::c_base_player* get_local_player()
		{
			return m_driver.read_t<cs2::c_base_player*>(this->m_client_dll + m_offsets.get_local_player_pawn());
		}

		cs2::c_global_vars* get_global_vars()
		{
			return m_driver.read_t<cs2::c_global_vars*>(this->m_client_dll + m_offsets.get_global_vars());
		}

		cs2::c_entity_list* get_entity_list()
		{
			return m_driver.read_t<cs2::c_entity_list*>(this->m_client_dll + m_offsets.get_entity_list());
		}
	};
}
inline usermode::c_cs2 m_cs2{};