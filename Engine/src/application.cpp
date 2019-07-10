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

#include "common/profiler.h"

#include "vm/virtualmachine.h"

#include <imgui.h>

#include "renderer/spriterenderer.h"
#include "game/particlesystem.h"
#include "renderer/particlerenderer.h"

#include <Box2D/Box2D.h>
#include "physics/box2dmanager.h"
#include "physics/box2ddebugdraw.h"
#include "graphics/texture2d.h"

extern unsigned int GLOBAL_DRAW_CALL_COUNT;

namespace prev {

	// Just used for initialization
	static constexpr float S_DEFCAMERA_SCALE_Y	=  10.0f;
	static constexpr float S_DEFCAMERA_NEAR		= -150.0f;
	static constexpr float S_DEFCAMERA_FAR		=  150.0f;

	Application::Application() {
		Timer::FPSCounter(false);
		EventHandler::CreateInst();

		auto dis = GraphicsContext::GetDisplayModes();
		unsigned int selectedDis = 0;
		dis[selectedDis].SetWindowStyle(WindowStyle::BORDERLESS);
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

		SpriteRenderer::CreateInst();
		ParticleRenderer::CreateInst();
		Box2DManager::CreateInst();

		Vec2 winSize = ToVec2(Window::Ref().GetDisplayMode().GetWindowSize());
		float aspect = winSize.x / winSize.y;

		float defCameraXScale = aspect * S_DEFCAMERA_SCALE_Y;

		m_DefCamera.SetScreenSpace(
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

		Box2DManager::DestroyInst();
		ParticleRenderer::DestroyInst();
		SpriteRenderer::DestroyInst();
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
			LayerStack::Ref().GetImGuiLayer()->StartFrame();

			LayerStack::Ref().OnUpdate();

			Box2DManager::Ref().Update();

			VirtualMachine::Ref().Update();
			VirtualMachine::Ref().Render();

			////////////////////////////////////////TESTING////////////////////////////////////////

			////////////////////////////////////////TESTING////////////////////////////////////////

			SpriteRenderer::Ref().Render(0);

			PROFILER_BEGIN("App::Gui");
			Gui();
			PROFILER_END("App::Gui");

			LayerStack::Ref().GetImGuiLayer()->EndFrame();
			GraphicsContext::Ref().EndFrame();

			Window::Ref().PollEvents();
			Input::Ref().Update();
			EventHandler::Ref().FlushEventQueue();

			PROFILER_ROOT_END;
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