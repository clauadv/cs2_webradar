#include "pch.hpp"

void f::bomb::get_carried_bomb(c_base_entity* bomb)
{
	m_bomb_idx = reinterpret_cast<uintptr_t>(bomb->m_hOwnerEntity()) & 0xffff;

	const auto scene_origin = bomb->get_scene_origin();
	if (scene_origin.is_zero())
		return;

	m_data["m_bomb"]["x"] = scene_origin.m_x;
	m_data["m_bomb"]["y"] = scene_origin.m_y;
}

void f::bomb::get_planted_bomb(c_planted_c4* planted_c4)
{
	if (!planted_c4->m_bBombTicking())
		return;

	const auto curtime = i::m_global_vars->m_curtime();

	const auto blow_time = (planted_c4->m_flC4Blow() - curtime);
	if (blow_time <= 0.f)
		return;

	const auto vec_origin = planted_c4->m_pGameSceneNode()->m_vecAbsOrigin();
	if (vec_origin.is_zero())
		return;

	const auto is_defused = planted_c4->m_bBombDefused();
	const auto is_defusing = planted_c4->m_bBeingDefused();
	const auto defuse_time = (planted_c4->m_flDefuseCountDown() - curtime);

	m_data["m_bomb"]["x"] = vec_origin.m_x;
	m_data["m_bomb"]["y"] = vec_origin.m_y;
	m_data["m_bomb"]["m_blow_time"] = blow_time;
	m_data["m_bomb"]["m_is_defused"] = is_defused;
	m_data["m_bomb"]["m_is_defusing"] = is_defusing;
	m_data["m_bomb"]["m_defuse_time"] = defuse_time;
}