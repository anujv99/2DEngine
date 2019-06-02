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
	public:
		static Application * CreateApplication();
	};

}