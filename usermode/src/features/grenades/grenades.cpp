#include "pch.hpp"

void f::nades::add(c_base_entity* e, const std::string& t)
{
	if (!e) return;
	const auto scene_node = e->m_pGameSceneNode();
	if (!scene_node) return;
	const auto& p = scene_node->m_vecAbsOrigin();
	if (p.m_x == 0.f && p.m_y == 0.f && p.m_z == 0.f) return;
	nlohmann::json nade = {};
	nade["m_type"] = t;
	nade["m_pos_x"] = p.m_x;
	nade["m_pos_y"] = p.m_y;
	nade["m_pos_z"] = p.m_z;
	f::m_data["m_grenades"].push_back(nade);
}
