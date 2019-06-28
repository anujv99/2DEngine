#pragma once

#include "math/vec2.h"
#include "math/vec4.h"
#include "common/timer.h"

namespace prev {

	struct Particle {
		Vec2 Position;
		Vec2 Velocity;
		Vec4 Color;
		float LifeLength;
		float Rotation;
		float Scale;
		float ElapsedTime = 0.0f;
	};

}
