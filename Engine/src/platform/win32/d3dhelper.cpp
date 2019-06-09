#include "pch.h"
#include "d3dhelper.h"

namespace prev {

	ID3D11Device * GetDevice() {
		D3DContext * d3dContext = (D3DContext *)GraphicsContext::Get();
		return d3dContext->GetDevice();
	}

	ID3D11DeviceContext * GetDeviceContext() {
		D3DContext * d3dContext = (D3DContext *)GraphicsContext::Get();
		return d3dContext->GetDeviceContext();
	}

}