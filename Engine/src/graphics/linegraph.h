#pragma once

#include "math/vec2i.h"
#include "math/vec2.h"

namespace prev {

	class LineGraph : public HandledObject<LineGraph> {
	public:
		LineGraph(float minValue = 0.0f, float maxValue = 1.0f, Vec2i dimen = Vec2i(200), int numValuesMax = 128);

		void Draw(Vec2 pos);
		void DrawBackground(Vec2 pos);
		void PushValue(float val);
		void PopValue();
		void Clear();
		
		void SetDimension(Vec2i dimen) {  }
		void SetMinVal(float minVal) { m_MinValue = minVal; }
		void SetMaxVal(float maxVal) { m_MaxValue = maxVal; }

		int GetMaxNumValues() const { return m_NumMaxValue; }

		Vec2i GetDimension() const { return m_Dimension; }

		//Use this to get imgui PlotLine function
		float GetData(int index);

		void DrawImGui();
	private:
		float m_MinValue;
		float m_MaxValue;
		Vec2i m_Dimension;
		int m_NumMaxValue;
		int m_IndexFront;
		std::vector<float> m_Vaues;
	};

}