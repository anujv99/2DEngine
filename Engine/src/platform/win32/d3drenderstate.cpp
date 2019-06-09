#include "pch.h"
#include "d3drenderstate.h"

namespace prev {

	RenderState * RenderState::CreateRenderState() {
		return new D3DRenderSate();
	}

	D3DRenderSate::D3DRenderSate() {
		m_PrimitiveTopology = defaultRenderState.DefaultPrimTopology;
		m_Viewport = defaultRenderState.DefaultViewport;

		SetPrimitiveTopology(m_PrimitiveTopology);
		SetViewport(m_Viewport);
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

}