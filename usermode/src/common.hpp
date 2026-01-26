#pragma once

/* current build of cs2_webradar */
#define CS2_WEBRADAR_VERSION "v1.2.9"

/* game modules */
#define CLIENT_DLL "client.dll"
#define ENGINE2_DLL "engine2.dll"
#define SCHEMASYSTEM_DLL "schemasystem.dll"

/* game signatures */
#define GET_SCHEMA_SYSTEM "48 89 05 ? ? ? ? 4c 8d 0d ? ? ? ? 33 c0"
#define GET_ENTITY_LIST "48 8b 0d ? ? ? ? 48 89 7c 24 ? 8b fa c1 eb"
#define GET_GLOBAL_VARS "48 89 15 ? ? ? ? 48 89 42"
#define GET_LOCAL_PLAYER_CONTROLLER "4c 8d 05 ? ? ? ? 33 d2 4d 8b 04 c0"

/* custom defines */
#define LOG_INFO(str, ...) \
    printf(" [info] " str "\n", __VA_ARGS__)

#define LOG_WARNING(str, ...) \
    printf(" [warning] " str "\n", __VA_ARGS__)

#define LOG_ERROR(str, ...) \
    { \
        const auto filename = std::filesystem::path(__FILE__).filename().string(); \
        printf(" [error] [%s:%d] " str "\n", filename.c_str(), __LINE__, __VA_ARGS__); \
        std::this_thread::sleep_for(std::chrono::seconds(5)); \
    }