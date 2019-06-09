#pragma once

#include "bindable.h"

namespace prev {

	enum BufferUsage {
		BUFFER_USAGE_STATIC,
		BUFFER_USAGE_DYNAMIC,
		BUFFER_USAGE_STREAM,
	};

	class VertexBuffer : public HandledObject<VertexBuffer>, public Bindable {
	protected:
		VertexBuffer() {}
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() override = 0;
		virtual void UnBind() override = 0;
		virtual void Init(const void * data, unsigned int numElements, unsigned int strideBytes, BufferUsage bufferType) = 0;
		virtual void SubData(const void * data, unsigned int numBytes, unsigned int byteOffset) = 0;

		virtual BindableType GetType() const override {
			return BindableType::VERTEX_BUFFER;
		}

		static StrongHandle<VertexBuffer> CreateVertexBuffer();
	};

}
