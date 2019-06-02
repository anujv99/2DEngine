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
		auto dis = DisplayMode();
		Window::CreateInst(dis);
		EventHandler::RegisterEventFunction(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		Input::CreateInst();
	}

	Application::~Application() {
		Input::DestroyInst();
		Window::DestroyInst();
	}

	void Application::Run() {
		while (true) {
			Window::Ref().PollEvents();
			
			

			Input::Ref().Update();
			EventHandler::FlushEventQueue();
		}
	}

	void Application::OnEvent(Event & e) {
		//LOG_TRACE("{}", e);
	}

}