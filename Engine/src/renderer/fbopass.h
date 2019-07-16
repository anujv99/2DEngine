#pragma once

#include "graphics/framebuffer.h"
#include "graphics/vertexbuffer.h"
#include "graphics/shadermanager.h"
#include "graphics/vertexlayout.h"

namespace prev {

	class FramebufferPass : public Singleton<FramebufferPass> {
		friend class Singleton<FramebufferPass>;
	private:
		FramebufferPass();
		~FramebufferPass();
	private:
		void LoadShaders();
		void CreateVertexBuffer();
		void CreateVertexLayout();
	public:
		void Pass(StrongHandle<Framebuffer> fbo, StrongHandle<VertexShader> vShader = nullptr, StrongHandle<PixelShader> pShader = nullptr);
	public:
		StrongHandle<VertexShader> m_DefaultFBOVertexShader;
		StrongHandle<PixelShader> m_DefaultFBOPixelShader;
		StrongHandle<VertexBuffer> m_VertexBuffer;
		StrongHandle<VertexLayout> m_VertexLayout;
	};

}
