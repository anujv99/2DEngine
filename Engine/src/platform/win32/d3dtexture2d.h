#pragma once

#include "d3dhelper.h"
#include "graphics/texture2d.h"

#include "d3dsampler2d.h"

namespace prev {

	class D3DTexture2D : public Texture2D {
	public:
		D3DTexture2D() : m_IsCreated(false), m_BindSlot(0u), m_TextureSampler(nullptr) {}
		~D3DTexture2D() {}

		// Inherited via Texture2D
		virtual void Init(const std::string & fileName, TextureParams texParams = TextureParams(), unsigned int texSlot = 0) override;
		virtual void Init(const Texture2DDesc desc, TextureParams texParams = TextureParams(), unsigned int texSlot = 0) override;
		virtual void SetTextureSlot(unsigned int texSlot) override;
		virtual void SetTextureParams(TextureParams texParams) override;

		virtual void Bind() override;
		virtual void UnBind() override;

	private:
		bool CreateTexture(const Texture2DDesc & desc);
		DXGI_FORMAT GetTextureFormat(TextureFormat texFormat);
	private:
		unsigned int m_BindSlot;
		bool m_IsCreated;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_TextureView;
		D3DSampler2D * m_TextureSampler;
	};

}
