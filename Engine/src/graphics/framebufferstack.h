#pragma once

#include "framebuffer.h"

namespace prev {

	class FramebufferStack : public Singleton<FramebufferStack> {
		friend class Singleton<FramebufferStack>;
	private:
		FramebufferStack();
		~FramebufferStack();
	public:
		void Start();
		void End();
		void Present();

		void OnEvent(Event & e);
		bool OnWindowResize(WindowResizeEvent & e);
	private:
		std::stack<StrongHandle<Framebuffer>> m_FBOStack;
		StrongHandle<Framebuffer> m_BaseFBO;
	};

}
