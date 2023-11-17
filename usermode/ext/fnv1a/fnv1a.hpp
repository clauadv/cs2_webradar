#pragma once

namespace ext::fnv1a
{
	constexpr uint64_t m_basis = 0xcbf29ce484222325;
	constexpr uint64_t m_prime = 0x100000001b3;

	consteval uint64_t hash_const(const char* str, const uint64_t key = m_basis)
	{
		return (str[0] == '\0') ? key : hash_const(&str[1], (key ^ static_cast<uint64_t>(str[0])) * m_prime);
	}

	inline uint64_t hash(std::string str, uint64_t key = m_basis)
	{
		for (auto i{ 0 }; i < str.size(); ++i)
		{
			key ^= str.data()[i];
			key *= m_prime;
		}

		return key;
	}
}