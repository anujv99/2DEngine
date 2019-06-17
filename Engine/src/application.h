#pragma once

#include "utils/layerstack.h"

namespace prev {

	class Application : public HandledObject<Application> {
	public:
		Application();
		~Application();

		void Run();
	private:
		void Gui();
	private:
		void OnEvent(Event & e);
		bool WindowClosed(WindowCloseEvent & e);
	private:
		bool m_ApplicationRunning = true;
	};

}