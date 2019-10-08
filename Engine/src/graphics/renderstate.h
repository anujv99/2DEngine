#pragma once

#include "window.h"
#include "viewport.h"
#include "uniform.h"

#include "math/mvp.h"
#include "math/vecconversion.h"

#include "datatypes.h"

namespace prev {

	static const unsigned int RESERVED_MVP_UNIFORM_SLOT = 0;

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
			DefaultPrimTopology(PV_PRIM_TRIANGLELIST) {
			auto windowSize = Window::Ref().GetDisplayMode().GetWindowSize();

			DefaultViewport.TopLeft = Vec2(0.0f, 0.0f);
			DefaultViewport.Dimension = ToVec2(windowSize);
			DefaultViewport.DepthValues = Vec2(0.0f, 1.0f);

			Blend.SrcBlend = PV_BLEND_SRC_ALPHA;
			Blend.DestBlend = PV_BLEND_INV_SRC_ALPHA;
			Blend.Operation = PV_BLEND_OP_ADD;
		}

		PrimitiveTopology DefaultPrimTopology;
		Viewport DefaultViewport;
		BlendFunction Blend;
	};

	class RenderState : public Singleton<RenderState> {
		friend class Singleton<RenderState>;
	protected:
		RenderState() {
			m_Uniform = Uniform::CreateUniform();
			m_Uniform->Init(&MVP::Ref().Get(), sizeof(Mat4), RESERVED_MVP_UNIFORM_SLOT, VERTEX_SHADER);
			m_Uniform->Bind();
		}
		virtual ~RenderState() {}
	public:
		StrongHandle<Uniform> & GetMVPUniform() { return m_Uniform; }
	public:
		//Primitive Topology
		virtual void SetPrimitiveTopology(PrimitiveTopology prim) = 0;
		virtual PrimitiveTopology GetPrimitiveTopology() const = 0;

		//Viewport
		virtual void SetViewport(const Viewport & viewport) = 0;
		virtual Viewport GetViewport() const = 0;
		
		//Scissors
		virtual void SetScissorBox(const ScissorBox & sBox) = 0;
		virtual ScissorBox GetScissorBox() = 0;
		virtual void DisableScissors() = 0;

		//Blending
		virtual void SetBlendFunction(const BlendFunction & blendFunc) = 0;
		virtual void DisableBlend() = 0;
		virtual BlendFunction GetBlendFunction() = 0;
	private:
		StrongHandle<Uniform> m_Uniform;
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
