#include "pch.h"
#include "square.h"

namespace prev {

	Square::Square() : m_Position(0.0f), m_Dimension(1.0f), m_Rotation(0.0f), m_Color(Vec4(1.0f)), m_Texture(nullptr), m_Body(nullptr) {}

	void Square::MakeKinematic(b2World * world, b2BodyType type) {
		if (m_Body != nullptr) return;

		b2BodyDef def;
		def.position = m_Position.xy();
		def.angle = ToRadians(m_Rotation);
		def.userData = (void *)this;
		def.type = type;

		b2PolygonShape shape;
		shape.SetAsBox(m_Dimension.x / 2.0f, m_Dimension.y / 2.0f);

		b2FixtureDef fdef;
		fdef.density = 1.0f;
		fdef.friction = 0.3f;
		fdef.shape = &shape;

		m_Body = world->CreateBody(&def);
		m_Body->CreateFixture(&fdef);
	}

	Vec3 Square::GetPosition() const {
		if (m_Body) {
			return Vec3(m_Body->GetPosition(), m_Position.z);
		}
		return m_Position;
	}

	Vec2 Square::GetDimension() const {
		return m_Dimension;
	}

	float Square::GetRotation() const {
		if (m_Body) {
			return ToDegrees(m_Body->GetAngle());
		}
		return m_Rotation;
	}

	SpriteColor Square::GetColor() const {
		return m_Color;
	}

	void Square::SetPosition(Vec3 pos) {
		if (m_Body) {
			m_Body->SetTransform(pos.xy(), m_Body->GetAngle());
			m_Body->SetLinearVelocity(Vec2(0.0f));
			m_Body->SetAngularVelocity(0.0f);
		}
		m_Position = pos;
	}

	void Square::SetDimension(Vec2 dimen) {
		m_Dimension = dimen;
	}

	void Square::SetRotation(float angle) {
		m_Rotation = angle;
	}

	void Square::SetColor(Vec4 color) {
		m_Color = color;
	}

}