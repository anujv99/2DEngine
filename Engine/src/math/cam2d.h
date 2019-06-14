#pragma once

#include "mvp.h"

namespace prev {

	class Cam2D : public HandledObject<Cam2D> {
	public:
		void Begin();
		void End();

		void SetScreenSpace(Vec2 bottomLeft, Vec2 topRight);
	private:
		Vec2 m_BottomLeft;
		Vec2 m_TopRight;

		Vec2 m_NearFar = Vec2(-10.0f, 10.0f);
	};

}
