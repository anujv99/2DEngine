#include "pch.h"
#include "cam2d.h"
#include "vecconversion.h"
#include "graphics/window.h"

namespace prev {

	void Cam2D::Begin() {
		MVP::Ref().Projection().Push();
		MVP::Ref().View().Push();
		UpdateMatrix();
	}

	void Cam2D::End() {
		MVP::Ref().View().Pop();
		MVP::Ref().Projection().Pop();
	}

	void Cam2D::SetScreenSpace(Vec2 bottomLeft, Vec2 topRight) {
		m_BottomLeft = bottomLeft;
		m_TopRight = topRight;
	}

	void Cam2D::SetNearFar(float nearfloat, float farfloat) {
		m_NearFar = Vec2(nearfloat, farfloat);
	}

	void Cam2D::UpdateMatrix() {
		MVP::Ref().Projection().Load(
			Ortho(
				m_BottomLeft.x + m_Position.x, m_TopRight.x + m_Position.x,
				m_BottomLeft.y + m_Position.y, m_TopRight.y + m_Position.y,
				m_NearFar.x, m_NearFar.y
			)
		);
		MVP::Ref().View().Load(m_ViewMatrix);
	}

	prev::Vec2 Cam2D::MapMouseCoords(Vec2 mouseCoords) const {
		//Mapping coordinates to (0, 1)
		const Vec2 winSize = ToVec2(Window::Ref().GetDisplayMode().GetWindowSize());
		Vec2 newMousePos = mouseCoords / winSize; //Assume TopLeft is (0, 0)

		//Invering Y axis;
		newMousePos.y = 1.0f - newMousePos.y;

		const float xScale = m_TopRight.x - m_BottomLeft.x;
		const float yScale = m_TopRight.y - m_BottomLeft.y;

		newMousePos *= Vec2(xScale, yScale);
		newMousePos += m_BottomLeft + m_Position;

		return newMousePos;
	}

	prev::Vec2 Cam2D::MapMouseCoords(Vec2i mouseCoords) const {
		return MapMouseCoords(ToVec2(mouseCoords));
	}

	void Cam2D::SetPos(Vec2 pos) {
		m_ViewMatrix = Translate(Identity(), -Vec3(pos.x, pos.y, 0.0f));
		UpdateMatrix();
	}

}