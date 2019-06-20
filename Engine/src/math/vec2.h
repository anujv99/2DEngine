#pragma once

#include <cfloat>

#include "prevmath.h"
#include "src/common/assert.h"

#include "gm/gmMathVector.h"

namespace prev {

	class Vec2 {
	public:
		union {
			struct { float x, y; };
			struct { float u, v; };
			gmvec2 gmv;
		};

		Vec2();
		explicit Vec2(float _v);
		Vec2(gmvec2 _vec);
		Vec2(float _x, float _y);

		float & operator[](unsigned int _index) { ASSERT(_index < 2); return (&x)[_index]; }
		const float & operator[](unsigned int _index) const { ASSERT(_index < 2); return (&x)[_index]; }

		Vec2 & operator+=(const Vec2 & _vec);
		Vec2 & operator-=(const Vec2 & _vec);
		Vec2 & operator*=(const Vec2 & _vec);
		Vec2 & operator/=(const Vec2 & _vec);

		Vec2 & operator+=(float _v);
		Vec2 & operator-=(float _v);
		Vec2 & operator*=(float _v);
		Vec2 & operator/=(float _v);

		Vec2 operator+(const Vec2 & _vec) const;
		Vec2 operator-(const Vec2 & _vec) const;
		Vec2 operator-() const;
		Vec2 operator*(const Vec2 & _vec) const;
		Vec2 operator/(const Vec2 & _vec) const;

		Vec2 operator*(float _v) const ;
		Vec2 operator/(float _v) const ;

		bool operator==(const Vec2 & _vec) const;
		bool operator!=(const Vec2 & _vec) const;
	};

	bool CheckVec2(const Vec2 & _vec);

	float Length(const Vec2 & _vec);
	float LengthSqr(const Vec2 & _vec);
	float Dot(const Vec2 & _v1, const Vec2 & _v2);
	float Distance(const Vec2 & _v1, const Vec2 & _v2);

	Vec2 operator*(float _v, const Vec2 & _vec);

	Vec2 Normalize(const Vec2 & _vec);
	Vec2 Max(const Vec2 & _v1, const Vec2 & _v2);
	Vec2 Min(const Vec2 & _v1, const Vec2 & _v2);
	Vec2 Abs(const Vec2 & _vec);
	Vec2 Lerp(const Vec2 & _v1, const Vec2 & _v2, float _v);
	Vec2 Clamp(const Vec2 & _vec, const Vec2 & _min, const Vec2 & _max);
	Vec2 Saturate(const Vec2 & _vec);
	Vec2 Perpendicular(const Vec2 & _vec);
	Vec2 UnitCircle(float _angle);
	Vec2 Hermite(const Vec2 & _v1, const Vec2 & _tan1, const Vec2 & _v2, const Vec2 & _tan2, float _val);

	inline std::ostream & operator<<(std::ostream & os, const Vec2 & _vec) {
		return os << "[Vec2 : x = " << _vec.x <<
			", y = " << _vec.y << "]";
	}

	#include "Vec2.inl"
}
