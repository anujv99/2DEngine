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
#include "graphics/framebufferstack.h"

#include "math/mat4.h"
#include "math/mvp.h"
#include "math/camera.h"
#include "math/spline.h"

#include "common/profiler.h"

#include "vm/virtualmachine.h"

#include "renderer/renderer.h"
#include "renderer/fbopass.h"

#include "physics/box2dmanager.h"
#include "physics/box2ddebugdraw.h"

#include "game/square.h"
#include "game/cameracontroller.h"

#include "audio/fmod/fmodaudio.h"

//-----------------TEMP------------------
#ifdef IMGUI_ENABLED
#include "imgui.h"
#endif
//---------------------------------------

extern unsigned int GLOBAL_DRAW_CALL_COUNT;

#define INITIALIZE_MODULE(M, ...) M::CreateInst(__VA_ARGS__); LOG_INFO("Module " #M " successfully initialized");
#define DESTROY_MODULE(M) M::DestroyInst(); LOG_INFO("Module " #M " successfully destroyed");

namespace prev {

	static Spline s(Vec2(0.0f));

	Application::Application() {

		Timer::FPSCounter(false);
		INITIALIZE_MODULE(EventHandler);

		auto adapters = GraphicsContext::GetDisplayModes();

		const DisplayMode & mode = *adapters[0]->GetMonitors()[0]->GetDisplayModes()[0];

		Window::CreateInst(adapters[1]->GetMonitors()[0]);
		EventHandler::Ref().RegisterEventFunction(BIND_EVENT_FN(Application::OnEvent));

		StrongHandle<DisplayMode> disMode = Window::Ref().GetDisplayModePointer();

		INITIALIZE_MODULE(Input);
		INITIALIZE_MODULE(MVP);
		INITIALIZE_MODULE(GraphicsContext, Window::Ref().GetWindowRawPointer(), disMode, adapters[1]);
		INITIALIZE_MODULE(RenderState);
		INITIALIZE_MODULE(ShaderManager);
		INITIALIZE_MODULE(ImmediateGFX);
		INITIALIZE_MODULE(LayerStack);
		//ImGui Layer

		IMGUI_CALL(m_ImGuiLayer = new ImGuiLayer());
		IMGUI_CALL(LayerStack::Ref().PushLayer(m_ImGuiLayer));
		IMGUI_CALL(m_ImGuiLayer->AddGuiFunction(std::bind(&Application::Gui, this)));
		IMGUI_CALL(m_ImGuiLayer->SetSettingBoolean("App Settings", &m_IsGuiOpen));
		INITIALIZE_MODULE(Profiler); // Because profiler use imgui layer

		INITIALIZE_MODULE(VirtualMachine);
		VirtualMachine::Ref().RunMain();
		
		INITIALIZE_MODULE(Renderer);
		INITIALIZE_MODULE(FramebufferPass);
		INITIALIZE_MODULE(Box2DManager);
		INITIALIZE_MODULE(ImageComponent);
		INITIALIZE_MODULE(FMODAudio);
		INITIALIZE_MODULE(CameraController);
		INITIALIZE_MODULE(FramebufferStack);

		////////////////////////////////////////TESTING////////////////////////////////////////
		s.AddSegment(Vec2(0.5f, 0.0f));
		s.AddSegment(Vec2(1.5f, 0.0f));
		////////////////////////////////////////TESTING////////////////////////////////////////

	}

	Application::~Application() {
		DESTROY_MODULE(FramebufferStack);
		DESTROY_MODULE(CameraController);
		DESTROY_MODULE(FMODAudio);
		DESTROY_MODULE(ImageComponent);
		DESTROY_MODULE(Box2DManager);
		DESTROY_MODULE(FramebufferPass);
		DESTROY_MODULE(Renderer);
		DESTROY_MODULE(VirtualMachine);
		DESTROY_MODULE(Profiler);
		DESTROY_MODULE(LayerStack);
		DESTROY_MODULE(ImmediateGFX);
		DESTROY_MODULE(ShaderManager); 
		DESTROY_MODULE(RenderState);
		DESTROY_MODULE(GraphicsContext);
		DESTROY_MODULE(MVP);
		DESTROY_MODULE(Input);
		DESTROY_MODULE(Window);
		DESTROY_MODULE(EventHandler);
	}

	void Application::Render() {
		PROFILER_BEGIN("App::VirtualMachine::Render");
		VirtualMachine::Ref().Render();
		PROFILER_END("App::VirtualMachine::Render");

		PROFILER_BEGIN("App::Render");
		Renderer::Ref().Present();
		PROFILER_END("App::Render");

	}

	void Application::PreRender() {
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

		GraphicsContext::Ref().BeginFrame();

		IMGUI_CALL(if (LayerStack::Ref().GetImGuiLayer())
			LayerStack::Ref().GetImGuiLayer()->StartFrame());

		LayerStack::Ref().OnUpdate();
	}

	void Application::PostRender() {
		PROFILER_BEGIN("App::Gui");
		GuiUpdate();
		PROFILER_END("App::Gui");

		IMGUI_CALL(if (LayerStack::Ref().GetImGuiLayer())
			LayerStack::Ref().GetImGuiLayer()->EndFrame());

		GraphicsContext::Ref().EndFrame();

		Window::Ref().PollEvents();
		Input::Ref().Update();
		EventHandler::Ref().FlushEventQueue();

		CameraController::Ref().End();
	}

