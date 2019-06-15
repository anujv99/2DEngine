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
#include "graphics/bargraph.h"
#include "imgui/imguilayer.h"
#include "imgui/Imgui.h"

#include "math/mat4.h"

namespace prev {

	Application::Application() {
		Timer::FPSCounter(true);
		EventHandler::CreateInst();

		auto dis = GraphicsContext::GetDisplayModes();
		unsigned int selectedDis = 5;
		dis[selectedDis].SetWindowStyle(WindowStyle::WINDOWED);
		Window::CreateInst(dis[selectedDis]);
		EventHandler::Ref().RegisterEventFunction(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		Input::CreateInst();
		MVP::CreateInst();
		GraphicsContext::CreateInst(Window::Ref().GetWindowRawPointer(), Window::Ref().GetDisplayMode());
		RenderState::CreateInst();
		ShaderManager::CreateInst();
		ImmediateGFX::CreateInst();
		ImGuiLayer::CreateInst();


		////////////////////////////////////////TESTING////////////////////////////////////////

		////////////////////////////////////////TESTING////////////////////////////////////////

	}

	Application::~Application() {
		MVP::Ref().Projection().Pop();

		ImGuiLayer::DestroyInst();
		ImmediateGFX::DestroyInst();
		ShaderManager::DestroyInst();
		RenderState::DestroyInst();
		GraphicsContext::DestroyInst();
		MVP::DestroyInst();
		Input::DestroyInst();
		Window::DestroyInst();
		EventHandler::DestroyInst();
	}

	void Application::Run() {
		srand(Timer::GetTime());

		LineGraph lg(0.0f, 50.0f);
		while (m_ApplicationRunning) {
			Timer::Update();

			Window::Ref().PollEvents();

			GraphicsContext::Ref().BeginFrame();

			////////////////////////////////////////TESTING////////////////////////////////////////

			m_LayerStack.OnUpdate();

			ImGuiLayer::Ref().StartFrame();

			m_LayerStack.OnImGuiUpdate();

			ImGuiLayer::Ref().EndFrame();

			////////////////////////////////////////TESTING////////////////////////////////////////

			GraphicsContext::Ref().EndFrame();

			Input::Ref().Update();
			EventHandler::Ref().FlushEventQueue();
		}
	}

	void Application::OnEvent(Event & e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::WindowClosed));

		m_LayerStack.OnEvent(e);
	}

	bool Application::WindowClosed(WindowCloseEvent & e) {
		m_ApplicationRunning = false;
		return true;
	}

}