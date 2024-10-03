#pragma once

/* app defines */
#define GAME_NAME "cs2.exe"
#define APP_VERSION 1.0
#define USE_LOCALHOST 1

/* modules */
#define CLIENT_DLL "client.dll"
#define SCHEMASYSTEM_DLL "schemasystem.dll"

/* signatures */
#define GET_SCHEMA_SYSTEM "48 89 05 ? ? ? ? 4c 8d 0d ? ? ? ? 0f b6 45 ? 4c 8d 45 ? 33 f6"
#define GET_LOCAL_PLAYER_PAWN "48 8d 05 ? ? ? ? c3 cc cc cc cc cc cc cc cc 48 83 ec ? 8b 0d"
#define GET_ENTITY_LIST "48 8b 0d ? ? ? ? 48 89 7c 24 ? 8b fa c1 eb"
#define GET_GLOBAL_VARS "48 89 15 ? ? ? ? 48 89 42"

/* macros */
#define LOG(str, ...) \
	{ \
        const auto filename = std::filesystem::path(__FILE__).filename().string(); \
        printf("[%s] " str "\n", filename.c_str(), __VA_ARGS__); \
    }

#define LOG_ERROR(str, ...) \
    { \
        const auto filename = std::filesystem::path(__FILE__).filename().string(); \
        printf("[%s:%d] " str "\n", filename.c_str(), __LINE__, __VA_ARGS__); \
    }