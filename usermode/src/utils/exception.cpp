#include "pch.hpp"

void exc::log_file(const std::string& output)
{
	const std::string file_name = std::format("usermode_crash_{}.txt", std::time(nullptr));

	std::ofstream output_file(file_name);
	if (!output_file)
	{
		LOG_ERROR("failed to open '%s' for writing", file_name.c_str());
		return;
	}

	output_file << output << std::endl;
}

long __stdcall exc::handler(EXCEPTION_POINTERS* info)
{
	const auto code = info->ExceptionRecord->ExceptionCode;
	if (code != EXCEPTION_ACCESS_VIOLATION)
		return 0;

	const auto address = info->ExceptionRecord->ExceptionAddress;
	MEMORY_BASIC_INFORMATION memory_info{};
	std::uintptr_t alloc_base{};

	if (VirtualQuery(address, &memory_info, sizeof(memory_info)))
		alloc_base = reinterpret_cast<uintptr_t>(memory_info.AllocationBase);

	const auto app_version = std::to_string(APP_VERSION);
	const auto output = std::format("app ({}) crashed at usermode.exe+{:#04x}", app_version.substr(0, app_version.find(".") + 2), static_cast<std::uintptr_t>(info->ContextRecord->Rip) - alloc_base);
	log_file(output);

	MessageBoxA(nullptr, output.c_str(), "usermode", MB_OK);

	return 0;
}

bool exc::setup()
{
	const auto handle = AddVectoredExceptionHandler(false, handler);
	if (!handle)
	{
		LOG_ERROR("failed to add vectored exception handler");
		return false;
	}

	LOG("exception handler -> 0x%llx", handle);
	return true;
}