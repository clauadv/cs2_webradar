#pragma once

class c_engine_client
{
public:
	static std::string get_product_version()
	{
		const auto offset = m_memory->find_pattern(ENGINE2_DLL, GET_PRODUCT_VERSION)->rip().as<uintptr_t>();
		if (!offset)
			return {};

		const auto name_ptr = m_memory->read_t<uintptr_t>(offset);
		return m_memory->read_t<std::string>(name_ptr);
	}
};