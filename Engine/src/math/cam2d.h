#pragma once

#include "mvp.h"

namespace prev {

	class Cam2D : public HandledObject<Cam2D> {
	public:
		void Begin();
		void End();

		void SetScreenSpace(Vec2 bottomLeft, Vec2 topRight);
		void SetNearFar(float nearfloat, float farfloat);

		inline Vec2 GetBottomLeft() const { return m_BottomLeft; }
		inline Vec2 GetTopRight() const { return m_TopRight; }

		inline Vec2 GetVirtualScale() const { return m_TopRight - m_BottomLeft; }
		inline Vec2 GetInverseVirtualScale() const { return Vec2(1.0f) / (m_TopRight - m_BottomLeft); }

		void UpdateMatrix();
		Vec2 GetScreenMapRatio();

		//Only works with raw mouse pos i.e. TopLeft = (0.0f, 0.0f) BottomRight = WindowSize
		Vec2 MapMouseCoords(Vec2 mouseCoords) const;
		//Only works with raw mouse pos i.e. TopLeft = (0, 0) BottomRight = WindowSize
		Vec2 MapMouseCoords(Vec2i mouseCoords) const;
	public:
		void SetPos(Vec2 pos);
	private:
		Vec2 m_BottomLeft;
		Vec2 m_TopRight;

		Vec2 m_NearFar = Vec2(-10.0f, 10.0f);

		Vec2 m_Position = Vec2(0.0f);
	};

}
