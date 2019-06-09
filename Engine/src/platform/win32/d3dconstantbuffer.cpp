#include "pch.h"
#include "d3dconstantbuffer.h"

namespace prev {

	StrongHandle<Uniform> Uniform::CreateUniform() {
		return new D3DConstantBuffer();
	}

	void D3DConstantBuffer::Bind() {
		LOG_ON_CONDITION(m_IsCreated, LOG_ERROR, "Constant Buffer already initialized", return);
		if (m_ShaderType == BindableType::VERTEX_SHADER)
			GetDeviceContext()->VSSetConstantBuffers(m_BufferSlot, 1, m_Buffer.GetAddressOf());
		else if (m_ShaderType == BindableType::PIXEL_SHADER)
			GetDeviceContext()->PSSetConstantBuffers(m_BufferSlot, 1, m_Buffer.GetAddressOf());
	}

	void D3DConstantBuffer::UnBind() {
	}

	void D3DConstantBuffer::Init(const void * data, unsigned int dataBytes, unsigned int uniformLocation, BindableType shaderType) {
		LOG_ON_CONDITION(!m_IsCreated, LOG_ERROR, "Constant Buffer already initialized", return);
		LOG_ON_CONDITION(
			shaderType == BindableType::VERTEX_SHADER || shaderType == BindableType::PIXEL_SHADER,
			LOG_ERROR,
			"Please pass proper shader type for the uniform",
			return
		);
		LOG_ON_CONDITION(dataBytes % 16 == 0, LOG_ERROR, 
			"DirectX only support constant buffer with byte size a multiple of 16\n"
			"Please pass data with size % 16 = 0\n"
			"constant buffer creation failed", 
			return);

		ASSERT(data != nullptr);
		ASSERT(dataBytes > 0);

		m_ShaderType = shaderType;
		m_BufferSlot = uniformLocation;
		m_DataSize = dataBytes;

		m_IsCreated = CreateAndUpdateBuffer(data, dataBytes);
	}

	void D3DConstantBuffer::Update(const void * data, size_t dataSize) {
		ASSERT(dataSize <= m_DataSize);

		D3D11_MAPPED_SUBRESOURCE msr;

		GetDeviceContext()->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		memcpy_s(msr.pData, m_DataSize, data, dataSize);
		GetDeviceContext()->Unmap(m_Buffer.Get(), 0);
	}

	void D3DConstantBuffer::ChangeBindingShader(BindableType shaderType) {
		LOG_ON_CONDITION(
			shaderType == BindableType::VERTEX_SHADER || shaderType == BindableType::PIXEL_SHADER,
			LOG_ERROR,
			"Please pass proper shader type for the uniform",
			return
		);

		m_ShaderType = shaderType;
	}

	bool D3DConstantBuffer::CreateAndUpdateBuffer(const void * data, unsigned int dataBytes) {
		D3D11_BUFFER_DESC vbd;
		vbd.ByteWidth				= dataBytes;
		vbd.BindFlags				= D3D11_BIND_CONSTANT_BUFFER;
		vbd.CPUAccessFlags			= D3D11_CPU_ACCESS_WRITE;
		vbd.MiscFlags				= 0;
		vbd.StructureByteStride		= 0;
		vbd.Usage					= D3D11_USAGE_DYNAMIC;

		D3D11_SUBRESOURCE_DATA vbsd;
		vbsd.pSysMem				= data;
		vbsd.SysMemPitch			= 0;
		vbsd.SysMemSlicePitch		= 0;

		HRESULT hr = GetDevice()->CreateBuffer(&vbd, &vbsd, m_Buffer.GetAddressOf());
		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to create constant buffer");
			return false;
		}

		return true;
	}

}