#include "pch.hpp"

static std::unordered_map<std::string, bool> dropped_weapon_cache;

bool f::dropped_weapons::is_weapon(std::string weapon_name) 
{
	auto is_in_cache = dropped_weapon_cache.find(weapon_name);
	if (is_in_cache != dropped_weapon_cache.end()) {
		return is_in_cache->second;
	}

	bool is_weapon = weapon_name.starts_with("weapon_");

	dropped_weapon_cache[weapon_name] = is_weapon;

	return is_weapon;
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