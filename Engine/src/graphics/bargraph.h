#pragma once

#include "math/vec2i.h"
#include "math/vec2.h"

namespace prev {

	class BarGraph : public HandledObject<BarGraph> {
	public:
		BarGraph(float minValue = 0.0f, float maxValue = 1.0f, Vec2i dimen = Vec2i(200), int numValuesMax = 32, unsigned int valueToHighlight = -1);

		void Draw(Vec2 pos);
		void DrawBackground(Vec2 pos);
		void PushValue(float val);
		void PopValue();
		void Clear();

		void SetDimension(Vec2i dimen) {}
		void SetMinVal(float minVal) { m_MinValue = minVal; }
		void SetMaxVal(float maxVal) { m_MaxValue = maxVal; }
		void SetValueToHighlight(int valueToHighlight) { m_ValueToHightlight = valueToHighlight; }
		void SetValue(float val, unsigned int index);

		Vec2i GetDimension() const { return m_Dimension; }
	private:
		float m_MinValue;
		float m_MaxValue;
		Vec2i m_Dimension;
		int m_NumMaxValue;
		int m_ValueToHightlight;
		std::vector<float> m_Vaues;
	};

}
