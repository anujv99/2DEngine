#pragma once

#include <cfloat>
#include <cmath>

#include "prevmath.h"
#include "vec2.h"

namespace prev {

	class Vec3 {
	public:
		union {
			struct { float x, y, z; };
			struct { float r, g, b; };
		};

		Vec3();
		explicit Vec3(float _v);
		Vec3(float _x, float _y, float _z);
		explicit Vec3(const Vec2 &_vec);

		float & operator[](unsigned int _index) { ASSERT(_index < 3, "Index > 3"); return (&x)[_index]; }
		const float & operator[](unsigned int _index) const { ASSERT(_index < 3, "Index > 3"); return (&x)[_index]; }

		Vec3 & operator+=(const Vec3 & _vec);
		Vec3 & operator-=(const Vec3 & _vec);
		Vec3 & operator*=(const Vec3 & _vec);
		Vec3 & operator/=(const Vec3 & _vec);

		Vec3 & operator+=(float _v);
		Vec3 & operator-=(float _v);
		Vec3 & operator*=(float _v);
		Vec3 & operator/=(float _v);

		Vec3 operator+(const Vec3 & _vec) const;
		Vec3 operator-(const Vec3 & _vec) const;
		Vec3 operator-() const;
		Vec3 operator*(const Vec3 & _vec) const;
		Vec3 operator/(const Vec3 & _vec) const;

		Vec3 operator*(float _v) const;
		Vec3 operator/(float _v) const;

		Vec2 xy() const;
		Vec2 yx() const;
		Vec2 yz() const;
		Vec2 zy() const;
		Vec2 xz() const;
		Vec2 zx() const;

		bool operator==(const Vec3 & _vec) const;
		bool operator!=(const Vec3 & _vec) const;
	};

	bool CheckVec3(const Vec3 & _vec);

	float Length(const Vec3 & _vec);
	float LengthSqr(const Vec3 & _vec);
	float Dot(const Vec3 & _v1, const Vec3 & _v2);
	float Distance(const Vec3 & _v1, const Vec3 & _v2);

	Vec3 operator*(float _v, const Vec3 & _vec);

	Vec3 Cross(const Vec3 & _v1, const Vec3 & _v2);
	Vec3 Normalize(const Vec3 & _vec);
	Vec3 Max(const Vec3 & _v1, const Vec3 & _v2);
	Vec3 Min(const Vec3 & _v1, const Vec3 & _v2);
	Vec3 Abs(const Vec3 & _vec);
	Vec3 Lerp(const Vec3 & _v1, const Vec3 & _v2, float _v);
	Vec3 Clamp(const Vec3 & _vec, const Vec3 & _min, const Vec3 & _max);
	Vec3 Saturate(const Vec3 & _vec);

	Vec3 RotateAxisX(const Vec3 & _dir, float _angle);
	Vec3 RotateAxisY(const Vec3 & _dir, float _angle);
	Vec3 RotateAxisZ(const Vec3 & _dir, float _angle);
	Vec3 RotateAxis(const Vec3 & _dir, float _angle, const Vec3 & _axis);

	#include "vec3.inl"

	inline std::ostream & operator<<(std::ostream & os, const Vec3 & _vec) {
		return os << "[vec2 : x = " << _vec.x <<
			", y = " << _vec.y <<
			", z = " << _vec.z << "]";
	}

}
