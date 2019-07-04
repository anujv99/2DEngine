#pragma once

#include "math/vec2.h"
#include "math/vec4.h"
#include "common/timer.h"

namespace prev {

	struct Particle {
		Vec2 Position;

		Vec2 Velocity;
		Vec2 Acceleration;

		Vec2 StartScale;
		Vec2 EndScale;
		Vec2 CurrentScale;

		Vec3 StartColor;
		Vec3 EndColor;
		Vec3 CurrentColor;

		float StartAlpha;
		float EndAlpha;
		float CurrentAlpha;

		float StartAngle;
		float RotationVelocity;

		float Rotation;

		float LifeLength;

		bool Update(float dt) {
			ElapsedTime += dt;
			if (ElapsedTime > LifeLength)
				return false;

			float frac = 1 - (LifeLength - ElapsedTime) / LifeLength;

			Velocity += Acceleration;
			Position += Velocity;

			CurrentScale = StartScale + ((EndScale - StartScale) * frac);
			CurrentColor = StartColor + ((EndColor - StartColor) * frac);
			CurrentAlpha = StartAlpha + ((EndAlpha - StartAlpha) * frac);

			return true;
		}
	private:
		float ElapsedTime = 0.0f;
	};

}
