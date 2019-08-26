#pragma once

#include "math/vec3.h"
#include "math/vec2.h"

#include "graphics/texture2d.h"
#include "common/handledobject.h"

#include "sprite.h"

namespace prev {
	
	class Drawable : public HandledObject<Drawable> {
	public:
		virtual Vec3 GetPosition() const = 0;
		virtual Vec2 GetDimension() const = 0;
		virtual float GetRotation() const = 0;
		virtual SpriteColor GetColor() const = 0;
		virtual StrongHandle<Texture2D> GetTexture() const { return nullptr; }
	};

}
