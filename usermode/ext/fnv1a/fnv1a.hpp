#pragma once

using fnv1a_t = uint64_t;

namespace ext::fnv1a
{
	constexpr fnv1a_t m_basis = 0xcbf29ce484222325;
	constexpr fnv1a_t m_prime = 0x100000001b3;

	consteval fnv1a_t hash_const(const char* str, const fnv1a_t key = m_basis)
	{
		return (str[0] == '\0') ? key : hash_const(&str[1], (key ^ static_cast<fnv1a_t>(str[0])) * m_prime);
	}

	inline fnv1a_t hash(std::string str, fnv1a_t key = m_basis)
	{
		for (auto i{ 0 }; i < str.size(); ++i)
		{
			key ^= str.data()[i];
			key *= m_prime;
		}

		return key;
	}
}