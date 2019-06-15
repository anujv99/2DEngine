#include "pch.h"
#include "cubicspline2d.h"

#include "prevmath.h"

namespace prev {

	prev::Vec2 CubicSpline2D::CalculatePositionAt(float t) const {
		if (m_ControlPoints.size() < 2) {
			return Vec2(-1.0f);
		}

		// clamp t-value
		t = Saturate(t);

		float dt = 1.0f / (m_ControlPoints.size() - 1);
		int index0 = (int)(t / dt);
		int index1 = index0 + 1;

		const float lerpFactor = fmod(t, dt) / dt;
		const Vec2 tan0 = GetTangentAt(index0);
		const Vec2 tan1 = GetTangentAt(index1);
		const Vec2 pt0 = GetControlPoint(index0);
		const Vec2 pt1 = GetControlPoint(index1);

		float tOne = lerpFactor;
		float tSqr = lerpFactor * lerpFactor;
		float tCube = lerpFactor * lerpFactor * lerpFactor;

		float t0 = 2.0f * tCube - 3.0f * tSqr + 1.0f;
		float t1 = tCube - 2.0f * tSqr + tOne;
		float t2 = -2.0f * tCube + 3.0f * tSqr;
		float t3 = tCube - tSqr;

		Vec2 result = t0 * pt0 + t1 * (pt1.x - pt0.x) * tan0 + t2 * pt1 + t3 * (pt1.x - pt0.x) * tan1;
		return result;
	}

	int CubicSpline2D::NumControlPoints() const {
		return (int)m_ControlPoints.size();
	}

	const prev::Vec2 CubicSpline2D::GetControlPoint(int i) const {
		ASSERT(i >= 0 && i < (int)m_ControlPoints.size());
		return m_ControlPoints[i];
	}

	void CubicSpline2D::AddControlPoint(Vec2 point) {
		m_ControlPoints.push_back(point);
	}

	void CubicSpline2D::SetControlPoint(int i, Vec2 val) {
		ASSERT(i >= 0 && i < (int)m_ControlPoints.size());
		m_ControlPoints[i] = val;
	}

	void CubicSpline2D::RemoveControlPoint() {
		if (m_ControlPoints.empty()) return;
		m_ControlPoints.pop_back();
	}

	void CubicSpline2D::SortControlPointsByX() {
		for (size_t i = 0; i < m_ControlPoints.size() - 1; ++i) {
			for (size_t j = i + 1; j < m_ControlPoints.size(); ++j) {
				if (m_ControlPoints[j].x < m_ControlPoints[i].x) {
					Vec2 temp = m_ControlPoints[i];
					m_ControlPoints[i] = m_ControlPoints[j];
					m_ControlPoints[j] = temp;
				}
			}
		}
	}

	void CubicSpline2D::SortControlPointsByY() {
		for (size_t i = 0; i < m_ControlPoints.size() - 1; ++i) {
			for (size_t j = i + 1; j < m_ControlPoints.size(); ++j) {
				if (m_ControlPoints[j].y < m_ControlPoints[i].y) {
					Vec2 temp = m_ControlPoints[i];
					m_ControlPoints[i] = m_ControlPoints[j];
					m_ControlPoints[j] = temp;
				}
			}
		}
	}

	float CubicSpline2D::GetAlphaAt(int i) const {
		return float(i) / (m_ControlPoints.size() - 1);
	}

	prev::Vec2 CubicSpline2D::GetTangentAt(int i) const {
		if (i == 0) return (m_ControlPoints[i + 1] - m_ControlPoints[i]);
		if (i == m_ControlPoints.size() - 1) return (m_ControlPoints[i] - m_ControlPoints[i - 1]);

		float alphaCurr = GetAlphaAt(i);
		float alphaPrev = GetAlphaAt(i - 1);
		float alphaNext = GetAlphaAt(i + 1);

		Vec2 ptCurr = GetControlPoint(i);
		Vec2 ptPrev = GetControlPoint(i - 1);
		Vec2 ptNext = GetControlPoint(i + 1);

		Vec2 tangent = (ptNext - ptCurr) / (2.0f * (alphaNext - alphaCurr)) + (ptCurr - ptPrev) / (2.0f * (alphaCurr - alphaPrev));
		//v2 tangent = (ptNext-ptPrev)/(alphaNext-alphaPrev);

		return tangent;
	}

}