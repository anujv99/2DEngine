#include "pch.h"

#include "application.h"

using namespace prev;

int main() {
	PrevMemoryInit();

	Logger::CreateInst();

	LOG_WARN("This is the application!");

	Application::CreateInst();
	Application::Ref().Run();
	Application::DestroyInst();
	
	Logger::DestroyInst();

	return 0;
}