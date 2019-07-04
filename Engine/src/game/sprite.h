#pragma once

#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"

#include "Box2D/Box2D.h"

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
	private:
		Vec2 m_Position;		// Center
		Vec2 m_Dimension;		// Width-Height
		float m_Rotation;		// Degrees
		SpriteColor m_Color;	// RGBA

		Vec2 m_Uvx;			// Texture Coordinated in x direction x => left, y => right
		Vec2 m_Uvy;			// Texture Coordinated in y direction x => bottom, y => top

		b2Body * m_Body = nullptr;
	public:
		Vec2 GetPosition() const;
		Vec2 GetDimension() const;
		float GetRotation() const;

		void SetPosition(Vec2 pos);
		void SetDimension(Vec2 dimen);
		void SetRotation(float rot);

		inline void SetColor(SpriteColor color) { m_Color = color; }
		inline SpriteColor GetColor() const { return m_Color; }
		inline Vec2 GetUvx() const { return m_Uvx; }
		inline Vec2 GetUvy() const { return m_Uvy; }
		inline b2Body * GetBody() { return m_Body; }

		void MakeKinematic(b2World * world, b2BodyType type);
		bool IsPosInBox(Vec2 pos);
	};

}
