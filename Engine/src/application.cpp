#include "pch.h"
#include "application.h"

#include "utils/input.h"

#include "graphics/font.h"
#include "graphics/window.h"
#include "graphics/texture2d.h"
#include "graphics/renderstate.h"
#include "graphics/tiledtexture.h"
#include "graphics/immediategfx.h"
#include "graphics/computebuffer.h"
#include "graphics/shadermanager.h"
#include "graphics/imagecomponent.h"
#include "graphics/graphicscontext.h"

#include "math/mat4.h"
#include "math/mvp.h"
#include "math/camera.h"

#include "common/profiler.h"

#include "vm/virtualmachine.h"

#include "renderer/renderer.h"
#include "renderer/fbopass.h"

#include "physics/box2ddebugdraw.h"
#include "physics/box2dmanager.h"

#include "game/square.h"
#include "game/cameracontroller.h"

#include "audio/fmod/fmodaudio.h"

//-----------------TEMP------------------
#include "imgui.h"
//---------------------------------------

extern unsigned int GLOBAL_DRAW_CALL_COUNT;

namespace prev {

	StrongHandle<Sound> sound;

	Application::Application() {

		Timer::FPSCounter(false);
		EventHandler::CreateInst();

		auto dis = GraphicsContext::GetDisplayModes();
		unsigned int selectedDis = 5;
		dis[selectedDis].SetWindowStyle(WINDOW_STYLE_BORDERLESS);
		dis[selectedDis].SetMultisample(8);
		Window::CreateInst(dis[selectedDis]);
		EventHandler::Ref().RegisterEventFunction(BIND_EVENT_FN(Application::OnEvent));

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
		Box2DManager::CreateInst();
		ImageComponent::CreateInst();
		FMODAudio::CreateInst();
		CameraController::CreateInst();

		////////////////////////////////////////TESTING////////////////////////////////////////

		sound = FMODAudio::Ref().LoadSound("res/audio/test.wav");
		sound->SetLoopCount(LOOP_FOREVER);

		////////////////////////////////////////TESTING////////////////////////////////////////

	}

	Application::~Application() {

		sound = nullptr;

		CameraController::DestroyInst();
		FMODAudio::DestroyInst();
		ImageComponent::DestroyInst();
		Box2DManager::DestroyInst();
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

			CameraController::Ref().Begin();

			//////////////////////////////////////TESTING////////////////////////////////////////

			if (m_IsWindowReiszed) {
				Vec2 winSize = ToVec2(Window::Ref().GetDisplayMode().GetWindowSize());

				Viewport v;
				v.TopLeft = Vec2(0.0f);
				v.DepthValues = Vec2(0.0f, 1.0f);
				v.Dimension = winSize;

				GraphicsContext::Ref().ChangeResolution(ToVec2i(winSize));
				RenderState::Ref().SetViewport(v);
				RenderState::Ref().DisableScissors();

				m_IsWindowReiszed = false;
			}

			//////////////////////////////////////TESTING////////////////////////////////////////

			Timer::Update();

			GraphicsContext::Ref().BeginFrame();
			
			if (LayerStack::Ref().GetImGuiLayer())
				LayerStack::Ref().GetImGuiLayer()->StartFrame();

			LayerStack::Ref().OnUpdate();

			Box2DManager::Ref().Update();

			VirtualMachine::Ref().Update();
			VirtualMachine::Ref().Render();

			//////////////////////////////////////TESTING////////////////////////////////////////

			FMODAudio::Ref().Update();
			CameraController::Ref().Update();

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

			CameraController::Ref().End();

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
		m_IsWindowReiszed = true;
		return false;
	}

}