#include "pch.h"
#include "application.h"

#include "utils/input.h"

#include "graphics/graphicscontext.h"
#include "graphics/computebuffer.h"
#include "graphics/shadermanager.h"
#include "graphics/tiledtexture.h"
#include "graphics/immediategfx.h"
#include "graphics/renderstate.h"
#include "graphics/texture2d.h"
#include "graphics/window.h"
#include "graphics/font.h"

#include "math/screenspace.h"
#include "math/mat4.h"
#include "math/mvp.h"

#include "common/profiler.h"

#include "vm/virtualmachine.h"

#include "renderer/renderer.h"
#include "renderer/fbopass.h"

#include "physics/box2ddebugdraw.h"
#include "physics/box2dmanager.h"

#include "game/square.h"
#include "imgui.h"

//-----------------TEMP------------------
#include "platform/win32/d3dhelper.h"
#include "utils/datafile.h"
//---------------------------------------

extern unsigned int GLOBAL_DRAW_CALL_COUNT;

namespace prev {

	StrongHandle<ComputeBuffer> cb1 = nullptr;
	StrongHandle<ComputeBuffer> cb2 = nullptr;
	StrongHandle<ComputeShader> cs = nullptr;

	static const constexpr unsigned int TILES_X = 64u;
	static const constexpr unsigned int TILES_Y = 64u;

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

		Box2DManager::CreateInst();

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

		float data[TILES_X * TILES_Y];

		for (unsigned int i = 0; i < TILES_X * TILES_Y; i++) {
			data[i] = 0;
		}

		cb1 = ComputeBuffer::CreateComputeBuffer();
		cb1->Init(data, TILES_X * TILES_Y, sizeof(data[0]));
		cb1->SetBindSlot(0);

		cb2 = ComputeBuffer::CreateComputeBuffer();
		cb2->Init(data, TILES_X * TILES_Y, sizeof(data[0]));
		cb2->SetBindSlot(1);

		cs = ShaderManager::Ref().LoadComputeShaderFromFile("TEST_SHADER", "res/computeshader/test.hlsl");

		struct data {
			unsigned int size[2];
			float damping[2];
		} d;

		d = { TILES_X, TILES_Y, 0.95f, 0 };

		cs->SetUniform("Size", &d, sizeof(d));

		////////////////////////////////////////TESTING////////////////////////////////////////
	}

	Application::~Application() {
		cb1 = nullptr;
		cb2 = nullptr;
		cs = nullptr;

		m_DefCamera.End();
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

			Timer::Update();

			GraphicsContext::Ref().BeginFrame();
			
			if (LayerStack::Ref().GetImGuiLayer())
				LayerStack::Ref().GetImGuiLayer()->StartFrame();

			LayerStack::Ref().OnUpdate();

			Box2DManager::Ref().Update();

			VirtualMachine::Ref().Update();
			VirtualMachine::Ref().Render();

			//////////////////////////////////////TESTING////////////////////////////////////////
			{
				TIME_THIS_SCOPE_MS;

				cb1->Bind();
				cb2->Bind();

				cs->Bind();

				cs->Dispatch(TILES_X, TILES_Y, 1);

				float * data = (float *)cb2->Map();

				if (Input::Ref().IsMouseButtonDown(0))
					data[TILES_X / 2 + TILES_Y / 2 * TILES_Y] = 1.0f;

				for (unsigned int i = 0; i < TILES_X; i++) {
					for (unsigned int j = 0; j < TILES_Y; j++) {

						static float scale = 2.0f;

						static Sprite s;
						s.Position = Vec2((float)i / ((float)TILES_X / scale), (float)j / ((float)TILES_Y / scale));
						s.Dimension = Vec2(scale / TILES_X);
						s.Color = Vec4(data[i + j * TILES_Y]);

						Renderer::Ref().Submit(s);
					}
				}

				cb2->UnMap();

				StrongHandle<ComputeBuffer> temp = cb1;
				cb1 = cb2;
				cb2 = temp;

				cb1->SetBindSlot(0);
				cb2->SetBindSlot(1);
			}

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