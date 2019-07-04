#include "pch.h"
#include "d3dcontext.h"

namespace com = Microsoft::WRL;

namespace prev {

	std::vector<DisplayMode> GraphicsContext::GetDisplayModes() {
		com::ComPtr<IDXGIFactory> factory;
		com::ComPtr<IDXGIAdapter> adaptor;
		com::ComPtr<IDXGIOutput> output;
		UINT numDisplayModes = 0;
		DXGI_MODE_DESC * displayModes;

		CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)factory.GetAddressOf());

		factory->EnumAdapters(0, adaptor.GetAddressOf());

		adaptor->EnumOutputs(0, output.GetAddressOf());

		auto hr = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, NULL);

		displayModes = new DXGI_MODE_DESC[numDisplayModes];

		output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, displayModes);

		std::vector<DisplayMode> displayModeList;

		for (UINT i = 0; i < numDisplayModes; i++) {

			if (i == 0) { 
				displayModeList.push_back(DisplayMode());
				displayModeList[displayModeList.size() - 1].WindowSize = Vec2i(displayModes[i].Width, displayModes[i].Height);
				continue;
			}

			if (displayModes[i].Width != displayModeList[displayModeList.size() - 1].GetWindowSize().x ||
				displayModes[i].Height != displayModeList[displayModeList.size() - 1].GetWindowSize().y) {
				displayModeList.push_back(DisplayMode());
				displayModeList[displayModeList.size() - 1].WindowSize = Vec2i(displayModes[i].Width, displayModes[i].Height);
			}

		}

		delete[] displayModes;

		std::reverse(displayModeList.begin(), displayModeList.end());

		return displayModeList;
	}
	
	GraphicsContext * GraphicsContext::CreateContext(const uintptr_t & windowRawPointer, const DisplayMode & displayMode) {
		return new D3DContext((HWND)(void *)windowRawPointer, displayMode);
	}

	D3DContext::D3DContext(HWND hWnd, const DisplayMode & displayMode) {
		DXGI_MODE_DESC displayModeDesc = GetDisplayModeDesc(displayMode);
		if (m_IsContextCreated == false) {
			return;
		}
		m_IsContextCreated = InitializeD3D(displayModeDesc, hWnd);
		if (m_IsContextCreated == false) {
			return;
		}
		m_IsContextCreated = CreateRenderTargetView(displayModeDesc);
		if (m_IsContextCreated == false) {
			return;
		}
		m_IsContextCreated = CreateDepthBuffer(displayModeDesc);
		if (m_IsContextCreated == false) {
			return;
		}
		m_IsContextCreated = CreateRasterizerState(displayModeDesc);
		if (m_IsContextCreated == false) {
			return;
		}

		if (displayMode.IsWindowFullscreen()) {
			m_SwapChain->SetFullscreenState(TRUE, nullptr);
		}

		LOG_TRACE("D3D11 Successfully Initialized");
	}

	void D3DContext::BeginFrame() {
		float clearColor[] = { 0, 0, 0, 0 };
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), clearColor);
		m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void D3DContext::EndFrame() {
		m_SwapChain->Present(1u, 0u);
	}

	DXGI_MODE_DESC D3DContext::GetDisplayModeDesc(const DisplayMode & displayMode) {
		com::ComPtr<IDXGIFactory> factory;
		com::ComPtr<IDXGIAdapter> adaptor;
		com::ComPtr<IDXGIOutput> output;
		UINT numDisplayModes = 0;
		DXGI_MODE_DESC * displayModes;

		CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)factory.GetAddressOf());

		factory->EnumAdapters(0, adaptor.GetAddressOf());

		adaptor->EnumOutputs(0, output.GetAddressOf());

		auto hr = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, NULL);

		displayModes = new DXGI_MODE_DESC[numDisplayModes];

		output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, displayModes);

		for (UINT i = 0; i < numDisplayModes; i++) {
			if (displayModes[i].Width == displayMode.GetWindowSize().x && displayModes[i].Height == displayMode.GetWindowSize().y) {
				auto disMode = displayModes[i];
				delete[] displayModes;
				return disMode;
			}
		}

		delete[] displayModes;

		ERROR_TRACE(ERR_GRAPHICS_CONTEXT_CREATION_FAILED, "Inavlid display resolution");
		m_IsContextCreated = false;
		return DXGI_MODE_DESC();
	}

	bool D3DContext::InitializeD3D(const DXGI_MODE_DESC & displayMode, HWND windowHandle) {
		DXGI_SWAP_CHAIN_DESC scd;

		scd.BufferDesc			= displayMode;
		scd.SampleDesc.Count	= 4;
		scd.SampleDesc.Quality	= 0;
		scd.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount			= 1;
		scd.OutputWindow		= windowHandle;
		scd.Windowed			= TRUE;
		scd.SwapEffect			= DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags				= 0;

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};

		D3D_FEATURE_LEVEL selectedFeatureLevels;

		UINT flags = 0;

		#ifdef ENGINE_DEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif

		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			NULL, D3D_DRIVER_TYPE_HARDWARE,
			NULL, flags,
			featureLevels, (UINT)std::size(featureLevels),
			D3D11_SDK_VERSION, &scd,
			m_SwapChain.GetAddressOf(), m_Device.GetAddressOf(),
			&selectedFeatureLevels, m_DeviceContext.GetAddressOf());

		if (FAILED(hr)) {
			ERROR_TRACE(ERR_GRAPHICS_CONTEXT_CREATION_FAILED, "Failed to create DirectX Device and Swap chain");
			return false;
		}

		return true;
	}

	bool D3DContext::CreateRenderTargetView(const DXGI_MODE_DESC & displayMode) {
		com::ComPtr<ID3D11Resource> backBuffer;
		HRESULT hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), (void **)backBuffer.GetAddressOf());
		if (FAILED(hr)) {
			ERROR_TRACE(ERR_GRAPHICS_CONTEXT_CREATION_FAILED, "Unable to get back buffer");
			return false;
		}

		hr = m_Device->CreateRenderTargetView(backBuffer.Get(), NULL, m_RenderTargetView.GetAddressOf());
		if (FAILED(hr)) {
			ERROR_TRACE(ERR_GRAPHICS_CONTEXT_CREATION_FAILED, "Failed to create DirectX Render Traget View");
			return false;
		}

		m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), NULL);

		return true;
	}

	bool D3DContext::CreateRasterizerState(const DXGI_MODE_DESC & displayMode) {
		D3D11_RASTERIZER_DESC rd;
		rd.FillMode					= D3D11_FILL_SOLID;
		rd.CullMode					= D3D11_CULL_NONE;
		rd.FrontCounterClockwise	= FALSE;
		rd.DepthBias				= 0;
		rd.DepthBiasClamp			= 0.0f;
		rd.SlopeScaledDepthBias		= 0.0f;
		rd.DepthClipEnable			= FALSE;
		rd.ScissorEnable			= TRUE;
		rd.MultisampleEnable		= FALSE;
		rd.AntialiasedLineEnable	= FALSE;

		HRESULT hr = m_Device->CreateRasterizerState(&rd, m_RasterizerState.GetAddressOf());
		if (FAILED(hr)) {
			ERROR_TRACE(ERR_GRAPHICS_CONTEXT_CREATION_FAILED, "Failed to create Rasterizer state");
			return false;
		}

		m_DeviceContext->RSSetState(m_RasterizerState.Get());

		return true;
	}

	bool D3DContext::CreateDepthBuffer(const DXGI_MODE_DESC & displayMode) {
		D3D11_DEPTH_STENCIL_DESC dsd;
		ZeroMemory(&dsd, sizeof(dsd));

		dsd.DepthEnable						= FALSE;
		dsd.DepthWriteMask					= D3D11_DEPTH_WRITE_MASK_ALL;
		dsd.DepthFunc						= D3D11_COMPARISON_LESS_EQUAL;
		dsd.StencilEnable					= FALSE;

		HRESULT hr = m_Device->CreateDepthStencilState(&dsd, m_DepthStencilState.GetAddressOf());
		if (FAILED(hr)) {
			ERROR_TRACE(ERR_GRAPHICS_CONTEXT_CREATION_FAILED, "Failed to create Depth Stencil State");
			return false;
		}

		m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 1u);

		D3D11_TEXTURE2D_DESC dbd;
		ZeroMemory(&dbd, sizeof(dbd));

		dbd.Width					= displayMode.Width;
		dbd.Height					= displayMode.Height;
		dbd.MipLevels				= 1;
		dbd.ArraySize				= 1;
		dbd.Format					= DXGI_FORMAT_D32_FLOAT;
		dbd.SampleDesc.Count		= 4;
		dbd.SampleDesc.Quality		= 0;
		dbd.Usage					= D3D11_USAGE_DEFAULT;
		dbd.BindFlags				= D3D11_BIND_DEPTH_STENCIL;
		dbd.CPUAccessFlags			= 0;
		dbd.MiscFlags				= 0;

		hr = m_Device->CreateTexture2D(&dbd, NULL, m_DepthStencilBuffer.GetAddressOf());
		if (FAILED(hr)) {
			ERROR_TRACE(ERR_GRAPHICS_CONTEXT_CREATION_FAILED, "Failed to create Depth Stencil Texture2D");
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));

		dsvd.Format					= DXGI_FORMAT_D32_FLOAT;
		dsvd.ViewDimension			= D3D11_DSV_DIMENSION_TEXTURE2DMS;
		dsvd.Texture2D.MipSlice		= 0;

		hr = m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &dsvd, m_DepthStencilView.GetAddressOf());
		if (FAILED(hr)) {
			ERROR_TRACE(ERR_GRAPHICS_CONTEXT_CREATION_FAILED, "Failed to create Depth Stencil View");
			return false;
		}

		m_DeviceContext->OMSetRenderTargets(1u, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());

		return true;
	}

}