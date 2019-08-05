#include "pch.h"
#include "d3drenderstate.h"

namespace prev {

	RenderState * RenderState::CreateRenderState() {
		return new D3DRenderSate();
	}

	D3DRenderSate::D3DRenderSate() {
		m_PrimitiveTopology = defaultRenderState.DefaultPrimTopology;
		m_Viewport = defaultRenderState.DefaultViewport;
		m_BlendFunction = defaultRenderState.Blend;

		SetPrimitiveTopology(m_PrimitiveTopology);
		SetViewport(m_Viewport);
		SetBlendFunction(m_BlendFunction);
		DisableScissors();
	}

	void D3DRenderSate::SetPrimitiveTopology(PrimitiveTopology prim) {
		GetDeviceContext()->IASetPrimitiveTopology(MapPrimitiveTopology(prim));
	}

	PrimitiveTopology D3DRenderSate::GetPrimitiveTopology() const {
		return m_PrimitiveTopology;
	}

	void D3DRenderSate::SetViewport(const Viewport & viewport) {
		m_Viewport = viewport;

		D3D11_VIEWPORT vp;
		vp.TopLeftX = viewport.TopLeft.x;
		vp.TopLeftY = viewport.TopLeft.y;
		vp.Width = viewport.Dimension.x;
		vp.Height = viewport.Dimension.y;
		vp.MinDepth = viewport.DepthValues.x;
		vp.MaxDepth = viewport.DepthValues.y;

		GetDeviceContext()->RSSetViewports(1, &vp);
	}

	Viewport D3DRenderSate::GetViewport() const {
		return m_Viewport;
	}

	D3D_PRIMITIVE_TOPOLOGY D3DRenderSate::MapPrimitiveTopology(PrimitiveTopology prim) {
		switch (prim) {
		case PV_PRIM_POINTLIST: return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
		case PV_PRIM_LINELIST: return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
		case PV_PRIM_LINESTRIP: return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
		case PV_PRIM_TRIANGLELIST: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		case PV_PRIM_TRIANGLESTRIP: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		case PV_PRIM_LINELIST_ADJ: return D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
		case PV_PRIM_LINESTRIP_ADJ: return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
		case PV_PRIM_TRIANGLELIST_ADJ: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
		case PV_PRIM_TRIANGLESTRIP_ADJ: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
		default: return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
		}
	}

	D3D11_BLEND D3DRenderSate::MapBlendOption(BlendOption option) {
		switch (option) {
		case prev::PV_BLEND_ZERO: return D3D11_BLEND_ZERO;
		case prev::PV_BLEND_ONE: return D3D11_BLEND_ONE;
		case prev::PV_BLEND_SRC_ALPHA: return D3D11_BLEND_SRC_ALPHA;
		case prev::PV_BLEND_INV_SRC_ALPHA: return D3D11_BLEND_INV_SRC_ALPHA;
		case prev::PV_BLEND_DEST_ALPHA: return D3D11_BLEND_DEST_ALPHA;
		case prev::PV_BLEND_INV_DEST_ALPHA: return D3D11_BLEND_INV_DEST_ALPHA;
		default:
		{
			LOG_ERROR("Invalid blend option : {}", option);
			return D3D11_BLEND_ZERO;
		}
		}
	}

	D3D11_BLEND_OP D3DRenderSate::MapBlendOperation(BlendOperation operation) {
		switch (operation) {
		case prev::PV_BLEND_OP_ADD:	return D3D11_BLEND_OP_ADD;
		case prev::PV_BLEND_OP_SUBTRACT: return D3D11_BLEND_OP_SUBTRACT;
		case prev::PV_BLEND_OP_REV_SUBTRACT: return D3D11_BLEND_OP_REV_SUBTRACT;
		case prev::PV_BLEND_OP_MIN:	return D3D11_BLEND_OP_MIN;
		case prev::PV_BLEND_OP_MAX:	return D3D11_BLEND_OP_MAX;
		default:
		{
			LOG_ERROR("Invalid blend operation : {}", operation);
			return D3D11_BLEND_OP_ADD;
		}
		}
	}

	void D3DRenderSate::SetScissorBox(const ScissorBox & sBox) {
		Vec2i windowSize = Window::Ref().GetDisplayMode().GetWindowSize();

		D3D11_RECT rc;
		rc.left = (long)sBox.Left;
		rc.right = (long)sBox.Right;
		rc.top = (long)windowSize.y - (long)sBox.Top;
		rc.bottom = (long)windowSize.y - (long)sBox.Bottom;

		GetDeviceContext()->RSSetScissorRects(1, &rc);
		m_ScissorBox = sBox;
	}

	ScissorBox D3DRenderSate::GetScissorBox() {
		return m_ScissorBox;
	}

	void D3DRenderSate::DisableScissors() {
		D3D11_RECT rc;
		Vec2i windowSize = Window::Ref().GetDisplayMode().GetWindowSize();

		rc.left = 0;
		rc.right = windowSize.x;
		rc.top = 0;
		rc.bottom = windowSize.y;

		GetDeviceContext()->RSSetScissorRects(1, &rc);
		ZeroMemory(&m_ScissorBox, sizeof(m_ScissorBox));
	}

	void D3DRenderSate::SetBlendFunction(const BlendFunction & blendFunc) {
		//https://stackoverflow.com/questions/14491824/dx11-alpha-blending-when-rendering-to-a-texture

		D3D11_BLEND_DESC bd;
		bd.AlphaToCoverageEnable						= FALSE;
		bd.IndependentBlendEnable						= FALSE;
		bd.RenderTarget[0].BlendEnable					= TRUE;
		bd.RenderTarget[0].SrcBlend						= MapBlendOption(blendFunc.SrcBlend);
		bd.RenderTarget[0].DestBlend					= MapBlendOption(blendFunc.DestBlend);
		bd.RenderTarget[0].BlendOp						= MapBlendOperation(blendFunc.Operation);
		bd.RenderTarget[0].SrcBlendAlpha				= D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha				= D3D11_BLEND_ONE;
		bd.RenderTarget[0].BlendOpAlpha					= D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask		= D3D11_COLOR_WRITE_ENABLE_ALL;

		Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

		HRESULT hr = GetDevice()->CreateBlendState(&bd, blendState.GetAddressOf());
		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Failed to create Blend state");
		}

		GetDeviceContext()->OMSetBlendState(blendState.Get(), nullptr, 0xffffffff);

		m_BlendFunction = blendFunc;
	}

	void D3DRenderSate::DisableBlend() {
		D3D11_BLEND_DESC bd = {};
		bd.AlphaToCoverageEnable						= FALSE;
		bd.IndependentBlendEnable						= FALSE;
		bd.RenderTarget[0].BlendEnable					= FALSE;

		Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

		HRESULT hr = GetDevice()->CreateBlendState(&bd, blendState.GetAddressOf());
		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Failed to create Blend state");
		}

		GetDeviceContext()->OMSetBlendState(blendState.Get(), nullptr, 0xffffffff);
	}

	prev::BlendFunction D3DRenderSate::GetBlendFunction() {
		return m_BlendFunction;
	}

}