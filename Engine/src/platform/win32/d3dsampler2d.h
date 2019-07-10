#pragma once

#include "d3dhelper.h"
#include "graphics/texture2d.h"

namespace prev {

	class D3DSampler2D {
	public:
		D3DSampler2D() : m_IsCreated(false) {}
		~D3DSampler2D() {}

		// Inherited via Sampler2D
		virtual void Init(const TextureParams samplerDesc);
		virtual void Bind(unsigned int slot);
		virtual void UnBind(unsigned int slot);
	private:
		bool CreateSamplerState(const TextureParams samplerDesc);
		D3D11_FILTER GetTextureFilter(TextureFilterType filerType);
		D3D11_TEXTURE_ADDRESS_MODE GetTextureAdressMode(TextureWrapType wrapType);
	private:
		bool m_IsCreated;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SamplerState;
	};

}
