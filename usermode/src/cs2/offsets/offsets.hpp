#pragma once

namespace src::cs2
{
	class c_offsets
	{
	private:
		uintptr_t m_entity_list{ 0 };
		uintptr_t m_global_vars{ 0 };
		uintptr_t m_local_player_pawn{ 0 };
		uintptr_t m_planted_c4{ 0 };

	public:
		bool setup()
		{
			nlohmann::json data{};

			ifstream offsets_json{ "offsets.json" };
			if (!offsets_json.good())
			{
				LOG_ERROR("failed to open offsets.json");
				return false;
			}

			offsets_json >> data;

			this->m_entity_list = data["client_dll"]["data"]["dwEntityList"]["value"];
			this->m_global_vars = data["client_dll"]["data"]["dwGlobalVars"]["value"];
			this->m_local_player_pawn = data["client_dll"]["data"]["dwLocalPlayerPawn"]["value"];
			this->m_planted_c4 = data["client_dll"]["data"]["dwPlantedC4"]["value"];

			data.clear();
			return true;
		}

		uintptr_t get_entity_list() { return this->m_entity_list; }
		uintptr_t get_global_vars() { return this->m_global_vars; }
		uintptr_t get_local_player_pawn() { return this->m_local_player_pawn; }
		uintptr_t get_planted_c4() { return this->m_planted_c4; }
	};
}
inline src::cs2::c_offsets m_offsets{};