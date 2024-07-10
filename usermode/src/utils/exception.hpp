#pragma once

namespace exc
{
	void log_file(const std::string& output);
	long __stdcall handler(EXCEPTION_POINTERS* info);
	bool setup();
}