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

#include "physics/box2dmanager.h"
#include "physics/box2ddebugdraw.h"

#include "graphics/texture2d.h"
#include "graphics/font.h"
#include "imgui.h"

extern unsigned int GLOBAL_DRAW_CALL_COUNT;

namespace prev {

	StrongHandle<Framebuffer> fbo1;

	StrongHandle<Framebuffer> blurA;
	StrongHandle<Framebuffer> blurB;

	StrongHandle<PixelShader> bloomExtract;
	StrongHandle<PixelShader> bloomFinal;
	StrongHandle<PixelShader> blur;

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
<<<<<<< HEAD
		
=======
		fbo1 = Framebuffer::CreateFramebuffer();
		fbo1->Init(winSize, PV_TEXTURE_FORMAT_RGBA8, FRAMEBUFFER_ENABLE_MSAA | FRAMEBUFFER_ALLOCATE_DEPTH_BUFFER);

		bloomExtract = ShaderManager::Ref().LoadPixelShaderFromFile("BLOOM_EXTRACT", "res/shaders/bloomExtract.hlsl");

		blurA = Framebuffer::CreateFramebuffer();
		blurB = Framebuffer::CreateFramebuffer();

		blurA->Init(winSize, PV_TEXTURE_FORMAT_RGBA8, FRAMEBUFFER_ENABLE_MSAA);
		blurB->Init(winSize, PV_TEXTURE_FORMAT_RGBA8, FRAMEBUFFER_ENABLE_MSAA);

		blur = ShaderManager::Ref().LoadPixelShaderFromFile("BLUR", "res/shaders/blur.hlsl");
		bloomFinal = ShaderManager::Ref().LoadPixelShaderFromFile("bloomFinal", "res/shaders/bloomFinal.hlsl");

>>>>>>> parent of 73d38dd... addid chipmunk instead of box2d
		////////////////////////////////////////TESTING////////////////////////////////////////
	}

	Application::~Application() {

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
			fbo1->Bind();
			fbo1->Clear();

			static Sprite s;
			s.Depth = 0.0f;
			s.Position += Input::Ref().GetMouseDeltaRaw();
			Renderer::Ref().Submit(s);

			LOG_INFO("{}", s.Position);

			//////////////////////////////////////TESTING////////////////////////////////////////

			PROFILER_BEGIN("App::Present");
			Renderer::Ref().Present();
			PROFILER_END("App::Present");

			//////////////////////////////////////TESTING////////////////////////////////////////

			fbo1->UnBind();

			blurA->Bind();
			blurA->Clear();

			FramebufferPass::Ref().Pass(fbo1, nullptr, bloomExtract);

			blurA->UnBind();

			auto thisFBO = blurB;

			struct cbuffer {
				int a;
				float b[3];
			};

			static cbuffer buff;

			for (int i = 0; i < 10; i++) {
				if (thisFBO == blurB) {
					blurB->Bind();
					blurB->Clear();

					buff.a = 0;
					blur->SetUniform("Misc", &buff, sizeof(buff));

					FramebufferPass::Ref().Pass(blurA, nullptr, blur);
					blurB->UnBind();
					thisFBO = blurA;
				} else {
					blurA->Bind();
					blurA->Clear();

					buff.a = 1;
					blur->SetUniform("Misc", &buff, sizeof(buff));

					FramebufferPass::Ref().Pass(blurB, nullptr, blur);
					blurA->UnBind();
					thisFBO = blurB;
				}
			}

			if (thisFBO == blurB)
				thisFBO = blurA;
			else
				thisFBO = blurB;

			thisFBO->GetTexture()->SetTextureSlot(1);
			thisFBO->GetTexture()->Bind();

			FramebufferPass::Ref().Pass(fbo1, nullptr, bloomFinal);

			//////////////////////////////////////TESTING////////////////////////////////////////

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