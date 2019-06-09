#pragma once

#include "d3dhelper.h"

#include "graphics/vertexbuffer.h"

namespace prev {

	class D3DVertexBuffer : public VertexBuffer {
	public:
		D3DVertexBuffer();

		// Inherited via VertexBuffer
		virtual void Bind() override;
		virtual void UnBind() override;
		virtual void SubData(const void * data, unsigned int numBytes, unsigned int byteOffset) override;
		virtual void Init(const void * data, unsigned int numElements, unsigned int strideBytes, BufferUsage bufferType) override;
	private:
		void CreateBuffer(const void * data);
	private:
		UINT m_StrideBytes;
		UINT m_NumElements;
		UINT m_TotalBytes;
		D3D11_USAGE m_BufferUsage;
		bool m_IsInitialized;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
	};

}