#pragma once

class c_address
{
public:
	c_address() = default;
	explicit c_address(const uintptr_t address) : m_address(address) {}

	uintptr_t address() const
	{
		return this->m_address;
	}

	c_address add(const ptrdiff_t offset) const
	{
		return c_address(this->m_address + offset);
	}

	c_address sub(const ptrdiff_t offset) const
	{
		return c_address(this->m_address - offset);
	}

	c_address rip(const ptrdiff_t offset = 0x03, const size_t length = 0x07) const;

	template<typename T>
	T as() const
	{
		if constexpr (std::is_same_v<T, uintptr_t>)
			return this->m_address;
		else
			return reinterpret_cast<T>(this->m_address);
	}
private:
	uintptr_t m_address{};
};