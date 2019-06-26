#pragma once

#include "prevmath.h"
#include "common/assert.h"

#include "gm/gmMathVector.h"

namespace prev {

	class Vec2i {
	public:
		union {
			struct { int x, y; };
			struct { int u, v; };
			gmvec2i gmv;
		};

		Vec2i();
		explicit Vec2i(int _v);
		Vec2i(gmvec2i _vec);
		Vec2i(int _x, int _y);

		int & operator[](unsigned int _index) { ASSERT(_index < 2); return (&x)[_index]; }
		const int & operator[](unsigned int _index) const { ASSERT(_index < 2); return (&x)[_index]; }

		Vec2i & operator+=(const Vec2i & _vec);
		Vec2i & operator-=(const Vec2i & _vec);
		Vec2i & operator*=(const Vec2i & _vec);
		Vec2i & operator/=(const Vec2i & _vec);

		Vec2i & operator+=(int _v);
		Vec2i & operator-=(int _v);
		Vec2i & operator*=(int _v);
		Vec2i & operator/=(int _v);

		Vec2i operator+(const Vec2i & _vec) const;
		Vec2i operator-(const Vec2i & _vec) const;
		Vec2i operator-() const;
		Vec2i operator*(const Vec2i & _vec) const;
		Vec2i operator/(const Vec2i & _vec) const;

		Vec2i operator*(int _v) const ;
		Vec2i operator/(int _v) const ;

		bool operator==(const Vec2i & _vec) const;
		bool operator!=(const Vec2i & _vec) const;
	};

	float Length(const Vec2i & _vec);
	int   LengthSqr(const Vec2i & _vec);
	int   Dot(const Vec2i & _v1, const Vec2i & _v2);
	float Distance(const Vec2i & _v1, const Vec2i & _v2);

	Vec2i operator*(int _v, const Vec2i & _vec);

	Vec2i Max(const Vec2i & _v1, const Vec2i & _v2);
	Vec2i Min(const Vec2i & _v1, const Vec2i & _v2);
	Vec2i Abs(const Vec2i & _vec);
	Vec2i Lerp(const Vec2i & _v1, const Vec2i & _v2, float _v);
	Vec2i Clamp(const Vec2i & _vec, const Vec2i & _min, const Vec2i & _max);
	Vec2i Saturate(const Vec2i & _vec);
	Vec2i Perpendicular(const Vec2i & _vec);

	inline std::ostream & operator<<(std::ostream & os, const Vec2i & _vec) {
		return os << "[Vec2i : x = " << _vec.x <<
			", y = " << _vec.y << "]";
	}

	#include "vec2i.inl"
}
