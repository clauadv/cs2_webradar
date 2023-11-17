#pragma once

namespace cs2
{
	enum class e_team : int
	{
		none,
		spectator,
		terrorist,
		counter_terrorist
	};

	class c_base_player : public c_base_entity
	{
	public:
		static void iterate(const function<void(c_base_player*, c_base_entity*, size_t)>& function)
		{
			const auto global_vars = m_cs2.get_global_vars();
			if (!global_vars)
				return;

			const auto entity_list = m_cs2.get_entity_list();
			if (!entity_list)
				return;

			const auto map_name = global_vars->get_map_name();

			for (size_t idx{ 0 }; idx < 64 && map_name.find("invalid") == string::npos; idx++)
			{
				const auto entity = entity_list->get<c_base_entity*>(idx);
				if (!entity)
					continue;

				const auto entity_pawn = entity->get_pawn();
				if (!entity_pawn)
					continue;

				const auto player = entity_list->get<c_base_player*>(entity_pawn & 0x7fff);
				if (!player)
					continue;

				function(player, entity, idx);
			}
		}

		string get_name()
		{
			const auto sanitized_player_name = m_memory.read_t<uintptr_t>(this + m_offsets.get_sanitized_player_name());
			if (!sanitized_player_name)
				return "invalid";

			auto player_name = m_memory.read_t<string>(sanitized_player_name);
			if (player_name.empty())
				return "invalid";

			return player_name;
		}

		int get_health()
		{
			return m_memory.read_t<int>(this + m_offsets.get_health());
		}

		int get_armor()
		{
			return m_memory.read_t<int>(this + m_offsets.get_armor());
		}

		e_team get_team()
		{
			return m_memory.read_t<e_team>(this + m_offsets.get_team_num());
		}

		fvector3 get_position()
		{
			return m_memory.read_t<fvector3>(this + m_offsets.get_old_origin());
		}

		bool is_dead()
		{
			return this->get_health() <= 0;
		}

		fvector3 get_eye_angles()
		{
			return m_memory.read_t<fvector3>(this + m_offsets.get_ang_eye_angles());
		}

		bool has_helmet()
		{
			const auto item_services = m_memory.read_t<uintptr_t>(this + m_offsets.get_item_services());
			if (!item_services)
				return false;

			return m_memory.read_t<bool>(item_services + m_offsets.get_has_helmet());
		}

		bool has_defuser()
		{
			const auto item_services = m_memory.read_t<uintptr_t>(this + m_offsets.get_item_services());
			if (!item_services)
				return false;

			return m_memory.read_t<bool>(item_services + m_offsets.get_has_defuser());
		}

		c_player_weapon_services* get_weapon_services()
		{
			return m_memory.read_t<c_player_weapon_services*>(this + m_offsets.get_weapon_services());
		}

		string get_model_name()
		{
			const auto model_name_ptr = m_memory.read_t<uintptr_t>(m_memory.read_t<uintptr_t>(this + m_offsets.get_game_scene_node()) + m_offsets.get_model_state() + m_offsets.get_model_name());
			if (!model_name_ptr)
				return "invalid";

			const auto model_path = m_memory.read_t<string>(model_name_ptr);
			if (model_path.empty())
				return "invalid";

			return model_path.substr(model_path.rfind("/") + 1, model_path.rfind(".") - model_path.rfind("/") - 1);
		}
	};
}