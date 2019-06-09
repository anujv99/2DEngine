#include "pch.h"
#include "application.h"

#include "common/log.h"

#include "graphics/window.h"
#include "graphics/graphicscontext.h"
#include "utils/input.h"

#include "graphics/renderstate.h"

#include "graphics/uniform.h"
#include "graphics/vertexlayout.h"
#include "graphics/pixelshader.h"
#include "graphics/vertexbufferbuilder.h"
#include "graphics/immediategfx.h"

#include "graphics/shadermanager.h"

namespace prev {

	StrongHandle<Uniform> uBuffer;

	Application::Application() {
		Timer::FPSCounter(true);
		EventHandler::CreateInst();

		auto dis = GraphicsContext::GetDisplayModes();
		Window::CreateInst(dis[5]);
		EventHandler::Ref().RegisterEventFunction(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		Input::CreateInst();
		GraphicsContext::CreateInst(Window::Ref().GetWindowRawPointer(), Window::Ref().GetDisplayMode());
		RenderState::CreateInst();
		ShaderManager::CreateInst();

		////////////////////////////////////////TESTING////////////////////////////////////////
		
		ImmediateGFX::CreateInst();

		////////////////////////////////////////TESTING////////////////////////////////////////

	}

	Application::~Application() {
		ImmediateGFX::DestroyInst();
		ShaderManager::DestroyInst();
		RenderState::DestroyInst();
		GraphicsContext::DestroyInst();
		Input::DestroyInst();
		Window::DestroyInst();
		EventHandler::DestroyInst();
	}

	void Application::Run() {
		while (m_ApplicationRunning) {
			Timer::Update();

			Window::Ref().PollEvents();

			GraphicsContext::Ref().BeginFrame();

			ImmediateGFX::Ref().DrawLine(Vec2(-1, 1), Vec2(1, -1));

			if (Input::Ref().IsKeyDown('A'))
				ImmediateGFX::Ref().DrawLine(Vec2(1, 1), Vec2(-1, -1));

			GraphicsContext::Ref().EndFrame();

			Input::Ref().Update();
			EventHandler::Ref().FlushEventQueue();
		}
	}

	void Application::OnEvent(Event & e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::WindowClosed));
	}

	bool Application::WindowClosed(WindowCloseEvent & e) {
		m_ApplicationRunning = false;
		return true;
	}

}