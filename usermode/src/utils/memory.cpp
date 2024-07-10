#include "pch.hpp"

bool mem::setup()
{
	if (!m_memory->attach())
	{
		LOG_ERROR("failed to attach to cs2.exe");
		return false;
	}

	if (!m_memory->get_id())
	{
		LOG_ERROR("failed to get a process id for cs2.exe");
		return false;
	}

	return true;
}