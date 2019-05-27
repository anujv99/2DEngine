#pragma once

namespace prev {

	class Application {
		friend int MAIN_METHOD();
	public:
		Application();
		~Application();
	private:
		void Run();
	public:
		static Application * CreateApplication();
	};

}