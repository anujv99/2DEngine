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

		virtual void SetBlendFunction(const BlendFunction & blendFunc) override;
		virtual void DisableBlend() override;
		virtual BlendFunction GetBlendFunction() override;

		virtual void DisableDepthWrite() override;
		virtual void EnableDepthWrite() override;
	private:
		D3D_PRIMITIVE_TOPOLOGY MapPrimitiveTopology(PrimitiveTopology prim);
		D3D11_BLEND MapBlendOption(BlendOption option);
		D3D11_BLEND_OP MapBlendOperation(BlendOperation operation);
	private:
		PrimitiveTopology	m_PrimitiveTopology;
		Viewport			m_Viewport;
		ScissorBox			m_ScissorBox;
		BlendFunction		m_BlendFunction;

		//Used to store disabled depth buffer;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
	};

}
