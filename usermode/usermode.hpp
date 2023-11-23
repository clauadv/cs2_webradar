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
#include <thread>
#include <winternl.h>

// ext/includes
#include "ext/nlohmann/json.hpp"
#include "ext/easywsclient/easywsclient.hpp"
#include "ext/fnv1a/fnv1a.hpp"

using namespace ext;
using namespace std;

// src/defines
#define USE_LOCALHOST 1

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
#include "utils/utils.hpp"
#include "math/vector.hpp"

#include "memory/address/address.hpp"
// #include "driver/driver.hpp"
#include "memory/memory.hpp"

#include "source2/source2.hpp"
#include "source2/utils/utl_ts_hash.hpp"
#include "source2/schema/schema.hpp"
#include "source2/global_vars/global_vars.hpp"
#include "source2/entity_list/entity_list.hpp"
#include "source2/weapon_base_data/weapon_base_data.hpp"
#include "source2/base_entity/base_entity.hpp"
#include "source2/base_player_weapon/base_player_weapon.hpp"
#include "source2/player_weapon_services/player_weapon_services.hpp"
#include "source2/base_player/base_player.hpp"
#include "source2/planted_c4/planted_c4.hpp"
#include "source2/base_sdk/base_sdk.hpp"

#include "features/features.hpp"