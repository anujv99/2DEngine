#include "pch.h"
#include "application.h"

#include "common/log.h"

#include "src/graphics/window.h"
#include "src/utils/input.h"

#include "math/vec4.h"
#include "math/vec3.h"
#include "math/vec2.h"

namespace prev {

	Application::Application() {
		Logger::CreateInst();
		EventHandler::CreateInst();

		auto dis = DisplayMode();
		Window::CreateInst(dis);
		EventHandler::Ref().RegisterEventFunction(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		Input::CreateInst();
	}

	Application::~Application() {
		Input::DestroyInst();
		Window::DestroyInst();
		EventHandler::DestroyInst();
		Logger::DestroyInst();
	}

	void Application::Run() {
		while (m_ApplicationRunning) {
			Window::Ref().PollEvents();



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