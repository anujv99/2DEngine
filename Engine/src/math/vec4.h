#pragma once

#include <cfloat>

#include "prevmath.h"
#include "vec3.h"

namespace prev {

	class Vec4 {
	public:
		union {
			struct { float x, y, z, w; };
			struct { float r, g, b, a; };
		};

		Vec4();
		explicit Vec4(float _v);
		Vec4(float _x, float _y, float _z, float _w);
		explicit Vec4(const Vec2 & _vec);
		explicit Vec4(const Vec3 & _vec);
		explicit Vec4(const Vec3 & _vec, float _w);

		float & operator[](unsigned int _index) { ASSERT(_index < 4); return (&x)[_index]; }
		const float & operator[](unsigned int _index) const { ASSERT(_index < 4); return (&x)[_index]; }

		Vec4 & operator+=(const Vec4 & _vec);
		Vec4 & operator-=(const Vec4 & _vec);
		Vec4 & operator*=(const Vec4 & _vec);
		Vec4 & operator/=(const Vec4 & _vec);

		Vec4 & operator+=(float _v);
		Vec4 & operator-=(float _v);
		Vec4 & operator*=(float _v);
		Vec4 & operator/=(float _v);

		Vec4 operator+(const Vec4 & _vec) const;
		Vec4 operator-(const Vec4 & _vec) const;
		Vec4 operator-() const;
		Vec4 operator*(const Vec4 & _vec) const;
		Vec4 operator/(const Vec4 & _vec) const;

		Vec4 operator*(float _v) const;
		Vec4 operator/(float _v) const;

		Vec2 xy() const;
		Vec2 yx() const;
		Vec2 yz() const;
		Vec2 zy() const;
		Vec2 xz() const;
		Vec2 zx() const;

		Vec3 xyz() const;
		Vec3 xzy() const;
		Vec3 yxz() const;
		Vec3 yzx() const;
		Vec3 zxy() const;
		Vec3 zyx() const;

		bool operator==(const Vec4 & _vec) const;
		bool operator!=(const Vec4 & _vec) const;

	};

	bool CheckVec4(const Vec4 & _vec);

	float Length(const Vec4 & _vec);
	float LengthSqr(const Vec4 & _vec);
	float Dot(const Vec4 & _v1, const Vec4 & _v2);
	float Distance(const Vec4 & _v1, const Vec4 & _v2);

	Vec4 operator*(float _v, const Vec4 & _vec);

	Vec4 Cross(const Vec4 & _v1, const Vec4 & _v2);
	Vec4 Normalize(const Vec4 & _vec);
	Vec4 Max(const Vec4 & _v1, const Vec4 & _v2);
	Vec4 Min(const Vec4 & _v1, const Vec4 & _v2);
	Vec4 Abs(const Vec4 & _vec);
	Vec4 Lerp(const Vec4 & _v1, const Vec4 & _v2, float _v);
	Vec4 Clamp(const Vec4 & _vec, const Vec4 & _min, const Vec4 & _max);
	Vec4 Saturate(const Vec4 & _vec);

	inline std::ostream & operator<<(std::ostream & os, const Vec4 & _vec) {
		return os << "[Vec4 : x = " << _vec.x <<
			", y = " << _vec.y << ", z = " << _vec.z << ", w = " << _vec.w << "]";
	}

	#include "vec4.inl"
}
