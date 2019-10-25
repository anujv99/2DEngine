#include "pch.h"
#include "d3dcontext.h"

#include <dxgi1_6.h>
#include <comutil.h>

namespace com = Microsoft::WRL;

#if defined(ENGINE_DEBUG)
	#define CHECK_CONTEXT_CREATION(F) m_IsContextCreated = F; if (!m_IsContextCreated) return;
	#define CHECK_HR(F, M) if (FAILED(F)) { ERROR_TRACE(ERR_GRAPHICS_CONTEXT_CREATION_FAILED, M); return false; }
#elif defined(ENGINE_RELEASE) || defined(ENGINE_DIST)
	#define CHECK_CONTEXT_CREATION(F) F
	#define CHECK_HR(F, M) F
#endif

namespace prev {

	struct D3DGraphicsAdapter : public GraphicsAdapter {
		D3DGraphicsAdapter() : GraphicsAdapter() {}
		com::ComPtr<IDXGIAdapter1> Adapter;
	};

	struct D3DDisplayMode : public DisplayMode {
		D3DDisplayMode() {}
		DXGI_MODE_DESC DisplayMode;
	};

	std::vector<StrongHandle<GraphicsAdapter>> GraphicsContext::GetDisplayModes() {

		SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
		SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
		SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

		com::ComPtr<IDXGIFactory1> factory;
		CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void **)factory.GetAddressOf());

		com::ComPtr<IDXGIAdapter1> adapter;

		std::vector<StrongHandle<GraphicsAdapter>> graphicsadapters;
		
		for (UINT i = 0; !FAILED(factory->EnumAdapters1(i, adapter.GetAddressOf())); i++) {
			StrongHandle<D3DGraphicsAdapter> adap = new D3DGraphicsAdapter();
			adap->Adapter = adapter;

			DXGI_ADAPTER_DESC1 adapterDesc;
			adapter->GetDesc1(&adapterDesc);

			adap->AdapterDescription = _bstr_t(adapterDesc.Description);
			adap->DedicatedVideoMemory = adapterDesc.DedicatedVideoMemory;
			
			com::ComPtr<IDXGIOutput> output;

			for (UINT j = 0; !FAILED(adapter->EnumOutputs(j, output.GetAddressOf())); j++) {
				DXGI_OUTPUT_DESC outputDesc;
				output->GetDesc(&outputDesc);

				StrongHandle<Monitor> mon = new Monitor();
				mon->TopLeft = Vec2i(outputDesc.DesktopCoordinates.left, outputDesc.DesktopCoordinates.top);
				mon->BottomRight = Vec2i(outputDesc.DesktopCoordinates.right, outputDesc.DesktopCoordinates.bottom);
				mon->MonitorDescription = _bstr_t(outputDesc.DeviceName);

				UINT numDisplayModes = 0u;

				output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, NULL);

				DXGI_MODE_DESC * displayModes = new DXGI_MODE_DESC[numDisplayModes];

				output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, displayModes);

				for (UINT k = 0; k < numDisplayModes; k++) {
					
					if (k == 0) {
						StrongHandle<D3DDisplayMode> mode = new D3DDisplayMode();
						mode->WindowSize = Vec2i(displayModes[i].Width, displayModes[i].Height);
						mode->DisplayMode = displayModes[k];
						mon->DisplayModes.push_back((DisplayMode *)mode);
						continue;
					}

					if (displayModes[k].Width != displayModes[k - 1].Width || displayModes[k].Height != displayModes[k - 1].Height) {
						StrongHandle<D3DDisplayMode> mode = new D3DDisplayMode();
						mode->WindowSize = Vec2i(displayModes[k].Width, displayModes[k].Height);
						mode->DisplayMode = displayModes[k];
						mon->DisplayModes.push_back((DisplayMode *)mode);
					}

				}

				delete[] displayModes;

				std::reverse(mon->DisplayModes.begin(), mon->DisplayModes.end());

				adap->Monitors.push_back(mon);
			}

			graphicsadapters.push_back((GraphicsAdapter *)adap);
		}

		return graphicsadapters;

	}
	
	GraphicsContext * GraphicsContext::CreateContext(const uintptr_t & windowRawPointer, const StrongHandle<DisplayMode> & displayMode, const StrongHandle <GraphicsAdapter> & adapter) {
		return new D3DContext((HWND)(void *)windowRawPointer, displayMode, adapter);
	}

	D3DContext::D3DContext(HWND hWnd, const StrongHandle<DisplayMode> & displayMode, const StrongHandle <GraphicsAdapter> & adapter) {
		D3DDisplayMode * disMode = (D3DDisplayMode *)displayMode.Get();
		DXGI_MODE_DESC displayModeDesc = disMode->DisplayMode;
		m_NumSamples = displayMode->GetSamples();

		if (m_IsContextCreated == false) {
			return;
		}

		CHECK_CONTEXT_CREATION(InitializeD3D(displayModeDesc, hWnd));
		CHECK_CONTEXT_CREATION(CreateRenderTargetView());
		CHECK_CONTEXT_CREATION(CreateDepthBuffer(displayModeDesc));
		CHECK_CONTEXT_CREATION(CreateRasterizerState(displayModeDesc));

		if (displayMode->IsWindowFullscreen()) {
			m_SwapChain->SetFullscreenState(TRUE, nullptr);
		}

		LOG_TRACE("D3D11 Successfully Initialized");
	}

	D3DContext::~D3DContext() {
		m_DepthStencilBuffer = nullptr;
		m_DepthStencilView = nullptr;
		m_DepthStencilState = nullptr;
		m_RasterizerState = nullptr;
		m_RenderTargetView = nullptr;
		m_DeviceContext = nullptr;
		m_Device = nullptr;
		m_SwapChain = nullptr;
	}

	void D3DContext::BeginFrame() {
		float clearColor[] = { 0, 0, 0, 0 };
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), clearColor);
		m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void D3DContext::EndFrame() {
		m_SwapChain->Present(1u, 0u);
	}

	bool D3DContext::ChangeResolution(Vec2i newResolution) {
		com::ComPtr<ID3D11Resource> backBuffer;
		CHECK_HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), (void **)backBuffer.GetAddressOf()), "Unable to get back buffer");
		backBuffer = nullptr;
		m_RenderTargetView = nullptr;

		CHECK_HR(m_SwapChain->ResizeBuffers(0, newResolution.x, newResolution.y, DXGI_FORMAT_UNKNOWN, 0u), "Unable to resize D3DBuffer");

		CreateRenderTargetView();

		DXGI_MODE_DESC desc;
		desc.Width = newResolution.x;
		desc.Height = newResolution.y;

		m_DepthStencilBuffer = nullptr;
		m_DepthStencilView = nullptr;
		m_DepthStencilState = nullptr;

		CreateDepthBuffer(desc);

		return true;
	}

	void D3DContext::BindDefaultRenderTarget() {
		m_DeviceContext->OMSetRenderTargets(1u, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
	}

	bool D3DContext::InitializeD3D(const DXGI_MODE_DESC & displayMode, HWND windowHandle) {
		DXGI_SWAP_CHAIN_DESC scd;

		scd.BufferDesc			= displayMode;
		scd.SampleDesc.Count	= m_NumSamples;
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

		CHECK_HR(D3D11CreateDeviceAndSwapChain(
			NULL, D3D_DRIVER_TYPE_HARDWARE,
			NULL, flags,
			featureLevels, (UINT)std::size(featureLevels),
			D3D11_SDK_VERSION, &scd,
			m_SwapChain.GetAddressOf(), m_Device.GetAddressOf(),
			&selectedFeatureLevels, m_DeviceContext.GetAddressOf()), "Failed to create DirectX Device and Swap chain");

		return true;
	}

	bool D3DContext::CreateRenderTargetView() {
		com::ComPtr<ID3D11Resource> backBuffer;
		CHECK_HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), (void **)backBuffer.GetAddressOf()), "Unable to get back buffer");
		CHECK_HR(m_Device->CreateRenderTargetView(backBuffer.Get(), NULL, m_RenderTargetView.GetAddressOf()), "Failed to create DirectX Render Traget View");

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
		rd.MultisampleEnable		= m_NumSamples == 1 ? FALSE : TRUE;
		rd.AntialiasedLineEnable	= FALSE;

		CHECK_HR(m_Device->CreateRasterizerState(&rd, m_RasterizerState.GetAddressOf()), "Failed to create Rasterizer state");

		m_DeviceContext->RSSetState(m_RasterizerState.Get());

		return true;
	}

	bool D3DContext::CreateDepthBuffer(const DXGI_MODE_DESC & displayMode) {
		D3D11_DEPTH_STENCIL_DESC dsd;
		ZeroMemory(&dsd, sizeof(dsd));

		dsd.DepthEnable						= TRUE;
		dsd.DepthWriteMask					= D3D11_DEPTH_WRITE_MASK_ALL;
		dsd.DepthFunc						= D3D11_COMPARISON_LESS_EQUAL;
		dsd.StencilEnable					= FALSE;

		CHECK_HR(m_Device->CreateDepthStencilState(&dsd, m_DepthStencilState.GetAddressOf()), "Failed to create Depth Stencil State");

		m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 1u);

		//Move DepthStencilState to render state

		D3D11_TEXTURE2D_DESC dbd;
		ZeroMemory(&dbd, sizeof(dbd));

		dbd.Width					= displayMode.Width;
		dbd.Height					= displayMode.Height;
		dbd.MipLevels				= 1;
		dbd.ArraySize				= 1;
		dbd.Format					= DXGI_FORMAT_D32_FLOAT;
		dbd.SampleDesc.Count		= m_NumSamples;
		dbd.SampleDesc.Quality		= 0;
		dbd.Usage					= D3D11_USAGE_DEFAULT;
		dbd.BindFlags				= D3D11_BIND_DEPTH_STENCIL;
		dbd.CPUAccessFlags			= 0;
		dbd.MiscFlags				= 0;

		CHECK_HR(m_Device->CreateTexture2D(&dbd, NULL, m_DepthStencilBuffer.GetAddressOf()), "Failed to create Depth Stencil Texture2D");

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));

		dsvd.Format					= DXGI_FORMAT_D32_FLOAT;
		if (m_NumSamples > 1)
			dsvd.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2DMS;
		else
			dsvd.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Texture2D.MipSlice		= 0;

		CHECK_HR(m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &dsvd, m_DepthStencilView.GetAddressOf()), "Failed to create Depth Stencil View");
		
		m_DeviceContext->OMSetRenderTargets(1u, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());

		return true;
	}

}