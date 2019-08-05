#include "pch.h"
#include "application.h"

#include "utils/input.h"

#include "graphics/window.h"
#include "graphics/graphicscontext.h"
#include "graphics/renderstate.h"
#include "graphics/immediategfx.h"
#include "graphics/shadermanager.h"

#include "math/mvp.h"
#include "math/mat4.h"
#include "math/screenspace.h"

#include "common/profiler.h"

#include "vm/virtualmachine.h"

#include "renderer/renderer.h"
#include "renderer/fbopass.h"

#include "graphics/texture2d.h"
#include "graphics/font.h"
#include "imgui.h"

extern unsigned int GLOBAL_DRAW_CALL_COUNT;

namespace prev {

	Application::Application() {

		Timer::FPSCounter(false);
		EventHandler::CreateInst();

		auto dis = GraphicsContext::GetDisplayModes();
		unsigned int selectedDis = 0;
		dis[selectedDis].SetWindowStyle(WindowStyle::BORDERLESS);
		dis[selectedDis].SetMultisample(4);
		Window::CreateInst(dis[selectedDis]);
		EventHandler::Ref().RegisterEventFunction(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		Input::CreateInst();
		MVP::CreateInst();
		GraphicsContext::CreateInst(Window::Ref().GetWindowRawPointer(), Window::Ref().GetDisplayMode());
		RenderState::CreateInst();
		ShaderManager::CreateInst();
		ImmediateGFX::CreateInst();
		LayerStack::CreateInst();
		//ImGui Layer

		m_ImGuiLayer = new ImGuiLayer();
		LayerStack::Ref().PushLayer(m_ImGuiLayer);
		Profiler::CreateInst(); // Because profiler use imgui layer

		VirtualMachine::CreateInst();
		VirtualMachine::Ref().RunMain();

		Renderer::CreateInst();
		FramebufferPass::CreateInst();

		Vec2 winSize = ToVec2(Window::Ref().GetDisplayMode().GetWindowSize());
		float aspect = winSize.x / winSize.y;

		float defCameraXScale = aspect * S_DEFCAMERA_SCALE_Y;

		m_DefCamera.SetScreenSpace (
			Vec2(-defCameraXScale / 2, -S_DEFCAMERA_SCALE_Y / 2),
			Vec2(defCameraXScale / 2, S_DEFCAMERA_SCALE_Y / 2)
		);

		m_DefCamera.SetNearFar(S_DEFCAMERA_NEAR, S_DEFCAMERA_FAR);
		m_DefCamera.Begin();

		////////////////////////////////////////TESTING////////////////////////////////////////
		
		////////////////////////////////////////TESTING////////////////////////////////////////
	}

	Application::~Application() {
		m_DefCamera.End();

		FramebufferPass::DestroyInst();
		Renderer::DestroyInst();
		VirtualMachine::DestroyInst();
		Profiler::DestroyInst();
		LayerStack::DestroyInst();
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
		while (m_ApplicationRunning) {
			PROFILER_ROOT_BEGIN;

			Timer::Update();

			GraphicsContext::Ref().BeginFrame();
			
			if (LayerStack::Ref().GetImGuiLayer())
				LayerStack::Ref().GetImGuiLayer()->StartFrame();

			LayerStack::Ref().OnUpdate();

			VirtualMachine::Ref().Update();
			VirtualMachine::Ref().Render();

			//////////////////////////////////////TESTING////////////////////////////////////////

			//////////////////////////////////////TESTING////////////////////////////////////////

			PROFILER_BEGIN("App::Present");
			Renderer::Ref().Present();
			PROFILER_END("App::Present");

			PROFILER_BEGIN("App::Gui");
			Gui();
			PROFILER_END("App::Gui");

			if (LayerStack::Ref().GetImGuiLayer())
				LayerStack::Ref().GetImGuiLayer()->EndFrame();
 
			GraphicsContext::Ref().EndFrame();

			Window::Ref().PollEvents();
			Input::Ref().Update();
			EventHandler::Ref().FlushEventQueue();

			PROFILER_ROOT_END;

			//LOG_INFO("Draw Calls This Frame : {}", GLOBAL_DRAW_CALL_COUNT);
			GLOBAL_DRAW_CALL_COUNT = 0;

		}
	}

	void Application::Gui() {
		VirtualMachine::Ref().Gui();
		LayerStack::Ref().OnImGuiUpdate();
	}

	void Application::OnEvent(Event & e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::WindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::WindowResized));

		LayerStack::Ref().OnEvent(e);
	}

	bool Application::WindowClosed(WindowCloseEvent & e) {
		m_ApplicationRunning = false;
		return true;
	}

	bool Application::WindowResized(WindowResizeEvent & e) {
		Vec2 winSize = ToVec2(e.GetWindowSize());
		float aspect = winSize.x / winSize.y;

		float defCameraXScale = aspect * S_DEFCAMERA_SCALE_Y;

		m_DefCamera.SetScreenSpace(
			Vec2(-defCameraXScale / 2, -S_DEFCAMERA_SCALE_Y / 2),
			Vec2(defCameraXScale / 2, S_DEFCAMERA_SCALE_Y / 2)
		);

		m_DefCamera.SetNearFar(S_DEFCAMERA_NEAR, S_DEFCAMERA_FAR);
		m_DefCamera.End();
		m_DefCamera.Begin();

		return false;
	}

}