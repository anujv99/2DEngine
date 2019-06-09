#include "pch.h"
#include "vertexbufferbuilder.h"

namespace prev {

	VertexBufferBuilder::VertexBufferBuilder(int maxVertices) : VertexBatcher(maxVertices) {
		m_VertexLayout = VertexLayout::CreateVertexLayout();
	}

	VertexBufferBuilder::~VertexBufferBuilder() {
	}

	void VertexBufferBuilder::Clear() {
		GetVertexBatch().clear();
	}

	prev::StrongHandle<prev::VertexBuffer> VertexBufferBuilder::CreateVertexBuffer() {
		StrongHandle<VertexBuffer> vbo = VertexBuffer::CreateVertexBuffer();

		auto & batch = GetVertexBatch();

		if (!batch.empty()) {
			vbo->Init(&batch.front(), batch.size(), GetNumBytesPerVertex(), BUFFER_USAGE_STATIC);
		}

		return vbo;
	}

}