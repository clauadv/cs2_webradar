#pragma once

namespace usermode::math
{
	template<typename T>
	union vector3
	{
		T data[3];
		struct
		{
			T x, y, z;
		};

		vector3 operator+(float in) const { return { this->x + in, this->y + in, this->z + in }; }
		vector3 operator+(vector3 in) const { return { this->x + in.x, this->y + in.y, this->z + in.z }; }
		vector3 operator-(float in) const { return { this->x - in, this->y - in, this->z - in }; }
		vector3 operator-(vector3 in) const { return { this->x - in.x, this->y - in.y, this->z - in.z }; }
		vector3 operator*(float in) const { return { this->x * in, this->y * in, this->z * in }; }
		vector3 operator*(vector3 in) const { return { this->x * in.x, this->y * in.y, this->z * in.z }; }
		vector3 operator/(float in) const { return { this->x / in, this->y / in, this->z / in }; }
		vector3 operator/(vector3 in) const { return { this->x / in.x, this->y / in.y, this->z / in.z }; }
		vector3 operator+=(float in) const { return { this->x += in, this->y += in, this->z += in }; }
		vector3 operator+=(vector3 in) const { return { this->x += in.x, this->y += in.y, this->z += in.z }; }
		vector3 operator-=(float in) const { return { this->x -= in, this->y -= in, this->z -= in }; }
		vector3 operator-=(vector3 in) const { return { this->x -= in.x, this->y -= in.y, this->z -= in.z }; }
		vector3 operator*=(float in) const { return { this->x *= in, this->y *= in, this->z *= in }; }
		vector3 operator*=(vector3 in) const { return { this->x *= in.x, this->y *= in.y, this->z *= in.z }; }
		vector3 operator/=(float in) const { return { this->x /= in, this->y /= in, this->z /= in }; }
		vector3 operator/=(vector3 in) const { return { this->x /= in.x, this->y /= in.y, this->z /= in.z }; }

		bool zero() const
		{
			return (this->x == 0.f && this->y == 0.f && this->z == 0.f);
		}
	};

	typedef vector3<float> fvector3;
}