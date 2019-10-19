#pragma once

#include "vec2.h"

namespace prev {

	class Spline : public HandledObject<Spline> {
	public:
		Spline(Vec2 center);
		~Spline();

		void AddSegment(Vec2 anchorPos);
		void MovePoint(int index, Vec2 newPos);

		std::vector<Vec2> GetPointsInSegment(unsigned int index);
		std::vector<Vec2> GetControlPointsOfSegment(unsigned int index);
		std::vector<Vec2> GetAnchorPointsOfSegment(unsigned int index);

		Vec2 GetValue(float factor);

		inline unsigned int GetNumSegments() { return ((unsigned int)m_Points.size() - 4) / 3 + 1; }
		inline unsigned int GetNumPoints() { return (unsigned int)m_Points.size(); }
	public:
		Vec2 & operator[](unsigned int index) { return m_Points[index]; }
	private:
		std::vector<Vec2> m_Points;
	};

}
