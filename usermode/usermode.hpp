#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <cstdint>
#include <chrono>
#include <fstream>

// ext/includes
#include "ext/nlohmann/json.hpp"
#include "ext/easywsclient/easywsclient.hpp"

// src/macros
#define LOG_INFO(text, ...) printf("[info] " text "\n", __VA_ARGS__)
#define LOG_ERROR(text, ...) printf("[error] " text "\n", __VA_ARGS__)

// src/includes
#include "driver/driver.hpp"
#include "math/vector.hpp"
#include "cs2/offsets/offsets.hpp"
#include "cs2/classes/global_vars.hpp"
#include "cs2/classes/action_tracking_services.hpp"
#include "cs2/classes/in_game_money_services.hpp"
#include "cs2/classes/base_entity.hpp"
#include "cs2/classes/entity_list.hpp"
#include "cs2/classes/weapon_base_data.hpp"
#include "cs2/classes/base_player_weapon.hpp"
#include "cs2/classes/player_weapon_services.hpp"
#include "cs2/classes/base_player.hpp"
#include "cs2/cs2.hpp"