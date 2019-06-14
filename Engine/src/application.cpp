#include "pch.h"
#include "application.h"

#include "common/log.h"

#include "graphics/window.h"
#include "graphics/graphicscontext.h"
#include "utils/input.h"

#include "graphics/renderstate.h"

#include "graphics/uniform.h"
#include "graphics/vertexlayout.h"
#include "graphics/pixelshader.h"
#include "graphics/vertexbufferbuilder.h"
#include "graphics/immediategfx.h"
#include "graphics/shadermanager.h"
#include "math/mvp.h"
#include "graphics/texture2d.h"
#include "graphics/sampler2d.h"
#include "graphics/font.h"
#include "graphics/linegraph.h"
#include "imgui/imguimanager.h"
#include "imgui/imgui.h"

#include "math/mat4.h"

namespace prev {

	LineGraph lineGraph(0.0f, 50.0f);

	Application::Application() {
		Timer::FPSCounter(true);
		EventHandler::CreateInst();

		auto dis = GraphicsContext::GetDisplayModes();
		Window::CreateInst(dis[5]);
		EventHandler::Ref().RegisterEventFunction(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		Input::CreateInst();
		MVP::CreateInst();
		GraphicsContext::CreateInst(Window::Ref().GetWindowRawPointer(), Window::Ref().GetDisplayMode());
		RenderState::CreateInst();
		ShaderManager::CreateInst();
		ImmediateGFX::CreateInst();

		////////////////////////////////////////TESTING////////////////////////////////////////

		ImGuiManager::CreateInst();

		Mat4 projection = Ortho(0, 1280, 0, 720, -150, 150);
		MVP::Ref().Projection().Push();
		MVP::Ref().Projection().Load(projection);

		////////////////////////////////////////TESTING////////////////////////////////////////

	}

	Application::~Application() {
		MVP::Ref().Projection().Pop();

		ImGuiManager::DestroyInst();
		ImmediateGFX::DestroyInst();
		ShaderManager::DestroyInst();
		RenderState::DestroyInst();
		GraphicsContext::DestroyInst();
		MVP::DestroyInst();
		Input::DestroyInst();
		Window::DestroyInst();
		EventHandler::DestroyInst();
	}

	Vec2 pos = Vec2(0, 0);

	void Application::Run() {
		while (m_ApplicationRunning) {
			Timer::Update();

			Window::Ref().PollEvents();

			GraphicsContext::Ref().BeginFrame();

			////////////////////////////////////////TESTING////////////////////////////////////////

			ImmediateGFX::Ref().BeginDefaultShaders();

			ImGuiManager::Ref().PreUpdate();
			ImGuiManager::Ref().DetectConsumeInputs();

			static bool ispressed = false;
			static float radius = 1.0f;
			static int high = -1;

			lineGraph.PushValue(Timer::GetDeltaTime() * 1000.0f);

			ImmediateGFX::Ref().Color(0.5, 0.1, 0.8, 1);
			ImmediateGFX::Ref().DrawRectRounded(Vec2(500, 300), Vec2(100, 100), 1.0f);

			ImGui::Begin("Retarded Window");
			ImGui::SliderFloat("DickLength", radius, -10.0f, 10.0f);
			ImGui::SliderInt("Hello Penis", high, -1, 10);

			ImGui::Separator();

			ImGui::Print("Delta Time");
			ImGui::LineGraph(lineGraph);

			ImGui::Print("Hello World What are you doing");

			float arr[] = {0, 1, 2, 3, 4, 5, 6, radius, 1, 3};

			ImGui::BarGraph(arr, 10, 0, 10, high);

			ImGui::End();

			ImGuiManager::Ref().PostUpdate();

			////////////////////////////////////////TESTING////////////////////////////////////////

			GraphicsContext::Ref().EndFrame();

			Input::Ref().Update();
			EventHandler::Ref().FlushEventQueue();
		}
	}

	void Application::OnEvent(Event & e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::WindowClosed));
	}

	bool Application::WindowClosed(WindowCloseEvent & e) {
		m_ApplicationRunning = false;
		return true;
	}

}