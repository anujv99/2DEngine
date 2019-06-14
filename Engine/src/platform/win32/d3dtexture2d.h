#pragma once

#include "d3dhelper.h"

#include "graphics/texture2d.h"

namespace prev {

	class D3DTexture2D : public Texture2D {
	public:
		D3DTexture2D() : m_IsCreated(false), m_BindSlot(0u) {}
		~D3DTexture2D() {}

		// Inherited via Texture2D
		virtual void Init(const std::string & fileName, unsigned int bindSlot) override;
		virtual void Init(const Texture2DDesc desc, unsigned int bindSlot) override;

		virtual void Bind() override;
		virtual void UnBind() override;
	private:
		bool CreateTexture(const Texture2DDesc & desc);
		DXGI_FORMAT GetTextureFormat(TextureFormat texFormat);
	private:
		unsigned int m_BindSlot;
		bool m_IsCreated;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_TextureView;
	};

}