	void Application::Update() {
		Box2DManager::Ref().Update();
		VirtualMachine::Ref().Update();
	}

	void Application::TestRender() {
		//////////////////////////////////////TESTING////////////////////////////////////////

		FMODAudio::Ref().Update();
		CameraController::Ref().Update();

		Vec2 mousePos = CameraController::Ref().MapPixelsToScreen(Input::Ref().GetMousePosition());
		bool isHovered = false;
		static bool isDown = false;
		static unsigned int index = 0;

		static float CIRCLE_RADIUS = 0.02f;

		for (unsigned int i = 0; i < s.GetNumPoints(); i++) {
			ImmediateGFX::Ref().Color(Vec4(1.0f, 0.0f, 1.0f, 1.0f));
			if (!isDown && Length(mousePos - s[i]) < CIRCLE_RADIUS) {
				ImmediateGFX::Ref().Color(Vec4(0.0f, 1.0f, 1.0f, 1.0f));
				isHovered = true;
				index = i;
			}
			if (isDown && index == i) {
				ImmediateGFX::Ref().Color(Vec4(0.0f, 1.0f, 1.0f, 1.0f));
			}
			ImmediateGFX::Ref().DrawCircle(s[i], CIRCLE_RADIUS);
		}

		if (isHovered && Input::Ref().IsMouseButtonDown(0)) {
			isDown = true;
		}

		if (isDown && Input::Ref().IsMouseButtonReleased(0)) {
			isDown = false;
		}

		if (isDown) {
			s.MovePoint(index, mousePos);
		}

		Vec2 prevPos(0.0f);

		static unsigned int NUM_LINE_SEGMENTS = 128;

		ImmediateGFX::Ref().Color(Vec4(1.0f));
		for (unsigned int i = 0; i <= NUM_LINE_SEGMENTS; i++) {
			if (i == 0) prevPos = s.GetValue((float)i / (float)NUM_LINE_SEGMENTS);
			else {
				Vec2 newPos = s.GetValue((float)i / (float)NUM_LINE_SEGMENTS);
				ImmediateGFX::Ref().DrawLine(prevPos, newPos);
				prevPos = newPos;
			}
		}

		ImmediateGFX::Ref().Color(Vec4(1.0f, 1.0f, 0.0f, 1.0f));
		for (unsigned int i = 0; i < s.GetNumSegments(); i++) {
			auto c = s.GetControlPointsOfSegment(i);
			auto a = s.GetAnchorPointsOfSegment(i);
			ImmediateGFX::Ref().DrawLine(c[0], a[0]);
			ImmediateGFX::Ref().DrawLine(c[1], a[1]);
		}

		ImmediateGFX::Ref().Color(0.0f, 1.0f, 0.0f, 1.0f);
		ImmediateGFX::Ref().DrawCircle(Vec2(-0.5f), s.GetValue(Timer::GetTime() - (unsigned int)Timer::GetTime()).y, 128);

		//////////////////////////////////////TESTING////////////////////////////////////////
	}

	void Application::Run() {

		while (m_ApplicationRunning) {
			PROFILER_ROOT_BEGIN;

			Timer::Update();

			//Pre Render
			PreRender();

			static bool pause = false;
			if (Input::Ref().IsKeyDown(PV_KEY_P)) {
				pause = true;
			} else if (Input::Ref().IsKeyReleased(PV_KEY_P)) {
				pause = false;
			}

			//Update
			PROFILER_BEGIN("App::ALLUpdate");
			if (!pause)
			Update();
			PROFILER_END("App::ALLUpdate");
			
			//All Rendering
			PROFILER_BEGIN("App::ALLRendering");

			if (!pause) {
				FramebufferStack::Ref().Start();
				{
					//TestRender();
					Render();
				}
				FramebufferStack::Ref().End();
			}

			FramebufferStack::Ref().Present();

			PROFILER_END("App::ALLRendering");

			//Post Render
			PostRender();

			PROFILER_ROOT_END;

			//LOG_INFO("Draw Calls This Frame : {}", GLOBAL_DRAW_CALL_COUNT);
			GLOBAL_DRAW_CALL_COUNT = 0;
		}
	}

	void Application::GuiUpdate() {
		VirtualMachine::Ref().Gui();
		IMGUI_CALL(LayerStack::Ref().OnImGuiUpdate());
	}

	void Application::Gui() {
		//IMGUI_CALL(
		//	if (m_IsGuiOpen) {
		//		ImGui::Begin("Application Settings", &m_IsGuiOpen);
		//		if (ImGui::CollapsingHeader("Window Size")) {
		//			ImGui::Indent();
		//			for (const auto & disMode : m_DisplayModes) {
		//				if (ImGui::Selectable((std::to_string(disMode.GetWindowSize().x) + ", " + std::to_string(disMode.GetWindowSize().y)).c_str())) {
		//					Window::Ref().SetWindowSize(disMode.GetWindowSize());
		//				}
		//			}
		//			ImGui::Unindent();
		//		}
		//		ImGui::End();
		//	}
		//);
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