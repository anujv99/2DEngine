#pragma once

#include "common/stronghandle.h"

#include "graphics/vertexshader.h"
#include "graphics/pixelshader.h"

#include "math/vec2.h"
#include "math/vec4.h"

namespace prev {

	struct GameObject {
		GameObject() : Position(0.0f), Dimension(1.0f), Rotation(0.0f) {}
		GameObject(Vec3 pos, Vec2 dimen, float rot) : Position(pos), Dimension(dimen), Rotation(rot) {}

		Vec3 Position;
		Vec2 Dimension;
		float Rotation;
	};

	struct Color {
		Color() : Red(255u), Green(255u), Blue(255u), Alpha(255u) {}
		Color(Vec4 col) : 
			Red((unsigned char)(col.r * 255.0f) % 255u),
			Green((unsigned char)(col.g * 255.0f) % 255u),
			Blue((unsigned char)(col.b * 255.0f) % 255u),
			Alpha((unsigned char)(col.a * 255.0f) % 255u) {
		}

		unsigned char Red;
		unsigned char Green;
		unsigned char Blue;
		unsigned char Alpha;
	};

	struct Renderable {
		Renderable(StrongHandle<VertexShader> vShader = nullptr, StrongHandle<PixelShader> pShader = nullptr) :
			VertShader(vShader), PixShader(pShader) {
		}

		StrongHandle<VertexShader> VertShader;
		StrongHandle<PixelShader> PixShader;
	};

}
