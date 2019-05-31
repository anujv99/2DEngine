#include "pch.h"
#include "application.h"

#include "common/log.h"

#include "src/graphics/window.h"

namespace prev {

	Application::Application() {
		auto dis = DisplayMode();
		Window::CreateInst(dis);
	}

	Application::~Application() {
		Window::DestroyInst();
	}

	void Application::Run() {
		while (true) {
			Window::Ref().PollEvents();

			auto pos = Window::Ref().GetMousePosition();
			LOG_TRACE("{0}, {1}", pos.first, pos.second);

		}
	}

}