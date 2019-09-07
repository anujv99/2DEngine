#include "pch.h"
#include "d3dcomputebuffer.h"

namespace prev {

	StrongHandle<ComputeBuffer> ComputeBuffer::CreateComputeBuffer() {
		return new D3DComputeBuffer();
	}

	D3DComputeBuffer::D3DComputeBuffer() : 
		m_StrideBytes(0u),
		m_NumElements(0u),
		m_TotalBytes(0u),
		m_BindSlot(0u),
		m_IsInitialized(false),
		m_IsMapped(false), 
		m_MappedBuffer({}),
		m_ShaderResource(nullptr) {
	}

	void D3DComputeBuffer::Bind() {
		LOG_ON_CONDITION(m_IsInitialized, LOG_ERROR, "Buffer not initialized", return);

		static UINT INIT_COUNTS = 0u;
		GetDeviceContext()->CSSetUnorderedAccessViews(m_BindSlot, 1u, m_UnorderedView.GetAddressOf(), &INIT_COUNTS);
	}

	void D3DComputeBuffer::UnBind() {
		LOG_ON_CONDITION(m_IsInitialized, LOG_ERROR, "Buffer not initialized", return);
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav;
		GetDeviceContext()->CSSetUnorderedAccessViews(m_BindSlot, 1u, uav.GetAddressOf(), nullptr);
	}

	void D3DComputeBuffer::BindToPixelShader(unsigned int slot) {
		LOG_ON_CONDITION(m_IsInitialized, LOG_ERROR, "Buffer not initialized", return);

		if (m_ShaderResource == nullptr)
			CreateShaderResource();

		GetDeviceContext()->PSSetShaderResources(slot, 1, m_ShaderResource.GetAddressOf());
	}

	void D3DComputeBuffer::Init(const void * data, unsigned int numElements, unsigned int strideBytes) {
		LOG_ON_CONDITION(!m_IsInitialized, LOG_ERROR, "Buffer already initialized", return);

		ASSERT(numElements > 0);
		ASSERT(strideBytes > 0);

		m_StrideBytes = strideBytes;
		m_NumElements = numElements;
		m_TotalBytes = m_StrideBytes * m_NumElements;

		m_IsInitialized = CreateBuffer(data);
	}

	void D3DComputeBuffer::SubData(const void * data, unsigned int numBytes, unsigned int byteOffset) {
		LOG_ON_CONDITION(m_IsInitialized, LOG_ERROR, "Buffer not initialized", return);

		D3D11_MAPPED_SUBRESOURCE msr;

		GetDeviceContext()->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		memcpy_s((byte *)msr.pData + byteOffset, m_TotalBytes, data, numBytes);
		GetDeviceContext()->Unmap(m_Buffer.Get(), 0);
	}

	void * D3DComputeBuffer::Map() {
		LOG_ON_CONDITION(m_IsInitialized, LOG_ERROR, "Buffer not initialized", return nullptr);
		if (m_IsMapped) return m_MappedBuffer.pData;

		GetDeviceContext()->Map(m_Buffer.Get(), 0, D3D11_MAP_READ_WRITE, 0, &m_MappedBuffer);
		m_IsMapped = true;
		return m_MappedBuffer.pData;
	}

	void D3DComputeBuffer::UnMap() {
		if (!m_IsMapped) return;

		GetDeviceContext()->Unmap(m_Buffer.Get(), 0);
		ZeroMemory(&m_MappedBuffer, sizeof(m_MappedBuffer));
		m_IsMapped = false;
		return;
	}

	bool D3DComputeBuffer::CreateBuffer(const void * data) {
		D3D11_BUFFER_DESC vbd;
		vbd.ByteWidth				= m_TotalBytes;
		vbd.BindFlags				= D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		vbd.CPUAccessFlags			= D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		vbd.MiscFlags				= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		vbd.StructureByteStride		= m_StrideBytes;
		vbd.Usage					= D3D11_USAGE_DEFAULT;

		HRESULT hr = S_OK;

		if (data != nullptr) {
			D3D11_SUBRESOURCE_DATA vbsd;
			vbsd.pSysMem			= data;
			vbsd.SysMemPitch		= 0;
			vbsd.SysMemSlicePitch	= 0;

			hr = GetDevice()->CreateBuffer(&vbd, &vbsd, m_Buffer.GetAddressOf());
		} else {
			hr = GetDevice()->CreateBuffer(&vbd, nullptr, m_Buffer.GetAddressOf());
		}

		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to create compute buffer");
			return false;
		}

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavdesc;
		uavdesc.Buffer.FirstElement			= 0u;
		uavdesc.Buffer.Flags				= 0u;
		uavdesc.Buffer.NumElements			= m_NumElements;
		uavdesc.Format						= DXGI_FORMAT_UNKNOWN;
		uavdesc.ViewDimension				= D3D11_UAV_DIMENSION_BUFFER;
		
		hr = GetDevice()->CreateUnorderedAccessView(m_Buffer.Get(), &uavdesc, m_UnorderedView.GetAddressOf());

		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to create compute buffer unordered access view");
			return false;
		}
		
		return true;

	}

	bool D3DComputeBuffer::CreateShaderResource() {
		D3D11_SHADER_RESOURCE_VIEW_DESC srvdesc;
		srvdesc.Buffer.ElementOffset		= 0u;
		srvdesc.Buffer.ElementWidth			= m_StrideBytes;
		srvdesc.Buffer.FirstElement			= 0u;
		srvdesc.Buffer.NumElements			= m_NumElements;
		srvdesc.Format						= DXGI_FORMAT_UNKNOWN;
		srvdesc.ViewDimension				= D3D11_SRV_DIMENSION_BUFFER;

		HRESULT hr = GetDevice()->CreateShaderResourceView(m_Buffer.Get(), &srvdesc, m_ShaderResource.GetAddressOf());
		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to create compute buffer shader resource view");
			return false;
		}

		return true;
	}

}