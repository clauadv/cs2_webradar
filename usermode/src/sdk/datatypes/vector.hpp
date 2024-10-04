#pragma once

struct f_vector
{
	float m_x; // 0x00(0x04)
	float m_y; // 0x04(0x04)
	float m_z; // 0x08(0x04)

	inline f_vector()
		: m_x(0.f), m_y(0.f), m_z(0.f)
	{
	}

	inline f_vector(decltype(m_x) value)
		: m_x(value), m_y(value), m_z(value)
	{
	}

	inline f_vector(decltype(m_x) x, decltype(m_y) y, decltype(m_z) z)
		: m_x(x), m_y(y), m_z(z)
	{
	}

	bool is_zero() const
	{
		return (m_x == 0.f && m_y == 0.f && m_z == 0.f);
	}
};
static_assert(sizeof(f_vector) == 0x0c, "wrong size on f_vector");