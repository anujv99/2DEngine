#include "pch.h"
#include "bargraph.h"

#include "immediategfx.h"
#include "math/vecconversion.h"

namespace prev {

	BarGraph::BarGraph(float minValue /*= 0.0f*/, float maxValue /*= 1.0f*/, Vec2i dimen /*= Vec2i(200)*/, 
		int numValuesMax /*= 32*/, unsigned int valueToHighlight /*= -1*/) {
		m_NumMaxValue = numValuesMax;
		m_MinValue = minValue;
		m_MaxValue = maxValue;
		m_Dimension = dimen;
		m_ValueToHightlight = valueToHighlight;

		ASSERT(numValuesMax > 0);
		m_Vaues.reserve(numValuesMax);
	}

	void BarGraph::Draw(Vec2 pos) {
		if (m_Vaues.size() == 0) return;

		auto & imm = ImmediateGFX::Ref();

		const Vec2i dimen = m_Dimension;

		const float inverseRange = 1.0f / (m_MaxValue - m_MinValue);
		float deltaX = dimen.x / (float)m_Vaues.size();

		for (int i = 0; i < m_Vaues.size(); ++i) {
			float currVal = m_Vaues[i];
			float percent = Saturate((currVal - m_MinValue) * inverseRange);

			float x0 = (float)pos.x + deltaX * i;
			float x1 = x0 + deltaX;
			float y0 = (float)pos.y - dimen.y;
			float y1 = y0 + percent * dimen.y;

			Vec2 barPos = Vec2(x0, y0);
			Vec2 barDimen = Vec2(x1 - x0, y1 - y0);

			imm.Color(Vec3(0.0f, 0.8f, 0.9f), 0.4f);
			if (i == m_ValueToHightlight) {
				imm.Color(Vec3(1.0f), 0.2f);
				imm.DrawRectWire(barPos, Vec2(barDimen.x, dimen.y));
				imm.Color(Vec3(1.0f, 1.0f, 0.0f), 0.6f);
			}

			imm.DrawRect(barPos + Vec2(1.0f, 0.0f), barDimen - Vec2(1.0f, 0.0f));
		}

	}

	void BarGraph::DrawBackground(Vec2 pos) {
		const Vec2 minPos = pos + Vec2(0.0f, (float)-m_Dimension.y);
		const Vec2 dimen = ToVec2(m_Dimension);;
		const Vec2 maxPos = minPos + dimen;

		ImmediateGFX & imm = ImmediateGFX::Ref();

		imm.BeginDefaultShaders();
		imm.BeginDraw();

		imm.Color(Vec3(0.2f, 0.2f, 0.2f), 0.8f);
		imm.DrawRect(minPos, dimen);

		imm.Color(Vec3(1.0f), 0.1f);
		imm.PolygonBegin(PV_PRIM_LINELIST);

		int numLines = 3;
		for (int i = 0; i <= numLines + 1; ++i) {
			float alpha = (float)i / (numLines + 1);

			float x = Lerp(minPos.x, maxPos.x, alpha);
			float y = Lerp(minPos.y, maxPos.y, alpha);

			imm.Vertex(Vec2(x, minPos.y));
			imm.Vertex(Vec2(x, maxPos.y));
			imm.Vertex(Vec2(minPos.x, y));
			imm.Vertex(Vec2(maxPos.x, y));
		}

		imm.PolygonEnd();

		imm.EndDraw();
	}

	void BarGraph::PushValue(float val) {
		if ((int)m_Vaues.size() >= m_NumMaxValue) {
			LOG_ERROR("Bar graph number of values exeeded");
		} else {
			m_Vaues.push_back(val);
		}
	}

	void BarGraph::PopValue() {
		if (!m_Vaues.empty()) m_Vaues.pop_back();
	}

	void BarGraph::Clear() {
		m_Vaues.clear();
	}

	void BarGraph::SetValue(float val, unsigned int index) {
		ASSERT(index < m_Vaues.size());

		m_Vaues[index] = val;
	}

}