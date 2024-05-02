#pragma once

class c_exception_handler
{
public:
	static void log_file(const std::string& output)
	{
		const std::string file_name = "usermode_crash.txt";

		if (std::filesystem::exists(file_name))
		{
			if (!std::filesystem::remove(file_name))
			{
				LOG_ERROR("failed to remove existing file '%s'", file_name.c_str());
				return;
			}
		}

		std::ofstream output_file(file_name);
		if (!output_file)
		{
			LOG_ERROR("failed to open '%s' for writing", file_name.c_str());
			return;
		}

		output_file << output << std::endl;
	}

	static long __stdcall handler(EXCEPTION_POINTERS* info)
	{
		const auto code = info->ExceptionRecord->ExceptionCode;
		if (code != EXCEPTION_ACCESS_VIOLATION)
			return 0;

		const auto address = info->ExceptionRecord->ExceptionAddress;
		MEMORY_BASIC_INFORMATION memory_info{};
		std::uintptr_t alloc_base{};

		if (VirtualQuery(address, &memory_info, sizeof(memory_info)))
			alloc_base = reinterpret_cast<uintptr_t>(memory_info.AllocationBase);

		const auto output = std::format("app crashed at usermode.exe+{:#04x}", static_cast<std::uintptr_t>(info->ContextRecord->Rip) - alloc_base);
		log_file(output);

		return 0;
	}

	static bool setup()
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
};