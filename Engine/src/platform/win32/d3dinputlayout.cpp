#include "pch.h"
#include "d3dinputlayout.h"

#include "d3dvertexshader.h"

namespace prev {

	StrongHandle<VertexLayout> VertexLayout::CreateVertexLayout() {
		return new D3DInputLayout();
	}

	void D3DInputLayout::Bind() {
		LOG_ON_CONDITION(m_InputLayout != nullptr, LOG_ERROR, "Binding uninitialized input layout", return);
		GetDeviceContext()->IASetInputLayout(m_InputLayout.Get());
	}

	void D3DInputLayout::UnBind() {}

	bool D3DInputLayout::BuildVertexLayout(StrongHandle<VertexShader> vertexShader) {

		std::vector<D3D11_INPUT_ELEMENT_DESC> inputDescs;

		for (UINT i = 0; i < m_NumEntries; i++) {
			inputDescs.push_back(D3D11_INPUT_ELEMENT_DESC());

			inputDescs[i].SemanticName				= m_Entries[i].LayoutName.c_str();
			inputDescs[i].SemanticIndex				= 0;
			inputDescs[i].Format					= GetLayoutFormat(m_Entries[i].EntryDataType, m_Entries[i].NumElements, m_Entries[i].Normalized);
			inputDescs[i].InputSlot					= 0; //Consider changing this
			inputDescs[i].AlignedByteOffset			= D3D11_APPEND_ALIGNED_ELEMENT;
			inputDescs[i].InputSlotClass			= D3D11_INPUT_PER_VERTEX_DATA;
			inputDescs[i].InstanceDataStepRate		= 0u;
		}

		D3DVertexShader * vShader = dynamic_cast<D3DVertexShader *>(vertexShader.Get());
		Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecode = vShader->GetShaderBytecode();

		if (shaderBytecode == nullptr) {
			LOG_ERROR("Inavlid vertex shader passed");
			return false;
		}

		HRESULT hr = GetDevice()->CreateInputLayout(
			inputDescs.data(), (UINT)inputDescs.size(),
			shaderBytecode->GetBufferPointer(),
			shaderBytecode->GetBufferSize(),
			m_InputLayout.GetAddressOf()
		);

		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to create input layout");
			return false;
		}
		
		return true;
	}

	DXGI_FORMAT D3DInputLayout::GetLayoutFormat(LayoutDataType dataType, unsigned int numElements, bool normalized) {

		if (normalized) {
			switch (dataType) {
			case prev::PV_DATA_TYPE_SINT_16:
			{
				switch (numElements) {
				case 1: return DXGI_FORMAT_R16_SNORM;
				case 2: return DXGI_FORMAT_R16G16_SNORM;
				case 4: return DXGI_FORMAT_R16G16B16A16_SNORM;
				default: DXGI_FORMAT_UNKNOWN;
				}
			}
			case prev::PV_DATA_TYPE_SINT_8:
			{
				switch (numElements) {
				case 1: return DXGI_FORMAT_R8_SNORM;
				case 2: return DXGI_FORMAT_R8G8_SNORM;
				case 4: return DXGI_FORMAT_R8G8B8A8_SNORM;
				default: DXGI_FORMAT_UNKNOWN;
				}
			}
			case prev::PV_DATA_TYPE_UINT_16:
			{
				switch (numElements) {
				case 1: return DXGI_FORMAT_R16_UNORM;
				case 2: return DXGI_FORMAT_R16G16_UNORM;
				case 4: return DXGI_FORMAT_R16G16B16A16_UNORM;
				default: DXGI_FORMAT_UNKNOWN;
				}
			}
			case prev::PV_DATA_TYPE_UINT_8:
			{
				switch (numElements) {
				case 1: return DXGI_FORMAT_R8_UNORM;
				case 2: return DXGI_FORMAT_R8G8_UNORM;
				case 4: return DXGI_FORMAT_R8G8B8A8_UNORM;
				default: DXGI_FORMAT_UNKNOWN;
				}
			}
			default:
				break;
			}
		}

		switch (dataType) {
		case PV_DATA_TYPE_FLOAT_32:
		{
			switch (numElements) {
			case 1: return DXGI_FORMAT_R32_FLOAT;
			case 2: return DXGI_FORMAT_R32G32_FLOAT;
			case 3: return DXGI_FORMAT_R32G32B32_FLOAT;
			case 4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
			default: return DXGI_FORMAT_UNKNOWN;
			}
		}
		case PV_DATA_TYPE_FLOAT_16:
		{
			switch (numElements) {
			case 1: return DXGI_FORMAT_R16_FLOAT;
			case 2: return DXGI_FORMAT_R16G16_FLOAT;
			case 4: return DXGI_FORMAT_R16G16B16A16_FLOAT;
			default:
				return DXGI_FORMAT_UNKNOWN;
			}
		}
		case PV_DATA_TYPE_SINT_32:
		{
			switch (numElements) {
			case 1: return DXGI_FORMAT_R32_SINT;
			case 2: return DXGI_FORMAT_R32G32_SINT;
			case 3: return DXGI_FORMAT_R32G32B32_SINT;
			case 4: return DXGI_FORMAT_R32G32B32A32_SINT;
			default: return DXGI_FORMAT_UNKNOWN;
			}
		}
		case PV_DATA_TYPE_SINT_16:
		{
			switch (numElements) {
			case 1: return DXGI_FORMAT_R16_SINT;
			case 2: return DXGI_FORMAT_R16G16_SINT;
			case 4: return DXGI_FORMAT_R16G16B16A16_SINT;
			default: return DXGI_FORMAT_UNKNOWN;
			}
		}
		case PV_DATA_TYPE_SINT_8:
		{
			switch (numElements) {
			case 1: return DXGI_FORMAT_R8_SINT;
			case 2: return DXGI_FORMAT_R8G8_SINT;
			case 4: return DXGI_FORMAT_R8G8B8A8_SINT;
			default: return DXGI_FORMAT_UNKNOWN;
			}
		}
		case PV_DATA_TYPE_UINT_32:
		{
			switch (numElements) {
			case 1: return DXGI_FORMAT_R32_UINT;
			case 2: return DXGI_FORMAT_R32G32_UINT;
			case 3: return DXGI_FORMAT_R32G32B32_UINT;
			case 4: return DXGI_FORMAT_R32G32B32A32_UINT;
			default: return DXGI_FORMAT_UNKNOWN;
			}
		}
		case PV_DATA_TYPE_UINT_16:
		{
			switch (numElements) {
			case 1: return DXGI_FORMAT_R16_UINT;
			case 2: return DXGI_FORMAT_R16G16_UINT;
			case 4: return DXGI_FORMAT_R16G16B16A16_UINT;
			default: return DXGI_FORMAT_UNKNOWN;
			}
		}
		case PV_DATA_TYPE_UINT_8:
		{
			switch (numElements) {
			case 1: return DXGI_FORMAT_R8_UINT;
			case 2: return DXGI_FORMAT_R8G8_UINT;
			case 4: return DXGI_FORMAT_R8G8B8A8_UINT;
			default: return DXGI_FORMAT_UNKNOWN;
			}
		}
		default:
			return DXGI_FORMAT_UNKNOWN;
		}

	}

}
