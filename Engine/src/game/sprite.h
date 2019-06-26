#pragma once

#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"

namespace prev {

	typedef unsigned int SpriteTextureID;

	struct SpriteColor {
		SpriteColor() : r(0), g(0), b(0), a(255) {}
		SpriteColor(Vec4 color) :
			r((unsigned char)(color.r * 255)),
			g((unsigned char)(color.g * 255)),
			b((unsigned char)(color.b * 255)),
			a((unsigned char)(color.a * 255)) {
		}
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	};

	struct Sprite {
		Vec2 Position;		// Center
		Vec2 Dimension;		// Width-Height
		float Rotation;		// Degrees
		SpriteColor Color;  // RGBA

		Vec2 Uvx;			// Texture Coordinated in x direction x => left, y => right
		Vec2 Uvy;			// Texture Coordinated in y direction x => bottom, y => top
	};

}
