#pragma once

#include "vec2.h"

namespace prev {

	class CubicSpline2D : public HandledObject<CubicSpline2D> {
	public:
		Vec2 CalculatePositionAt(float t) const;

		int NumControlPoints() const;
		const Vec2 GetControlPoint(int i) const;
		void AddControlPoint(Vec2 point);
		void SetControlPoint(int i, Vec2 val);
		void RemoveControlPoint();
		void SortControlPointsByX();
		void SortControlPointsByY();
	private:
		float GetAlphaAt(int i) const;
		Vec2 GetTangentAt(int i) const;
	private:
		std::vector<Vec2> m_ControlPoints;
	};

}