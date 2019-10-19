#include "pch.h"
#include "linegraph.h"

#include "immediategfx.h"
#include "math/mvp.h"
#include "math/vecconversion.h"

#include "imgui.h"

namespace prev {

	static float ImGuiGetData(void * data, int index) {
		return (reinterpret_cast<LineGraph *>(data)->GetData(index));
	}

	LineGraph::LineGraph(float minValue /*= 0.0f*/, float maxValue /*= 1.0f*/, Vec2 dimen /*= Vec2(1.0f)*/, int numValuesMax /*= 128*/) {
		m_IndexFront = 0;
		m_NumMaxValue = numValuesMax;
		m_MinValue = minValue;
		m_MaxValue = maxValue;
		m_Dimension = dimen;

		ASSERT(numValuesMax > 0);
		m_Vaues.reserve(numValuesMax);
	}

	void LineGraph::PushValue(float val) {
		if ((int)m_Vaues.size() >= m_NumMaxValue) {
			m_Vaues[m_IndexFront++] = val;
			m_IndexFront %= m_NumMaxValue;
		} else {
			m_Vaues.push_back(val);
		}
	}

	void LineGraph::PopValue() {
		if (!m_Vaues.empty()) m_Vaues.pop_back();
	}

	void LineGraph::Clear() {
		m_IndexFront = 0;
		m_Vaues.clear();
	}

	float LineGraph::GetData(int index) {
		index += m_IndexFront;
		index %= m_NumMaxValue;

		if (index >= m_Vaues.size()) return 0;

		return m_Vaues[index];
	}

	void LineGraph::DrawImGui() {
		ImGui::PlotLines("", ImGuiGetData, this, m_NumMaxValue, 0, (const char *)0, m_MinValue, m_MaxValue, m_Dimension);
	}

}