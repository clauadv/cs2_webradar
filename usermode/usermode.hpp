#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <iostream>
#include <cstdint>
#include <chrono>
#include <fstream>
#include <set>
#include <regex>

// ext/includes
#include "ext/nlohmann/json.hpp"
#include "ext/easywsclient/easywsclient.hpp"
#include "ext/fnv1a/fnv1a.hpp"

using namespace ext;
using namespace std;

// src/defines
#define USE_LOCALHOST 1
#define GAME_VERSION 2000198

// src/modules
#define SCHEMASYSTEM_DLL "schemasystem.dll"

// src/signatures
#define SCHEMA_SYSTEM "48 8d 0d ? ? ? ? e9 ? ? ? ? cc cc cc cc 48 8d 0d ? ? ? ? e9 ? ? ? ? cc cc cc cc 48 83 ec 28"

// src/macros
#define LOG(str, ...) \
	{ \
        const auto filename = filesystem::path(__FILE__).filename().string(); \
        printf("[%s] " str "\n", filename.c_str(), __VA_ARGS__); \
    }

#define LOG_ERROR(str, ...) \
    { \
        const auto filename = filesystem::path(__FILE__).filename().string(); \
        printf("[%s:%d] " str "\n", filename.c_str(), __LINE__, __VA_ARGS__); \
    }

// src/includes
// #include "driver/driver.hpp"

#include "memory/address/address.hpp"
#include "memory/memory.hpp"

#include "utils/utils.hpp"
#include "math/vector.hpp"

#include "cs2/utils/utl_ts_hash.hpp"

#include "cs2/schema/schema_system.hpp"

#include "cs2/offsets/offsets.hpp"
#include "cs2/classes/global_vars.hpp"
#include "cs2/classes/entity_list.hpp"
#include "cs2/classes/weapon_base_data.hpp"

#include "cs2/cs2.hpp"

#include "cs2/classes/base_entity.hpp"
#include "cs2/classes/base_player_weapon.hpp"
#include "cs2/classes/player_weapon_services.hpp"
#include "cs2/classes/base_player.hpp"
#include "cs2/classes/planted_c4.hpp"

#include "features/features.hpp"