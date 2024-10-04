#pragma once

using fnv1a_t = uint64_t;

namespace fnv1a
{
	constexpr fnv1a_t m_basis = 0xcbf29ce484222325ull;
	constexpr fnv1a_t m_prime = 0x100000001b3ull;

	consteval fnv1a_t hash_const(const char* string, const fnv1a_t key = m_basis) noexcept
	{
		return (string[0] == '\0') ? key : hash_const(&string[1], (key ^ static_cast<fnv1a_t>(string[0])) * m_prime);
	}

	inline fnv1a_t hash(std::string string, fnv1a_t key = m_basis) noexcept
	{
		for (std::size_t i = 0u; i < string.size(); ++i)
			key = (key ^ string[i]) * m_prime;

		return key;
	}
}