#include "pch.h"
#include "cam2d.h"

namespace prev {

	void Cam2D::Begin() {
		MVP::Ref().Projection().Push();
		MVP::Ref().Projection().Load(Ortho(m_BottomLeft.x, m_TopRight.x, m_BottomLeft.y, m_TopRight.y, m_NearFar.x, m_NearFar.y));
	}

	void Cam2D::End() {
		MVP::Ref().Projection().Pop();
	}

	void Cam2D::SetScreenSpace(Vec2 bottomLeft, Vec2 topRight) {
		m_BottomLeft = bottomLeft;
		m_TopRight = topRight;
	}

}