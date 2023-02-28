#pragma once

#include "vertexbuffer.h"
#include "shadermanager.h"
#include "vertexbatcher.h"

namespace prev {

	class ImmediateGFX : public VertexBatcher, public Singleton<ImmediateGFX> {
		friend Singleton<ImmediateGFX>;
	public:
		void BeginDefaultShaders(bool bindPixelShader = true);
		void EndDefaultShaders();

		void BeginDraw();
		void EndDraw();

		virtual void PolygonBegin(PrimitiveTopology primitive) override;
		virtual void PolygonEnd(bool bindPixelShader = true) override;
	private:
		ImmediateGFX();
		~ImmediateGFX();

		void InitializeVertexBuffer();
		void DrawCurrentPrimitive(bool bindPixelShader = true);
	private:
		StrongHandle<VertexShader> m_VertexShader;
		StrongHandle<PixelShader> m_PixelShader;

		StrongHandle<VertexLayout> m_VertexLayout;

		StrongHandle<VertexBuffer> m_VertexBuffer;
		unsigned int m_BufferIndex;

		bool m_IsBuilding;
		PrimitiveTopology m_DrawPrimitive;
	};

}
