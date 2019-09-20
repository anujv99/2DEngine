#pragma once

#include "d3dhelper.h"
#include "graphics/texture2d.h"

#include "d3dsampler2d.h"

namespace prev {

	class D3DTexture2D : public Texture2D {
	public:
		D3DTexture2D() : m_IsCreated(false), m_IsMapped(false), 
			m_BindSlot(0u), m_TextureSampler(nullptr), 
			m_MappedTexture({}), m_TextureResource(nullptr), m_StagingTexture(nullptr) {}
		~D3DTexture2D();

		// Inherited via Texture2D
		virtual void Init(const std::string & fileName, TextureParams texParams = TextureParams(), unsigned int texSlot = 0) override;
		virtual void Init(const Texture2DDesc desc, TextureParams texParams = TextureParams(), unsigned int texSlot = 0) override;
		virtual void SetTextureSlot(unsigned int texSlot) override;
		virtual void SetTextureParams(TextureParams texParams) override;
		virtual void SetData(const void * pixels) override;

		virtual void * Map() override;
		virtual void UnMap() override;

		virtual void Bind() override;
		virtual void UnBind() override;
	public:
		D3DTexture2D(D3D11_TEXTURE2D_DESC desc);
		D3DTexture2D(D3D11_TEXTURE2D_DESC desc, D3D11_SHADER_RESOURCE_VIEW_DESC vDesc);

		Microsoft::WRL::ComPtr<ID3D11Texture2D> GetTexture2D();

		inline bool IsCreated() const { return m_IsCreated; }
	private:
		bool CreateTexture(const Texture2DDesc & desc);
		unsigned int GetStrideFromFormat(DXGI_FORMAT format);
		D3D11_TEXTURE2D_DESC GetTextureDesc();

		void CreateStagingTexture();
	private:
		unsigned int m_BindSlot;
		bool m_IsCreated, m_IsMapped;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_TextureView;
		StrongHandle<D3DSampler2D> m_TextureSampler;

		D3D11_MAPPED_SUBRESOURCE m_MappedTexture;
		Microsoft::WRL::ComPtr<ID3D11Resource> m_TextureResource;
		Microsoft::WRL::ComPtr<ID3D11Resource> m_StagingTexture;
	};

}
