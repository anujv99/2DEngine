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

	LineGraph::LineGraph(float minValue /*= 0.0f*/, float maxValue /*= 1.0f*/, Vec2i dimen /*= Vec2i(200)*/, int numValuesMax /*= 128*/) {
		m_IndexFront = 0;
		m_NumMaxValue = numValuesMax;
		m_MinValue = minValue;
		m_MaxValue = maxValue;
		m_Dimension = dimen;

		ASSERT(numValuesMax > 0);
		m_Vaues.reserve(numValuesMax);
	}

	void LineGraph::Draw(Vec2 pos) {
		if (m_Vaues.empty()) return;

		const Vec2 minPos = pos + Vec2(0.0f, (float)-m_Dimension.y);
		const Vec2 maxPos = minPos + ToVec2(m_Dimension);
		const float deltaX = 1.0f / m_NumMaxValue;
		const float denom = (m_MaxValue - m_MinValue);

		ImmediateGFX & imm = ImmediateGFX::Ref();
		imm.Color(1.0f, 1.0f, 0.0f);

		ScissorBox sb;
		sb.Left = minPos.x;
		sb.Right = maxPos.x;
		sb.Bottom = minPos.y;
		sb.Top = maxPos.y;

		RenderState::Ref().SetScissorBox(sb);

		imm.BeginDefaultShaders();
		imm.BeginDraw();
		imm.PolygonBegin(PV_PRIM_LINESTRIP);

		for (int i = m_IndexFront; i < (int)m_Vaues.size(); ++i) {
			float u = (i - m_IndexFront) * deltaX;
			float v = (m_Vaues[i] - m_MinValue) / denom;

			float x = Lerp(minPos.x, maxPos.x, u);
			float y = Lerp(minPos.y, maxPos.y, v);

			imm.Vertex(x, y);
		}

		if (m_IndexFront > 0) {
			const int offset = (int)m_Vaues.size() - m_IndexFront;

			for (int i = 0; i < m_IndexFront; ++i) {
				float u = (i + offset) * deltaX;
				float v = (m_Vaues[i] - m_MinValue) / denom;

				float x = Lerp(minPos.x, maxPos.x, u);
				float y = Lerp(minPos.y, maxPos.y, v);

				imm.Vertex(x, y);
			}
		}

		imm.PolygonEnd();

		RenderState::Ref().DisableScissors();
	}

	void LineGraph::DrawBackground(Vec2 pos) {

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