#pragma once

namespace src::cs2
{
	enum class e_color : int
	{
		blue,
		green,
		yellow,
		orange,
		purple,
		white
	};

	class c_base_entity
	{
	public:
		static void iterate(const string& class_name, const function<void(c_base_entity*)>& function)
		{
			const auto global_vars = m_cs2.get_global_vars();
			if (!global_vars)
				return;

			const auto entity_list = m_cs2.get_entity_list();
			if (!entity_list)
				return;

			const auto map_name = global_vars->get_map_name();

			for (size_t idx{ 64 }; idx < 1024 && map_name.find("invalid") == string::npos; idx++)
			{
				const auto entity = entity_list->get<c_base_entity*>(idx);
				if (!entity)
					continue;

				const auto name = entity->get_name();
				if (name.find(class_name) == string::npos)
					continue;

				function(entity);
			}
		}

		string get_name()
		{
			const auto designer_name = m_memory.read_t<uintptr_t>(m_memory.read_t<uintptr_t>(this + GET_SCHEMA("centityinstance->m_pentity")) + GET_SCHEMA("centityidentity->m_designername"));
			if (!designer_name)
				return "invalid";

			const auto name = m_memory.read_t<string>(designer_name);
			if (name.empty())
				return "invalid";

			return name;
		}

		uintptr_t get_pawn()
		{
			return m_memory.read_t<uintptr_t>(this + GET_SCHEMA("ccsplayercontroller->m_hplayerpawn"));
		}

		e_color get_color()
		{
			const auto color = m_memory.read_t<e_color>(this + GET_SCHEMA("ccsplayercontroller->m_icompteammatecolor"));
			if (color == static_cast<e_color>(-1))
			{
				return e_color::white;
			}

			return color;
		}

		int get_money()
		{
			const auto in_game_money_services = m_memory.read_t<uintptr_t>(this + GET_SCHEMA("ccsplayercontroller->m_pingamemoneyservices"));
			if (!in_game_money_services)
				return 0;

			return m_memory.read_t<int>(in_game_money_services + GET_SCHEMA("ccsplayercontroller_ingamemoneyservices->m_iaccount"));
		}

		string get_steam_id()
		{
			return to_string(m_memory.read_t<uintptr_t>(this + GET_SCHEMA("cbaseplayercontroller->m_steamid")));
		}

		fvector3 get_vec_origin()
		{
			const auto game_scene_node = m_memory.read_t<uintptr_t>(this + GET_SCHEMA("c_baseentity->m_pgamescenenode"));
			if (!game_scene_node)
				return fvector3{};

			return m_memory.read_t<fvector3>(game_scene_node + GET_SCHEMA("cgamescenenode->m_vecabsorigin"));
		}

		c_base_entity* get_owner()
		{
			return m_memory.read_t<c_base_entity*>(this + GET_SCHEMA("c_baseentity->m_hownerentity"));
		}
	};
}