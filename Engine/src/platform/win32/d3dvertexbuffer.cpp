#include "pch.h"
#include "d3dvertexbuffer.h"

unsigned int GLOBAL_DRAW_CALL_COUNT = 0;

namespace prev {

	StrongHandle<VertexBuffer> VertexBuffer::CreateVertexBuffer() {
		return new D3DVertexBuffer();
	}

	D3DVertexBuffer::D3DVertexBuffer() :
		m_StrideBytes(0),
		m_NumElements(0),
		m_TotalBytes(0),
		m_BufferUsage(D3D11_USAGE_DEFAULT),
		m_IsInitialized(false)
	{}

	void D3DVertexBuffer::Init(const void * data, unsigned int numElements, unsigned int strideBytes, BufferUsage bufferType) {
		LOG_ON_CONDITION(!m_IsInitialized, LOG_ERROR, "Buffer already initialized", return);

		ASSERT(numElements > 0);
		ASSERT(strideBytes > 0);

		m_StrideBytes = strideBytes;
		m_NumElements = numElements;
		m_TotalBytes = m_StrideBytes * m_NumElements;

		switch (bufferType) {
		case BufferUsage::BUFFER_USAGE_STATIC:
			m_BufferUsage = D3D11_USAGE_DYNAMIC;
			break;
		case BufferUsage::BUFFER_USAGE_DYNAMIC:
			m_BufferUsage = D3D11_USAGE_DYNAMIC;
			break;
		case BufferUsage::BUFFER_USAGE_STREAM:
			m_BufferUsage = D3D11_USAGE_DYNAMIC;
			break;

		default:
		{
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Invalid buffer type");
			break;
		}
		}

		CreateBuffer(data);

		m_IsInitialized = true;
	}

	void D3DVertexBuffer::CreateBuffer(const void * data) {
		D3D11_BUFFER_DESC vbd;
		vbd.ByteWidth				= m_TotalBytes;
		vbd.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags			= D3D11_CPU_ACCESS_WRITE;
		vbd.MiscFlags				= 0;
		vbd.StructureByteStride		= m_StrideBytes;
		vbd.Usage					= m_BufferUsage;

		HRESULT hr = S_OK;

		if (data != nullptr) {
			D3D11_SUBRESOURCE_DATA vbsd;
			vbsd.pSysMem = data;
			vbsd.SysMemPitch = 0;
			vbsd.SysMemSlicePitch = 0;

			hr = GetDevice()->CreateBuffer(&vbd, &vbsd, m_Buffer.GetAddressOf());
		} else {
			hr = GetDevice()->CreateBuffer(&vbd, nullptr, m_Buffer.GetAddressOf());
		}

		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to create vertex buffer");
			return;
		}
	}

	void D3DVertexBuffer::Draw(unsigned int numVertices, unsigned int vertexOffset) {
		GLOBAL_DRAW_CALL_COUNT++;
		GetDeviceContext()->Draw(numVertices, vertexOffset);
	}

	void D3DVertexBuffer::Bind() {
		LOG_ON_CONDITION(m_IsInitialized, LOG_ERROR, "Buffer not initialized", return)

		const static UINT OFFSET = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, m_Buffer.GetAddressOf(), (UINT *)&m_StrideBytes, &OFFSET);
	}

	void D3DVertexBuffer::UnBind() {
	}

	void D3DVertexBuffer::SubData(const void * data, unsigned int numBytes, unsigned int byteOffset) {
		ASSERT(m_BufferUsage == D3D11_USAGE_DYNAMIC || m_BufferUsage == D3D11_USAGE_DEFAULT);

		D3D11_MAPPED_SUBRESOURCE msr;

		GetDeviceContext()->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		memcpy_s((byte *)msr.pData + byteOffset, m_TotalBytes, data, numBytes);
		GetDeviceContext()->Unmap(m_Buffer.Get(), 0);
	}

}
