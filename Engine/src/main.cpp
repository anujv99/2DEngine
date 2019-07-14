#include "pch.h"

#include "application.h"

using namespace prev;

int main(_In_opt_ int argc, _In_opt_ char * argv[]) {
	//_CrtSetBreakAlloc(24006);

	PrevMemoryInit();

	Logger::CreateInst();

	LOG_WARN("This is the application!");

	Application::CreateInst();
	Application::Ref().Run();
	Application::DestroyInst();
	
	Logger::DestroyInst();

	return 0;
}