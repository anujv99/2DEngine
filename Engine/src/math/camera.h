#pragma once

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"

namespace prev {

	class Camera : public HandledObject<Camera> {
	public:
		Camera(float left, float right, float top, float bottom);
		Camera(float left, float right, float top, float bottom, float nearPlane, float farPlane);

		void Begin();
		void End();

		void SetPosition(Vec2 pos) { m_Position = -pos; RecalculateViewMatrix(); }
		inline Vec2 GetPosition() { return -m_Position; }

		void SetRotation(float rot) { m_Rotation = rot; RecalculateViewMatrix(); }
		inline float GetRotation() { return m_Rotation; }
	private:
		void RecalculateViewMatrix();
	private:
		Mat4 m_OrthoMatrix;
		Mat4 m_ViewMatrix;

		Vec2 m_Position;
		float m_Rotation;
	};

}
