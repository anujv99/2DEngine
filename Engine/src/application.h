#pragma once

namespace prev {

	class Application : public HandledObject<Application> {
		friend int MAIN_METHOD();
	public:
		Application();
		~Application();

		void Run();
	private:
		void OnEvent(Event & e);
		bool WindowClosed(WindowCloseEvent & e);
	private:
		bool m_ApplicationRunning = true;
	};

}