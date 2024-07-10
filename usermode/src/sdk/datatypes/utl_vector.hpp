#pragma once

template <typename T>
class c_network_utl_vector_base
{
public:
	uint32_t m_size;
	T* m_elements;
};