#include "pch.h"
#include "framebufferstack.h"

#include "math/vecconversion.h"

#include "window.h"
#include "renderer/fbopass.h"

namespace prev {

	FramebufferStack::FramebufferStack() {
		m_BaseFBO = Framebuffer::CreateFramebuffer();
		m_BaseFBO->Init(ToVec2(Window::Ref().GetDisplayMode().GetWindowSize()), PV_TEXTURE_FORMAT_RGBA8, FRAMEBUFFER_ALLOCATE_DEPTH_BUFFER | FRAMEBUFFER_MSAA_4x);
		m_FBOStack.push(m_BaseFBO);
		EventHandler::Ref().RegisterEventFunction(BIND_EVENT_FN(FramebufferStack::OnEvent));
	}

	FramebufferStack::~FramebufferStack() {
		ASSERT(m_FBOStack.size() == 1ull);
		m_FBOStack.pop();
	}

	void FramebufferStack::Start() {
		ASSERT(m_FBOStack.size() == 1ull);
		m_FBOStack.top()->Bind();
		m_FBOStack.top()->Clear();
	}

	void FramebufferStack::End() {
		ASSERT(m_FBOStack.size() == 1ull);
		m_FBOStack.top()->UnBind();
	}

	void FramebufferStack::Present() {
		FramebufferPass::Ref().Pass(m_FBOStack.top());
	}

	void FramebufferStack::OnEvent(Event & e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(FramebufferStack::OnWindowResize));
	}

	bool FramebufferStack::OnWindowResize(WindowResizeEvent & e) {
		ASSERT(m_FBOStack.size() == 1ull);
		m_FBOStack.pop();
		m_BaseFBO = Framebuffer::CreateFramebuffer();
		m_BaseFBO->Init(ToVec2(e.GetWindowSize()), PV_TEXTURE_FORMAT_RGBA8, FRAMEBUFFER_ALLOCATE_DEPTH_BUFFER | FRAMEBUFFER_MSAA_4x);
		m_FBOStack.push(m_BaseFBO);
		return false;
	}

}