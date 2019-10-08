#include "common/memoryleak.inl"
#include "pch.h"

#include "application.h"

using namespace prev;

//int WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd ) {
int main(_In_opt_ int argc, _In_opt_ char * argv[]) {
	//_CrtSetBreakAlloc(86698);

	PrevMemoryInit();

	Logger::CreateInst();

	LOG_WARN("This is the application!");

	Application::CreateInst();
	Application::Ref().Run();
	Application::DestroyInst();

	Logger::DestroyInst();

	return 0;
}