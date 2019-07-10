#include "pch.h"
#include "d3dsampler2d.h"

namespace prev {

	void D3DSampler2D::Init(const TextureParams samplerDesc) {
		if (m_IsCreated) {
			m_SamplerState.Reset();
			m_IsCreated = false;
		}
		m_IsCreated = CreateSamplerState(samplerDesc);
	}

	void D3DSampler2D::Bind(unsigned int slot) {
		LOG_ON_CONDITION(m_IsCreated, LOG_ERROR, "Binding uninitialized sampler state", return);

		GetDeviceContext()->PSSetSamplers(slot, 1, m_SamplerState.GetAddressOf());
	}

	void D3DSampler2D::UnBind(unsigned int slot) {
		LOG_ON_CONDITION(m_IsCreated, LOG_ERROR, "UnBinding uninitialized sampler state", return);

		ID3D11SamplerState * const temp[] = { nullptr };
		GetDeviceContext()->PSSetSamplers(slot, 1, temp);
	}

	bool D3DSampler2D::CreateSamplerState(const TextureParams samplerDesc) {
		D3D11_SAMPLER_DESC sd = {};
		sd.Filter			= GetTextureFilter(samplerDesc.Filtering);
		sd.AddressU			= GetTextureAdressMode(samplerDesc.Wrapping);
		sd.AddressV			= GetTextureAdressMode(samplerDesc.Wrapping);
		sd.AddressW			= GetTextureAdressMode(samplerDesc.Wrapping);
		sd.MaxAnisotropy	= 16;

		HRESULT hr = GetDevice()->CreateSamplerState(&sd, m_SamplerState.GetAddressOf());
		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "unable to create sampler state");
			return false;
		}

		return true;
	}

	D3D11_FILTER D3DSampler2D::GetTextureFilter(TextureFilterType filerType) {
		switch (filerType) {
		case prev::PV_TEXTURE_FILTER_DEFAULT: return D3D11_FILTER_MAXIMUM_ANISOTROPIC;
		case prev::PV_TEXTURE_FILTER_NEAREST: return D3D11_FILTER_MIN_MAG_MIP_POINT;
		default: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		}
	}

	D3D11_TEXTURE_ADDRESS_MODE D3DSampler2D::GetTextureAdressMode(TextureWrapType wrapType) {
		switch (wrapType) {
		case prev::PV_TEXTURE_WRAP_DEFAULT: return D3D11_TEXTURE_ADDRESS_WRAP;
		default: return D3D11_TEXTURE_ADDRESS_WRAP;
		}
	}

}