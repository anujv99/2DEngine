#pragma once

#include <d3d11.h>
#include <wrl.h>

#include "graphics/graphicscontext.h"

namespace prev {

	class D3DContext : public GraphicsContext {
	public:
		D3DContext(HWND hWnd, const DisplayMode & displayMode);

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		inline ID3D11Device * GetDevice() { return m_Device.Get(); }
		inline ID3D11DeviceContext * GetDeviceContext() { return m_DeviceContext.Get(); }

		void BindDefaultRenderTarget();
	private:
		DXGI_MODE_DESC GetDisplayModeDesc(const DisplayMode & displayMode);

		bool InitializeD3D(const DXGI_MODE_DESC & displayMode, HWND windowHandle);
		bool CreateRenderTargetView(const DXGI_MODE_DESC & displayMode);
		bool CreateRasterizerState(const DXGI_MODE_DESC & displayMode);
		bool CreateDepthBuffer(const DXGI_MODE_DESC & displayMode);
	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain>				m_SwapChain;
		Microsoft::WRL::ComPtr<ID3D11Device>				m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>			m_DeviceContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_RenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>		m_RasterizerState;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_DepthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		m_DepthStencilState;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		m_DepthStencilView;
	private:
		unsigned int m_NumSamples;
	};

}
