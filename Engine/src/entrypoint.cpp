#include "pch.h"

#include "application.h"
#include "common/log.h"

#include "src/common/timer.h"

namespace prev {

	int MAIN_METHOD() {
		Application * app = Application::CreateApplication();
		LOG_INFO("Application Created");
		app->Run();
		delete app;
		return 0;
	}

}

int main() {
	prev::Logger::Initialize(); // To be initialized at first
	return prev::MAIN_METHOD();
}