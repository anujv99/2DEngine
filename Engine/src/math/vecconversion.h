#pragma once

#include "vec2i.h"
#include "vec2.h"

namespace prev {

	inline Vec2 ToVec2(const Vec2i & _vec) {
		return Vec2((float)_vec.x, (float)_vec.y);
	}

	inline Vec2i ToVec2i(const Vec2 & _vec) {
		return Vec2i((int)_vec.x, (int)_vec.y);
	}

}