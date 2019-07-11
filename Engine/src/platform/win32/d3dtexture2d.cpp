#include "pch.h"
#include "d3dtexture2d.h"

#include <Windows.h>
#include <comutil.h>

#include "wictextureloader.h"

namespace prev {

	StrongHandle<Texture2D> Texture2D::CreateTexture2D() {
		return new D3DTexture2D();
	}

	void D3DTexture2D::Init(const std::string & fileName, TextureParams texParams /*= TextureParams()*/, unsigned int texSlot /*= 0*/) {
		LOG_ON_CONDITION(!m_IsCreated, LOG_ERROR, "Texture already created", return);

		Microsoft::WRL::ComPtr<ID3D11Resource> textureResource;

		HRESULT hr = CreateWICTextureFromFile(
			GetDevice(),
			GetDeviceContext(),
			_bstr_t(fileName.c_str()),
			textureResource.GetAddressOf(),
			m_TextureView.GetAddressOf()
		);

		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to load texture : " + fileName);
			m_IsCreated = false;
			return;
		}

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
		textureResource->QueryInterface(texture.GetAddressOf());

		D3D11_TEXTURE2D_DESC textureDesc;
		texture->GetDesc(&textureDesc);

		m_TextureDesc.TextureSize = Vec2i(textureDesc.Width, textureDesc.Height);

		m_IsCreated = true;
		m_BindSlot = texSlot;

		m_TextureSampler = new D3DSampler2D();
		m_TextureSampler->Init(texParams);
	}

	void D3DTexture2D::Init(const Texture2DDesc desc, TextureParams texParams /*= TextureParams()*/, unsigned int texSlot /*= 0*/) {
		LOG_ON_CONDITION(!m_IsCreated, LOG_ERROR, "Texture already created", return);

		m_IsCreated = CreateTexture(desc);
		m_BindSlot = texSlot;

		m_TextureSampler = new D3DSampler2D();
		m_TextureSampler->Init(texParams);
	}

	void D3DTexture2D::SetTextureSlot(unsigned int texSlot) {
		m_BindSlot = texSlot;
	}

	void D3DTexture2D::SetTextureParams(TextureParams texParams) {
		m_TextureSampler->Init(texParams);
	}

	void D3DTexture2D::SetData(const void * pixels) {
		///https://github.com/TheCherno/Sparky

		D3D11_MAPPED_SUBRESOURCE msr;
		ZeroMemory(&msr, sizeof(msr));

		Microsoft::WRL::ComPtr<ID3D11Resource> textureRes;
		m_TextureView->GetResource(textureRes.GetAddressOf());
		GetDeviceContext()->Map(textureRes.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);

		auto texDesc = GetTextureDesc();

		for (unsigned int i = 0; i < texDesc.Width * texDesc.Height * GetStrideFromFormat(texDesc.Format); i += 4) {
			((byte *)msr.pData)[i + 0] = 0xff;
			((byte *)msr.pData)[i + 1] = 0xff;
			((byte *)msr.pData)[i + 2] = 0xff;
			((byte *)msr.pData)[i + 3] = ((byte *)pixels)[i / 4];
		}
		GetDeviceContext()->Unmap(textureRes.Get(), 0u);
	}

	void D3DTexture2D::Bind() {
		LOG_ON_CONDITION(m_IsCreated, LOG_ERROR, "Binding uninitialized texutre", return);

		//Change this so that texture can be used in vertex shader also
		m_TextureSampler->Bind(m_BindSlot);
		GetDeviceContext()->PSSetShaderResources(m_BindSlot, 1, m_TextureView.GetAddressOf());
	}

	void D3DTexture2D::UnBind() {
		m_TextureSampler->UnBind(m_BindSlot);

		ID3D11ShaderResourceView * const temp[] = { nullptr };
		GetDeviceContext()->PSSetShaderResources(m_BindSlot, 1, temp);
	}

	bool D3DTexture2D::CreateTexture(const Texture2DDesc & desc) {
		D3D11_TEXTURE2D_DESC textureDesc	= {};
		textureDesc.Width					= (UINT)desc.TextureSize.x;
		textureDesc.Height					= (UINT)desc.TextureSize.y;
		textureDesc.MipLevels				= 1;
		textureDesc.ArraySize				= 1;
		textureDesc.Format					= GetTextureFormat(desc.TexFormat);
		textureDesc.SampleDesc.Count		= 1;
		textureDesc.SampleDesc.Quality		= 0;
		textureDesc.Usage					= D3D11_USAGE_DYNAMIC;
		textureDesc.BindFlags				= D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags			= D3D11_CPU_ACCESS_WRITE;
		textureDesc.MiscFlags				= 0;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> d3dtexure;
		HRESULT hr = GetDevice()->CreateTexture2D(&textureDesc, nullptr, d3dtexure.GetAddressOf());
		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to create texutre2d");
			return false;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc		= {};
		srvDesc.Format								= textureDesc.Format;
		srvDesc.ViewDimension						= D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip			= 0;
		srvDesc.Texture2D.MipLevels					= 1;

		hr = GetDevice()->CreateShaderResourceView(d3dtexure.Get(), &srvDesc, m_TextureView.GetAddressOf());
		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to create texutre2d view");
			return false;
		}

		m_TextureDesc = desc;
		return true;
	}

	DXGI_FORMAT D3DTexture2D::GetTextureFormat(TextureFormat texFormat) {
		switch (texFormat) {
		case prev::PV_TEXTURE_FORMAT_RGBA8: return DXGI_FORMAT_R8G8B8A8_UNORM;
		default: return DXGI_FORMAT_UNKNOWN;
		}
	}

	unsigned int D3DTexture2D::GetStrideFromFormat(DXGI_FORMAT format) {
		switch (format) {
		case DXGI_FORMAT_R8G8B8A8_UNORM: return 4;
		default: return 0;
		}
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> D3DTexture2D::GetTexture2D() {
		Microsoft::WRL::ComPtr<ID3D11Resource> textureRes;
		m_TextureView->GetResource(textureRes.GetAddressOf());

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
		textureRes->QueryInterface<ID3D11Texture2D>(texture.GetAddressOf());
		return texture;
	}

	D3D11_TEXTURE2D_DESC D3DTexture2D::GetTextureDesc() {
		D3D11_TEXTURE2D_DESC texDesc;
		GetTexture2D()->GetDesc(&texDesc);
		return texDesc;
	}

}