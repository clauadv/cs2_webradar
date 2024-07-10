#include "pch.hpp"

c_address c_address::rip(const ptrdiff_t offset, const size_t length) const
{
	auto base = this->m_address;
	const auto rip_offset = m_memory->read_t<int32_t>(base + offset);

	base += rip_offset;
	base += length;

	return c_address(base);
}