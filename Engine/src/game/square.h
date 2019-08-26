#pragma once

#include "drawable.h"

#include <Box2D/Box2D.h>

namespace prev {

	class Square : public Drawable {
	public:
		Square();

		void MakeKinematic(b2World * world, b2BodyType type);

		virtual Vec3 GetPosition() const override;
		virtual Vec2 GetDimension() const override;
		virtual float GetRotation() const override;
		virtual SpriteColor GetColor() const override;

		void SetPosition(Vec3 pos);
		void SetDimension(Vec2 dimen);
		void SetRotation(float angle);
		void SetColor(Vec4 color);

		inline b2Body * GetBody() { return m_Body; }
	private:
		Vec3 m_Position;
		Vec2 m_Dimension;
		float m_Rotation;
		SpriteColor m_Color;
		StrongHandle<Texture2D> m_Texture;

		b2Body * m_Body;
	};

}
