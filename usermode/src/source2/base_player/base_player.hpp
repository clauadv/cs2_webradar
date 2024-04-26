#pragma once

namespace src::source2
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
		static c_base_player* get();

		static void iterate(const function<void(c_base_player*, c_base_entity*, size_t)>& function)
		{
			const auto global_vars = m_global_vars->get();
			if (!global_vars)
				return;

			const auto entity_list = m_entity_list->get();
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
			const auto sanitized_player_name = m_memory.read_t<uintptr_t>(this + GET_SCHEMA("ccsplayercontroller->m_ssanitizedplayername"));
			if (!sanitized_player_name)
				return "invalid";

			auto player_name = m_memory.read_t<string>(sanitized_player_name);
			if (player_name.empty())
				return "invalid";

			return player_name;
		}

		int get_health()
		{
			return m_memory.read_t<int>(this + GET_SCHEMA("c_baseentity->m_ihealth"));
		}

		int get_armor()
		{
			return m_memory.read_t<int>(this + GET_SCHEMA("c_csplayerpawn->m_armorvalue"));
		}

		e_team get_team()
		{
			return m_memory.read_t<e_team>(this + GET_SCHEMA("c_baseentity->m_iteamnum"));
		}

		fvector3 get_position()
		{
			return m_memory.read_t<fvector3>(this + GET_SCHEMA("c_baseplayerpawn->m_voldorigin"));
		}

		bool is_dead()
		{
			return this->get_health() <= 0;
		}

		fvector3 get_eye_angles()
		{
			return m_memory.read_t<fvector3>(this + GET_SCHEMA("c_csplayerpawnbase->m_angeyeangles"));
		}

		bool has_helmet()
		{
			const auto item_services = m_memory.read_t<uintptr_t>(this + GET_SCHEMA("c_baseplayerpawn->m_pitemservices"));
			if (!item_services)
				return false;

			return m_memory.read_t<bool>(item_services + GET_SCHEMA("ccsplayer_itemservices->m_bhashelmet"));
		}

		bool has_defuser()
		{
			const auto item_services = m_memory.read_t<uintptr_t>(this + GET_SCHEMA("c_baseplayerpawn->m_pitemservices"));
			if (!item_services)
				return false;

			return m_memory.read_t<bool>(item_services + GET_SCHEMA("ccsplayer_itemservices->m_bhasdefuser"));
		}

		c_player_weapon_services* get_weapon_services()
		{
			return m_memory.read_t<c_player_weapon_services*>(this + GET_SCHEMA("c_baseplayerpawn->m_pweaponservices"));
		}

		string get_model_name()
		{
			const auto model_name_ptr = m_memory.read_t<uintptr_t>(m_memory.read_t<uintptr_t>(this + GET_SCHEMA("c_baseentity->m_pgamescenenode")) + GET_SCHEMA("cskeletoninstance->m_modelstate") + GET_SCHEMA("cmodelstate->m_modelname"));
			if (!model_name_ptr)
				return "invalid";

			const auto model_path = m_memory.read_t<string>(model_name_ptr);
			if (model_path.empty())
				return "invalid";

			return model_path.substr(model_path.rfind("/") + 1, model_path.rfind(".") - model_path.rfind("/") - 1);
		}
	};
}
inline src::source2::c_base_player* m_base_player{};