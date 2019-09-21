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

		GetDeviceContext()->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthView.Get());
	}

	void D3DFramebuffer::UnBind() {
		D3DContext * d3dContext = dynamic_cast<D3DContext *>(GraphicsContext::Get());
		d3dContext->BindDefaultRenderTarget();
	}

	void D3DFramebuffer::Init(Vec2 size, TextureFormat format, FramebufferFlags flags) {
		LOG_ON_CONDITION(!m_IsCreated, LOG_ERROR, "Framebuffer already created", return);

		m_TextureSize = size;
		m_TextureFormat = GetTextureFormat(format);

		bool msaa = false;
		unsigned int samples = 1;

		if (flags & FRAMEBUFFER_DISABLE_MSAA) {
			msaa = false;
		} else if (flags & FRAMEBUFFER_ENABLE_MSAA) {
			msaa = true;
		} else {
			msaa = Window::Ref().GetDisplayMode().IsMultisampled();
		}

		if (msaa) {
			if (flags & FRAMEBUFFER_MSAA_16x) {
				samples = 16;
			} else if (flags & FRAMEBUFFER_MSAA_8x) {
				samples = 8;
			} else if (flags & FRAMEBUFFER_MSAA_4x) {
				samples = 4;
			} else if (flags & FRAMEBUFFER_MSAA_2x) {
				samples = 2;
			} else {
				samples = Window::Ref().GetDisplayMode().GetSamples();
			}
		}

		m_ISMultisampled = msaa;

		if (msaa) {
			m_IsCreated = CreateFramebufferMSAA(size, samples);
		} else {
			m_IsCreated = CreateFramebufferNoMSAA(size);
		}

		if (flags & FRAMEBUFFER_ALLOCATE_DEPTH_BUFFER) {
			m_IsCreated = CreateDepthBuffer(size, samples);
		}
	}

	void D3DFramebuffer::Clear() {
		static float clearColor[] = { 0, 0, 0, 0 };
		GetDeviceContext()->ClearRenderTargetView(m_RenderTargetView.Get(), clearColor);
		if (m_DepthView != nullptr)
			GetDeviceContext()->ClearDepthStencilView(m_DepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void D3DFramebuffer::Clear(Vec4 color) {
		GetDeviceContext()->ClearRenderTargetView(m_RenderTargetView.Get(), &color[0]);
		if (m_DepthView != nullptr)
			GetDeviceContext()->ClearDepthStencilView(m_DepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	Vec2 D3DFramebuffer::GetSize() {
		return m_TextureSize;
	}

	StrongHandle<Texture2D> D3DFramebuffer::GetTexture() {
		if (m_ISMultisampled)
			ResolveTexture();
		return StrongHandle(dynamic_cast<Texture2D *>(m_AntialiasedTexture.Get()));
	}

	prev::StrongHandle<prev::Texture2D> D3DFramebuffer::GetDepthTexture() {
		return StrongHandle(dynamic_cast<Texture2D *>(m_DepthTexture.Get()));
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

		if (!m_AntialiasedTexture->IsCreated()) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to create texutre2d");
			return false;
		}

		return true;
	}

	bool D3DFramebuffer::CreateFramebufferNoMSAA(Vec2 size) {

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

	bool D3DFramebuffer::CreateDepthBuffer(Vec2 size, unsigned int numSamples) {
		D3D11_TEXTURE2D_DESC dbd;
		ZeroMemory(&dbd, sizeof(dbd));

		dbd.Width					= (UINT)size.x;
		dbd.Height					= (UINT)size.y;
		dbd.MipLevels				= 1;
		dbd.ArraySize				= 1;
		dbd.Format					= DXGI_FORMAT_R32_TYPELESS;
		dbd.SampleDesc.Count		= numSamples;
		dbd.SampleDesc.Quality		= 0;
		dbd.Usage					= D3D11_USAGE_DEFAULT;
		dbd.BindFlags				= D3D11_BIND_DEPTH_STENCIL;
		dbd.CPUAccessFlags			= 0;
		dbd.MiscFlags				= 0;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format						= DXGI_FORMAT_R32_FLOAT;
		if (numSamples > 1)
			srvDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2DMS;
		else
			srvDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip	= 0;
		srvDesc.Texture2D.MipLevels			= 1;

		m_DepthTexture = new D3DTexture2D(dbd, srvDesc);
		if (!m_DepthTexture->IsCreated()) {
			ERROR_TRACE(ERR_GRAPHICS_CONTEXT_CREATION_FAILED, "Failed to create Depth Stencil Texture2D");
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));

		dsvd.Format					= DXGI_FORMAT_D32_FLOAT;
		if (numSamples > 1)
			dsvd.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2DMS;
		else
			dsvd.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Texture2D.MipSlice		= 0;

		HRESULT hr = GetDevice()->CreateDepthStencilView(m_DepthTexture->GetTexture2D().Get(), &dsvd, m_DepthView.GetAddressOf());
		if (FAILED(hr)) {
			ERROR_TRACE(ERR_GRAPHICS_CONTEXT_CREATION_FAILED, "Failed to create Depth Stencil View");
			return false;
		}

		return true;
	}

}