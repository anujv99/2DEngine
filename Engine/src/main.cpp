#include "pch.h"

#include "application.h"

using namespace prev;

class Sandbox : public Application {
public:
	Sandbox() {

	}
};

Application * Application::CreateApplication() {
	return new Sandbox();
}
