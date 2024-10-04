#include "pch.hpp"

void sdk::update()
{
	sdk::m_local_controller = c_cs_player_controller::get_local_player_controller();
}