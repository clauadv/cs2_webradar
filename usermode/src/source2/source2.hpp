#pragma once

// modules
#define CLIENT_DLL "client.dll"
#define SCHEMASYSTEM_DLL "schemasystem.dll"

// signatures
#define GET_SCHEMA_SYSTEM "48 8d 0d ? ? ? ? e9 ? ? ? ? cc cc cc cc 48 8d 0d ? ? ? ? e9 ? ? ? ? cc cc cc cc 48 83 ec 28"
#define GET_LOCAL_PLAYER_PAWN "48 8d 05 ? ? ? ? c3 cc cc cc cc cc cc cc cc 48 83 ec ? 8b 0d"
#define GET_ENTITY_LIST "48 8b 0d ? ? ? ? 48 89 7c 24 ? 8b fa c1 eb"
#define GET_GLOBAL_VARS "48 89 0d ? ? ? ? 48 89 41"
#define GET_PLANTED_C4 "48 8b 15 ? ? ? ? ff c0 48 8d 4c 24"