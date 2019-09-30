#include "pch.h"
#include "camera.h"

#include "mvp.h"

namespace prev {
	
	static const constexpr float NEAR_PLANE = -150.0f;
	static const constexpr float FAR_PLANE  =  150.0f;

	Camera::Camera(float left, float right, float top, float bottom) :
		m_OrthoMatrix(Identity()), m_ViewMatrix(Identity()), m_Position(0.0f), m_Rotation(0.0f) {
		m_OrthoMatrix = Ortho(left, right, bottom, top, NEAR_PLANE, FAR_PLANE);
	}

	Camera::~Camera() {}

	void Camera::SetProjection(float left, float right, float top, float bottom) {
		m_OrthoMatrix = Ortho(left, right, bottom, top, NEAR_PLANE, FAR_PLANE);
	}

	void Camera::Begin() {
		MVP::Ref().Projection().Push();
		MVP::Ref().Projection().Load(m_OrthoMatrix);

		MVP::Ref().View().Push();
		MVP::Ref().View().Load(m_ViewMatrix);
	}

	void Camera::End() {
		MVP::Ref().Projection().Pop();
		MVP::Ref().View().Pop();
	}

	void Camera::RecalculateViewMatrix() {
		m_ViewMatrix = Identity();
		m_ViewMatrix = Translate(m_ViewMatrix, m_Position);
		m_ViewMatrix = Rotate(m_ViewMatrix, m_Rotation, Vec3(0.0f, 0.0f, 1.0f));
	}

}