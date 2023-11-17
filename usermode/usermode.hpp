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

// src/modules
#define CLIENT_DLL "client.dll"
#define SCHEMASYSTEM_DLL "schemasystem.dll"

// src/signatures
#define GET_SCHEMA_SYSTEM "48 8d 0d ? ? ? ? e9 ? ? ? ? cc cc cc cc 48 8d 0d ? ? ? ? e9 ? ? ? ? cc cc cc cc 48 83 ec 28"
#define GET_LOCAL_PLAYER_PAWN "48 8d 05 ? ? ? ? c3 cc cc cc cc cc cc cc cc 48 83 ec ? 8b 0d"
#define GET_ENTITY_LIST "48 8b 0d ? ? ? ? 48 89 7c 24 ? 8b fa c1 eb"
#define GET_GLOBAL_VARS "48 89 0d ? ? ? ? 48 89 41"
#define GET_PLANTED_C4 "48 8b 15 ? ? ? ? ff c0 48 8d 4c 24"

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

#include "cs2/schema/schema.hpp"

#include "cs2/classes/global_vars.hpp"
#include "cs2/classes/entity_list.hpp"
#include "cs2/classes/weapon_base_data.hpp"

#include "cs2/classes/base_entity.hpp"
#include "cs2/classes/base_player_weapon.hpp"
#include "cs2/classes/player_weapon_services.hpp"
#include "cs2/classes/base_player.hpp"
#include "cs2/classes/planted_c4.hpp"

#include "cs2/cs2.hpp"

#include "features/features.hpp"