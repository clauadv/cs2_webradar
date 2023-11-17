#pragma once

namespace math
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

		vector3 normalize() const
		{
			vector3 out = *this;

			if (out.x > 89)
				out.x = 89;

			if (out.x < -89)
				out.x = -89;

			while (out.y > 180)
				out.y -= 360;

			while (out.y < -180)
				out.y += 360;

			out.z = 0;

			return out;
		}

		bool zero() const
		{
			return (this->x == 0.f && this->y == 0.f && this->z == 0.f);
		}
	};
}
typedef math::vector3<float> fvector3;