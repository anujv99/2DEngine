#pragma once

#include "d3dhelper.h"
#include "graphics/renderstate.h"

namespace prev {

	class D3DRenderSate : public RenderState {
	public:
		D3DRenderSate();

		// Inherited via RenderState
		virtual void SetPrimitiveTopology(PrimitiveTopology prim) override;
		virtual PrimitiveTopology GetPrimitiveTopology() const override;

		virtual void SetViewport(const Viewport & viewport) override;
		virtual Viewport GetViewport() const override;

		virtual void SetScissorBox(const ScissorBox & sBox) override;
		virtual ScissorBox GetScissorBox() override;
		virtual void DisableScissors() override;
	private:
		D3D_PRIMITIVE_TOPOLOGY MapPrimitiveTopology(PrimitiveTopology prim);
	private:
		PrimitiveTopology m_PrimitiveTopology;
		Viewport m_Viewport;
		ScissorBox m_ScissorBox;
	};

}
