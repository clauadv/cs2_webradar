#pragma once

namespace src::cs2
{
	class c_planted_c4 : public c_base_entity
	{
	public:
		static c_planted_c4* get();

		bool get_state();

		float get_blow_time()
		{
			const auto global_vars = m_global_vars->get();
			if (!global_vars)
				return 0.f;

			const auto c4_blow = m_memory.read_t<float>(this + GET_SCHEMA("c_plantedc4->m_flc4blow"));
			if (!c4_blow)
				return 0.f;

			return (c4_blow - global_vars->get_curtime());
		}

		bool get_defused()
		{
			return m_memory.read_t<bool>(this + GET_SCHEMA("c_plantedc4->m_bbombdefused"));
		}

		bool get_being_defused()
		{
			return m_memory.read_t<bool>(this + GET_SCHEMA("c_plantedc4->m_bbeingdefused"));
		}

		float get_defuse_time()
		{
			const auto global_vars = m_global_vars->get();
			if (!global_vars)
				return 0.f;

			const auto defuse_countdown = m_memory.read_t<float>(this + GET_SCHEMA("c_plantedc4->m_fldefusecountdown"));
			if (!defuse_countdown)
				return 0.f;

			return (defuse_countdown - global_vars->get_curtime());
		}
	};
}
inline src::cs2::c_planted_c4* m_planted_c4{};