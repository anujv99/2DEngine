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
//---------------------------------------

extern unsigned int GLOBAL_DRAW_CALL_COUNT;

namespace prev {

	static constexpr const unsigned int TILE_X = 1600;
	static constexpr const unsigned int TILE_Y = 900;

	static constexpr const unsigned int THREAD_X = TILE_X / 100;
	static constexpr const unsigned int THREAD_Y = TILE_Y / 10;

	StrongHandle<ComputeShader> cs = nullptr;
	StrongHandle<ComputeBuffer> b1 = nullptr;
	StrongHandle<ComputeBuffer> b2 = nullptr;
	StrongHandle<Framebuffer>	fm = nullptr;
	StrongHandle<PixelShader>	ps = nullptr;
	StrongHandle<VertexShader>	vs = nullptr;

	struct {
		unsigned int Size[2];
		float Damping;
		float Padding;
	} d;

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

		////////////////////////////////////////TESTING////////////////////////////////////// //

		cs = ShaderManager::Ref().LoadComputeShaderFromFile("TEST_COMPUTE", "res/shaders/testCompute.hlsl");

		b1 = ComputeBuffer::CreateComputeBuffer();
		b1->Init(nullptr, TILE_X * TILE_Y, sizeof(float));
		b1->SetBindSlot(1);

		b2 = ComputeBuffer::CreateComputeBuffer();
		b2->Init(nullptr, TILE_X * TILE_Y, sizeof(float));
		b2->SetBindSlot(2);

		d.Damping = 0.98f;
		d.Size[0] = TILE_X;
		d.Size[1] = TILE_Y;
		
		cs->SetUniform("Data", &d, sizeof(d));

		fm = Framebuffer::CreateFramebuffer();
		fm->Init(winSize, PV_TEXTURE_FORMAT_RGBA8, FRAMEBUFFER_NO_FLAGS);

		ps = ShaderManager::Ref().LoadPixelShaderFromFile("TEST_PIXEL", "res/shaders/testFboPixel.hlsl");
		vs = ShaderManager::Ref().LoadVertexShaderFromFile("TEST_VERTEX", "res/shaders/testFboVertex.hlsl");

		unsigned int size[4] = { TILE_X, TILE_Y, 0u, 0u };
		vs->SetUniform("VData", size, sizeof(size));
		ps->SetUniform("PData", size, sizeof(size));

		////////////////////////////////////////TESTING////////////////////////////////////////
	}

	Application::~Application() {
		cs = nullptr;
		b1 = nullptr;
		b2 = nullptr;
		fm = nullptr;
		ps = nullptr;
		vs = nullptr;

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

			fm->Bind();		//TEMP
			fm->Clear();	//TEMP

			VirtualMachine::Ref().Update();
			VirtualMachine::Ref().Render();

			//////////////////////////////////////TESTING////////////////////////////////////////

			{
				b1->Bind();
				b2->Bind();

				cs->Bind();

				cs->Dispatch(THREAD_X, THREAD_Y, 1);

				b2->UnBind();
				b1->UnBind();

				StrongHandle<ComputeBuffer> temp = b1;
				b1 = b2;
				b2 = temp;

				b1->SetBindSlot(1);
				b2->SetBindSlot(2);

				//------------------------------------------------------------------------------

				if (Input::Ref().IsMouseButtonDown(0)) {

					Vec2 pos = Input::Ref().GetMousePosition();
					pos = ScreenToPixels(pos);
					pos += ToVec2(Window::Ref().GetDisplayMode().GetWindowSize() / 2);
					pos = Vec2(pos.x, Window::Ref().GetDisplayMode().GetWindowSize().y - pos.y);
					pos = Clamp(pos, Vec2(0.0f), ToVec2(Window::Ref().GetDisplayMode().GetWindowSize()));
					pos /= ToVec2(Window::Ref().GetDisplayMode().GetWindowSize());
					pos *= Vec2((float)TILE_X, (float)TILE_Y);

					float * d = (float *)b2->Map();
					d[((int)pos.x) + ((int)pos.y) * TILE_X] = 1.0f;
					b2->UnMap();
				}
				
				//------------------------------------------------------------------------------

				Renderer::Ref().Present();

				fm->UnBind();

				b2->BindToPixelShader(10);

				FramebufferPass::Ref().Pass(fm, vs, ps);

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