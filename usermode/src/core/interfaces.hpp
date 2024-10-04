#pragma once

class c_schema_system;
class c_game_entity_system;
class c_global_vars;

namespace i
{
	bool setup();

	inline c_schema_system* m_schema_system = nullptr;
	inline c_game_entity_system* m_game_entity_system = nullptr;
	inline c_global_vars* m_global_vars = nullptr;
}