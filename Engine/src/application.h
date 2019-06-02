#pragma once

namespace prev {

	class Application {
		friend int MAIN_METHOD();
	public:
		Application();
		~Application();
	private:
		void Run();
		void OnEvent(Event & e);

		bool WindowClosed(WindowCloseEvent & e);
	public:
		static Application * CreateApplication();
	private:
		bool m_ApplicationRunning = true;
	};

}