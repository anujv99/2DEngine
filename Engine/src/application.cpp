#include "pch.h"
#include "application.h"

#include "common/log.h"

#include "src/graphics/window.h"

#include "math/vec3.h"
#include "math/vec2i.h"

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
			LOG_WARN("{}", pos);

		}
	}

}