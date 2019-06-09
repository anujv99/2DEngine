#include "pch.h"
#include "vertexbatcher.h"

namespace prev {

	VertexBatcher::VertexBatcher(unsigned int reserveCount) {
		m_VertexBatch.reserve(reserveCount);
	}

	VertexBatcher::~VertexBatcher() {
	}

	void VertexBatcher::Vertex(Vec3 position) {
		ASSERT(m_VertexBatch.size() < m_VertexBatch.capacity());

		VertexBatcherVert vert;
		vert.Position = position;
		vert.Color = m_CurrentColor;

		m_VertexBatch.push_back(vert);
	}

	void VertexBatcher::Color(Vec3 color, float alpha /*= 1.0f*/) {
		m_CurrentColor.r = color.r;
		m_CurrentColor.g = color.g;
		m_CurrentColor.b = color.b;
		m_CurrentColor.a = alpha;
	}

	void VertexBatcher::DrawLine(const Vec2 start, const Vec2 end) {
		PolygonBegin(PV_PRIM_LINELIST);
		RenderState::Ref().SetPrimitiveTopology(PV_PRIM_LINELIST);
		Vertex(start);
		Vertex(end);
		PolygonEnd();
	}

	prev::StrongHandle<prev::VertexLayout> VertexBatcher::CreateVertexLayout(StrongHandle<VertexShader> vShader) const {
		StrongHandle<VertexLayout> vertexLayout = VertexLayout::CreateVertexLayout();

		vertexLayout->BeginEntries();
		vertexLayout->AddEntry(PV_DATA_TYPE_FLOAT_32, 3, offsetof(VertexBatcherVert, Position), "POSITION", false);
		vertexLayout->AddEntry(PV_DATA_TYPE_FLOAT_32, 4, offsetof(VertexBatcherVert, Color), "COLOR", false);
		vertexLayout->EndEntries(vShader);

		return vertexLayout;
	}

}