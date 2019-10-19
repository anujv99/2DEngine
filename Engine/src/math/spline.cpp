#include "pch.h"
#include "spline.h"

namespace prev {

	static const float START_OFFSET = 0.4f;

	Spline::Spline(Vec2 center) {
		m_Points.push_back(center + Vec2(-START_OFFSET, 0.0f));
		m_Points.push_back(center + Vec2(-START_OFFSET, START_OFFSET) * 0.5f);
		m_Points.push_back(center + Vec2(START_OFFSET, -START_OFFSET) * 0.5f);
		m_Points.push_back(center + Vec2(+START_OFFSET, 0.0f));
	}

	Spline::~Spline() {}

	void Spline::AddSegment(Vec2 anchorPos) {
		m_Points.push_back(m_Points[m_Points.size() - 1] * 2 - m_Points[m_Points.size() - 2]);
		m_Points.push_back((m_Points[m_Points.size() - 1] + anchorPos) * 0.5f);
		m_Points.push_back(anchorPos);
	}

	std::vector<Vec2> Spline::GetPointsInSegment(unsigned int index) {
		return std::vector<Vec2>(m_Points.begin() + (index * 3ull), m_Points.begin() + (index * 3ull) + 3ull);
	}

	std::vector<Vec2> Spline::GetControlPointsOfSegment(unsigned int index) {
		size_t i = (size_t)index;
		std::vector<Vec2> controls;
		controls.push_back(m_Points[i * 3ull + 1ull]);
		controls.push_back(m_Points[i * 3ull + 2ull]);
		return controls;
	}

	std::vector<prev::Vec2> Spline::GetAnchorPointsOfSegment(unsigned int index) {
		size_t i = (size_t)index;
		std::vector<Vec2> anchor;
		anchor.push_back(m_Points[i * 3ull + 0ull]);
		anchor.push_back(m_Points[i * 3ull + 3ull]);
		return anchor;
	}

	prev::Vec2 Spline::GetValue(float factor) {
		unsigned int segmentNum = 0;
		float bias = 0.0f;
		Clamp(factor, 0.0f, 1.0f);
		factor = GetNumSegments() * factor;
		segmentNum = (unsigned int)factor;
		if (segmentNum == GetNumSegments()) {
			segmentNum = segmentNum - 1;
			bias = 1.0f;
		}
		auto control = GetControlPointsOfSegment(segmentNum);
		auto anchor = GetAnchorPointsOfSegment(segmentNum);
		return Bezier(anchor[0], anchor[1], control[0], control[1], (factor - (unsigned int)factor) + bias);
	}

	void Spline::MovePoint(int index, Vec2 newPos) {
		Vec2 deltaMove = newPos - m_Points[index];
		m_Points[index] = newPos;

		if (index % 3 == 0) {
			if (index + 1 < m_Points.size()) {
				m_Points[index + 1] += deltaMove;
			}
			if (index - 1 >= 0) {
				m_Points[index - 1] += deltaMove;
			}
		} else {
			bool nextPointIsAnchor = (index + 1) % 3 == 0;
			int correspondingControlIndex = (nextPointIsAnchor) ? index + 2 : index - 2;
			int anchorIndex = (nextPointIsAnchor) ? index + 1 : index - 1;
			if (correspondingControlIndex >= 0 && correspondingControlIndex < m_Points.size()) {
				float dst = Length(m_Points[anchorIndex] - m_Points[correspondingControlIndex]);
				Vec2 dir = Normalize(m_Points[anchorIndex] - newPos);
				m_Points[correspondingControlIndex] = m_Points[anchorIndex] + dir * dst;
			}
		}
	}

}