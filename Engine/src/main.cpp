#include "pch.h"

#include "application.h"

using namespace prev;

int main() {
	//_CrtSetBreakAlloc(382);

	PrevMemoryInit();

	Logger::CreateInst();

	LOG_WARN("This is the application!");

	Application * app = new Application();
	app->Run();
	delete app;

	Logger::DestroyInst();

	return 0;
}