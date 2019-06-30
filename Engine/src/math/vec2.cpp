#include "pch.h"
#include "vec2.h"

#include <Box2D/Common/b2Math.h>

namespace prev {

	Vec2::Vec2(b2Vec2 _vec) : x(_vec.x), y(_vec.y) {}

}