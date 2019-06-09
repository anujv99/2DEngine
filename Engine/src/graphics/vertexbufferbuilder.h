#pragma once

#include "vertexbatcher.h"
#include "vertexbuffer.h"

namespace prev {

	class VertexBufferBuilder : public VertexBatcher, public HandledObject<VertexBufferBuilder> {
	public:
		VertexBufferBuilder(int maxVertices);
		~VertexBufferBuilder();

		void Clear();
		StrongHandle<VertexBuffer> CreateVertexBuffer();
	private:
		StrongHandle<VertexLayout> m_VertexLayout;
	};

}
