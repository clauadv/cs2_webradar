#include "pch.hpp"

static long __stdcall handler(EXCEPTION_POINTERS* info)
{
	if (info->ExceptionRecord->ExceptionCode != EXCEPTION_ACCESS_VIOLATION)
		return {};

	MEMORY_BASIC_INFORMATION memory_info = {};
	const auto alloc_base = VirtualQuery(info->ExceptionRecord->ExceptionAddress, &memory_info, sizeof(memory_info))
		? reinterpret_cast<uintptr_t>(memory_info.AllocationBase) : 0;

	const auto output = std::format("app crashed at usermode.exe+{:#04x}", static_cast<uintptr_t>(info->ContextRecord->Rip) - alloc_base);
	MessageBoxA(nullptr, output.c_str(), "usermode " CS2_WEBRADAR_VERSION, MB_OK);

	return {};
}

bool exc::setup()
{
	return AddVectoredExceptionHandler(false, handler) != nullptr;
}