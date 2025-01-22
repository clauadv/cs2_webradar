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

/* ext/includes */
#include "ext/nlohmann/json.hpp"
#include "ext/easywsclient/easywsclient.hpp"
#include "ext/fnv1a/fnv1a.hpp"
#include "ext/curl/include/curl.h"

/* root */
#include "common.hpp"

/* utils */
#include "utils/config.hpp"
#include "utils/utils.hpp"
#include "utils/exception.hpp"
#include "utils/address.hpp"
#include "utils/memory.hpp"

/* core */
#include "core/interfaces.hpp"
#include "core/schema.hpp"

/* datatypes */
#include "sdk/datatypes/utl_ts_hash.hpp"
#include "sdk/datatypes/utl_vector.hpp"
#include "sdk/datatypes/vector.hpp"

/* sdk */
#include "sdk/entity_handle.hpp"
#include "sdk/entity.hpp"

/* sdk/interfaces */
#include "sdk/interfaces/game_entity_system.hpp"
#include "sdk/interfaces/schema_system.hpp"
#include "sdk/interfaces/global_vars.hpp"

#include "core/sdk.hpp"

/* features */
#include "features/features.hpp"