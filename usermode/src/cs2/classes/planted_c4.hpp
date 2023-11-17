#pragma once

namespace cs2
{
	class c_planted_c4 : public c_base_entity
	{
	public:
		bool get_state()
		{
			return m_memory.read_t<bool>(m_cs2.get_client() + m_offsets.get_planted_c4() - 0x8);
		}

		float get_blow_time()
		{
			const auto global_vars = m_cs2.get_global_vars();
			if (!global_vars)
				return 0.f;

			const auto c4_blow = m_memory.read_t<float>(this + m_offsets.get_c4_blow());
			if (!c4_blow)
				return 0.f;

			return (c4_blow - global_vars->get_curtime());
		}

		bool get_defused()
		{
			return m_memory.read_t<bool>(this + m_offsets.get_bomb_defused());
		}

		bool get_being_defused()
		{
			return m_memory.read_t<bool>(this + m_offsets.get_being_defused());
		}

		float get_defuse_time()
		{
			const auto global_vars = m_cs2.get_global_vars();
			if (!global_vars)
				return 0.f;

			const auto defuse_countdown = m_memory.read_t<float>(this + m_offsets.get_defuse_countdown());
			if (!defuse_countdown)
				return 0.f;

			return (defuse_countdown - global_vars->get_curtime());
		}
	};
}