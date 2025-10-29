#include "pch.hpp"

bool f::dropped_weapons::is_weapon(std::string weapon_name) 
{
	return weapon_name.starts_with("weapon_");
}


void f::dropped_weapons::get_weapon(c_base_entity* weapon)
{
	const auto vec_origin = weapon->m_pGameSceneNode()->m_vecOrigin();

	std::string weapon_name = reinterpret_cast<c_base_player_weapon*>(weapon)->m_WeaponData()->m_szName();

	if (weapon_name.empty())
		return;

	weapon_name.erase(weapon_name.begin(), weapon_name.begin() + 7);

	if (vec_origin.is_zero())
		return;


	m_dropped_weapon_data["m_name"] = weapon_name;
	m_dropped_weapon_data["m_x"] = vec_origin.m_x;
	m_dropped_weapon_data["m_y"] = vec_origin.m_y;
}