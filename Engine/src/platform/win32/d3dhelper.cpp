#include "pch.h"
#include "d3dhelper.h"

namespace prev {

	ID3D11Device * GetDevice() {
		D3DContext * d3dContext = dynamic_cast<D3DContext *>(GraphicsContext::Get());
		return d3dContext->GetDevice();
	}

	ID3D11DeviceContext * GetDeviceContext() {
		D3DContext * d3dContext = dynamic_cast<D3DContext *>(GraphicsContext::Get());
		return d3dContext->GetDeviceContext();
	}

	DXGI_FORMAT GetTextureFormat(TextureFormat texFormat) {
		switch (texFormat) {
		case PV_TEXTURE_FORMAT_RGBA8: return DXGI_FORMAT_R8G8B8A8_UNORM;
		case PV_TEXTURE_FORMAT_R32F: return DXGI_FORMAT_R32_FLOAT;
		case PV_TEXTURE_FORMAT_R32SINT: return DXGI_FORMAT_R32_SINT;
		default: return DXGI_FORMAT_UNKNOWN;
		}
	}

	TextureFormat GetTextureFormat(DXGI_FORMAT texFormat) {
		switch (texFormat) {
		case DXGI_FORMAT_R8G8B8A8_UNORM: return PV_TEXTURE_FORMAT_RGBA8;
		case DXGI_FORMAT_R32_FLOAT: return PV_TEXTURE_FORMAT_R32F;
		case DXGI_FORMAT_R32_SINT: return PV_TEXTURE_FORMAT_R32SINT;
		default: return PV_TEXTURE_FORMAT_UNKNOWN;
		}
	}

}