#include "pch.h"
#include "d3dframebuffer.h"
#include "math/vecconversion.h"

namespace prev {

	StrongHandle<Framebuffer> Framebuffer::CreateFramebuffer() {
		return new D3DFramebuffer();
	}

	D3DFramebuffer::~D3DFramebuffer() {
		//delete m_RenderTargetTexture;
	}

	void D3DFramebuffer::Bind() {
		LOG_ON_CONDITION(m_IsCreated, LOG_ERROR, "Binding uninitialized framebuffer", return);

		GetDeviceContext()->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);
	}

	void D3DFramebuffer::UnBind() {
		D3DContext * d3dContext = dynamic_cast<D3DContext *>(GraphicsContext::Get());
		d3dContext->BindDefaultRenderTarget();
	}

	void D3DFramebuffer::Init(Vec2 size, TextureFormat format, bool msaa) {
		LOG_ON_CONDITION(!m_IsCreated, LOG_ERROR, "Framebuffer already created", return);

		unsigned int samples = Window::Ref().GetDisplayMode().GetSamples();
		m_ISMultisampled = msaa;
		m_TextureFormat = GetTextureFormat(format);

		if (msaa) {
			m_IsCreated = CreateFramebufferMSAA(size, samples);
		} else {
			m_IsCreated = CreateFramebufferNoMSAA(size);
		}
	}

	void D3DFramebuffer::Clear() {
		static float clearColor[] = { 0, 0, 0, 0 };
		GetDeviceContext()->ClearRenderTargetView(m_RenderTargetView.Get(), clearColor);
	}

	void D3DFramebuffer::Clear(Vec4 color) {
		GetDeviceContext()->ClearRenderTargetView(m_RenderTargetView.Get(), &color[0]);
	}

	Vec2 D3DFramebuffer::GetSize() {
		return m_TextureSize;
	}

	StrongHandle<Texture2D> D3DFramebuffer::GetTexture() {
		if (m_ISMultisampled)
			ResolveTexture();
		return m_AntialiasedTexture;
	}

	void D3DFramebuffer::ResolveTexture() {
		GetDeviceContext()->ResolveSubresource(
			m_AntialiasedTexture->GetTexture2D().Get(), 0,
			m_MultisampledTexture.Get(), 0,
			m_TextureFormat
		);
	}

	bool D3DFramebuffer::CreateFramebufferMSAA(Vec2 size, unsigned int numSamples) {
		m_TextureSize = size;

		D3D11_TEXTURE2D_DESC textureDesc		= {};
		textureDesc.Width						= (UINT)size.x;
		textureDesc.Height						= (UINT)size.y;
		textureDesc.MipLevels					= 1;
		textureDesc.ArraySize					= 1;
		textureDesc.Format						= m_TextureFormat;
		textureDesc.SampleDesc.Count			= numSamples;
		textureDesc.SampleDesc.Quality			= 0;
		textureDesc.Usage						= D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags					= D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags				= 0;
		textureDesc.MiscFlags					= 0;

		HRESULT hr = GetDevice()->CreateTexture2D(&textureDesc, nullptr, m_MultisampledTexture.GetAddressOf());

		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to create texutre2d");
			return false;
		}

		D3D11_RENDER_TARGET_VIEW_DESC rtvd		= {};
		rtvd.Format								= textureDesc.Format;
		rtvd.ViewDimension						= D3D11_RTV_DIMENSION_TEXTURE2DMS;
		rtvd.Texture2D.MipSlice					= 0;

		hr = GetDevice()->CreateRenderTargetView(
			m_MultisampledTexture.Get(), &rtvd, m_RenderTargetView.GetAddressOf()
		);

		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to create rener target");
			return false;
		}

		D3D11_TEXTURE2D_DESC textureDescaa		= {};
		textureDescaa.Width						= (UINT)size.x;
		textureDescaa.Height					= (UINT)size.y;
		textureDescaa.MipLevels					= 1;
		textureDescaa.ArraySize					= 1;
		textureDescaa.Format					= m_TextureFormat;
		textureDescaa.SampleDesc.Count			= 1;
		textureDescaa.SampleDesc.Quality		= 0;
		textureDescaa.Usage						= D3D11_USAGE_DEFAULT;
		textureDescaa.BindFlags					= 0;
		textureDescaa.CPUAccessFlags			= 0;
		textureDescaa.MiscFlags					= 0;

		m_AntialiasedTexture = new D3DTexture2D(textureDescaa);

		return true;
	}

	bool D3DFramebuffer::CreateFramebufferNoMSAA(Vec2 size) {
		m_TextureSize = size;

		D3D11_TEXTURE2D_DESC textureDesc		= {};
		textureDesc.Width						= (UINT)size.x;
		textureDesc.Height						= (UINT)size.y;
		textureDesc.MipLevels					= 1;
		textureDesc.ArraySize					= 1;
		textureDesc.Format						= m_TextureFormat;
		textureDesc.SampleDesc.Count			= 1;
		textureDesc.SampleDesc.Quality			= 0;
		textureDesc.Usage						= D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags					= D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags				= 0;
		textureDesc.MiscFlags					= 0;

		m_AntialiasedTexture = new D3DTexture2D(textureDesc);

		if (!m_AntialiasedTexture->IsCreated()) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to create texutre2d");
			return false;
		}

		m_MultisampledTexture = m_AntialiasedTexture->GetTexture2D();

		D3D11_RENDER_TARGET_VIEW_DESC rtvd		= {};
		rtvd.Format								= textureDesc.Format;
		rtvd.ViewDimension						= D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvd.Texture2D.MipSlice					= 0;

		HRESULT hr = GetDevice()->CreateRenderTargetView(
			m_MultisampledTexture.Get(), &rtvd, m_RenderTargetView.GetAddressOf()
		);

		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to create rener target");
			return false;
		}

		return true;
	}

}