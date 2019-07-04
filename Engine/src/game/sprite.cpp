#include "pch.h"
#include "sprite.h"

namespace prev {

	prev::Vec2 Sprite::GetPosition() const {
		if (m_Body)
			return m_Body->GetPosition();
		return m_Position;
	}

	prev::Vec2 Sprite::GetDimension() const {
		return m_Dimension;
	}

	float Sprite::GetRotation() const {
		if (m_Body)
			return ToDegrees(m_Body->GetAngle());
		return m_Rotation;
	}

	void Sprite::SetPosition(Vec2 pos) {
		if (m_Body) {
			m_Body->SetTransform(pos, m_Body->GetAngle());
			m_Body->SetLinearVelocity(Vec2(0));
			m_Body->SetAngularVelocity(0);
			m_Body->SetAwake(true);
		} else
			m_Position = pos;
	}

	void Sprite::SetDimension(Vec2 dimen) {
		m_Dimension = dimen;
	}

	void Sprite::SetRotation(float rot) {
		if (m_Body)
			m_Body->SetTransform(m_Body->GetPosition(), ToRadians(rot));
		else
			m_Rotation = rot;
	}

	void Sprite::MakeKinematic(b2World * world, b2BodyType type) {
		ASSERT(world != nullptr);
		ASSERT(m_Body == nullptr);

		b2BodyDef def;
		def.type = type;
		def.angle = ToRadians(m_Rotation);
		def.position = m_Position;

		m_Body = world->CreateBody(&def);

		b2PolygonShape shape;
		shape.SetAsBox(m_Dimension.x / 2.0f, m_Dimension.y / 2.0f);

		m_Body->CreateFixture(&shape, 1.0f);
	}

	bool Sprite::IsPosInBox(Vec2 pos) {
		if (m_Body == nullptr) {
			float left = m_Position.x - m_Dimension.x / 2;
			float right = m_Position.x + m_Dimension.x / 2;
			float bottom = m_Position.y - m_Dimension.y / 2;
			float top = m_Position.y + m_Dimension.y / 2;

			return (pos.x >= left) && (pos.x <= right) && (pos.y >= bottom) && (pos.y <= top);
		} else {
			return m_Body->GetFixtureList()[0].TestPoint(pos);
		}
	}

}