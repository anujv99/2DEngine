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

	void * D3DTexture2D::Map() {
		if (m_IsMapped) return m_MappedTexture.pData;

		if (m_StagingTexture == nullptr)
			CreateStagingTexture();

		if (m_StagingTexture == nullptr) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to map texture");
			return nullptr;
		}

		if (m_TextureResource == nullptr) {
			m_TextureView->GetResource(m_TextureResource.GetAddressOf());
		}

		if (m_TextureResource == nullptr) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unabel to map texture");
			return nullptr;
		}

		GetDeviceContext()->CopyResource(m_StagingTexture.Get(), m_TextureResource.Get());

		HRESULT hr = GetDeviceContext()->Map(m_StagingTexture.Get(), 0u, D3D11_MAP_READ, 0u, &m_MappedTexture);
		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to map texture");
			return nullptr;
		}

		m_IsMapped = true;
		return m_MappedTexture.pData;
	}

	void D3DTexture2D::UnMap() {
		if (!m_IsMapped) return;

		GetDeviceContext()->Unmap(m_StagingTexture.Get(), 0u);
		m_IsMapped = false;
		m_TextureResource = nullptr;
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

	void * D3DTexture2D::GetNativeTexture() {
		return (void *)m_TextureView.Get();
	}

	D3DTexture2D::D3DTexture2D(D3D11_TEXTURE2D_DESC desc) : m_IsCreated(false), m_BindSlot(0u), m_TextureSampler(nullptr) {

		desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> d3dtexure;
		HRESULT hr = GetDevice()->CreateTexture2D(&desc, nullptr, d3dtexure.GetAddressOf());
		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to create texutre2d");
			return;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format						= desc.Format;
		srvDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip	= 0;
		srvDesc.Texture2D.MipLevels			= 1;

		hr = GetDevice()->CreateShaderResourceView(d3dtexure.Get(), &srvDesc, m_TextureView.GetAddressOf());
		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to create texutre2d view");
			return;
		}

		TextureParams params;
		params.Filtering	= PV_TEXTURE_FILTER_NEAREST;
		params.Wrapping		= PV_TEXTURE_WRAP_CLAMP;

		m_TextureSampler = new D3DSampler2D();
		m_TextureSampler->Init(params);


		m_TextureDesc.TexFormat = GetTextureFormat(desc.Format);
		m_TextureDesc.TextureSize = Vec2i(desc.Width, desc.Height);
		m_IsMapped = false;
		m_IsCreated = true;
	}

	D3DTexture2D::D3DTexture2D(D3D11_TEXTURE2D_DESC desc, D3D11_SHADER_RESOURCE_VIEW_DESC vDesc) : 
		m_IsCreated(false), m_BindSlot(0u), m_TextureSampler(nullptr), m_IsMapped(false) {
		desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> d3dtexure;
		HRESULT hr = GetDevice()->CreateTexture2D(&desc, nullptr, d3dtexure.GetAddressOf());
		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to create texutre2d");
			return;
		}

		hr = GetDevice()->CreateShaderResourceView(d3dtexure.Get(), &vDesc, m_TextureView.GetAddressOf());
		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to create texutre2d view");
			return;
		}

		TextureParams params;
		params.Filtering	= PV_TEXTURE_FILTER_LINEAR;
		params.Wrapping		= PV_TEXTURE_WRAP_CLAMP;

		m_TextureSampler = new D3DSampler2D();
		m_TextureSampler->Init(params);

		m_IsCreated = true;
	}

	D3DTexture2D::~D3DTexture2D() {}

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

	void D3DTexture2D::CreateStagingTexture() {
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = (UINT)m_TextureDesc.TextureSize.x;
		textureDesc.Height = (UINT)m_TextureDesc.TextureSize.y;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = GetTextureFormat(m_TextureDesc.TexFormat);
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_STAGING;
		textureDesc.BindFlags = 0u;
		textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		textureDesc.MiscFlags = 0;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D;

		HRESULT hr = GetDevice()->CreateTexture2D(&textureDesc, nullptr, tex2D.GetAddressOf());
		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to create staging texutre2d");
			tex2D = nullptr;
		}

		if (tex2D != nullptr) {
			HRESULT hr = tex2D->QueryInterface<ID3D11Resource>(m_StagingTexture.GetAddressOf());
			if (FAILED(hr)) {
				ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to get resource on staging texture");
				tex2D = nullptr;
				m_StagingTexture = nullptr;
			}
		}

		return;
	}

}