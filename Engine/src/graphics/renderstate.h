#pragma once

#include "window.h"
#include "viewport.h"
#include "vertexshader.h"
#include "pixelshader.h"

namespace prev {

	enum PrimitiveTopology {
		PV_PRIM_POINTLIST,
		PV_PRIM_LINELIST,
		PV_PRIM_LINESTRIP,
		PV_PRIM_TRIANGLELIST,
		PV_PRIM_TRIANGLESTRIP,
		PV_PRIM_LINELIST_ADJ,
		PV_PRIM_LINESTRIP_ADJ,
		PV_PRIM_TRIANGLELIST_ADJ,
		PV_PRIM_TRIANGLESTRIP_ADJ,
	};

	struct DefaultRenderSate {
		DefaultRenderSate() :
			DefaultPrimTopology(PV_PRIM_TRIANGLELIST)
		{
			auto windowSize = Window::Ref().GetDisplayMode().GetWindowSize();
			DefaultViewport.TopLeft = Vec2(0.0f, 0.0f);
			DefaultViewport.Dimension = Vec2((float)windowSize.x, (float)windowSize.y);
			DefaultViewport.DepthValues = Vec2(0.0f, 1.0f);
		}

		PrimitiveTopology DefaultPrimTopology;
		Viewport DefaultViewport;
	};

	class RenderState : public Singleton<RenderState> {
		friend class Singleton<RenderState>;
	protected:
		RenderState() {}
		virtual ~RenderState() {}
	public:
		//Primitive Topology
		virtual void SetPrimitiveTopology(PrimitiveTopology prim) = 0;
		virtual PrimitiveTopology GetPrimitiveTopology() const = 0;

		//Viewport
		virtual void SetViewport(const Viewport & viewport) = 0;
		virtual Viewport GetViewport() const = 0;

		virtual void SetScissorBox(const ScissorBox & sBox) = 0;
		virtual ScissorBox GetScissorBox() = 0;
		virtual void DisableScissors() = 0;
	private:
		friend PixelShader;
		friend VertexShader;

		StrongHandle<VertexShader> m_BoundVertexShader;
		StrongHandle<PixelShader> m_BoundPixelShader;
	private:
		static RenderState * CreateRenderState();
	protected:
		const DefaultRenderSate defaultRenderState;
	};

	template<>
	template<>
	inline void Singleton<RenderState>::CreateInst() {
		if(!s_Instance) s_Instance = RenderState::CreateRenderState();
	}

}
