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

	StrongHandle<Framebuffer>	framebuffer;
	StrongHandle<Framebuffer>	framebuffer2;
	StrongHandle<Texture2D>		texture;
	StrongHandle<PixelShader>	pShader;
	StrongHandle<VertexShader>	vShader;

	StrongHandle<Framebuffer> curr;
	StrongHandle<Framebuffer> prev;
	StrongHandle<Framebuffer> temp;
	StrongHandle<PixelShader> waterRippleTex;
	StrongHandle<PixelShader> waterRippleVisualizer;
	StrongHandle<PixelShader> pixelSetter;

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
		framebuffer = Framebuffer::CreateFramebuffer();
		framebuffer->Init(winSize, PV_TEXTURE_FORMAT_RGBA8, true);

		framebuffer2 = Framebuffer::CreateFramebuffer();
		framebuffer2->Init(winSize, PV_TEXTURE_FORMAT_RGBA8, true);

		texture = Texture2D::CreateTexture2D();
		TextureParams params;
		params.Filtering = PV_TEXTURE_FILTER_LINEAR;
		params.Wrapping = PV_TEXTURE_WRAP_DEFAULT;

		texture->Init("res/textures/waterNormal.jpg", params);

		pShader = ShaderManager::Ref().LoadPixelShaderFromFile("TEST_PIXEL_SHADER", "res/shaders/waterRipplePixel.hlsl");
		vShader = ShaderManager::Ref().LoadVertexShaderFromFile("TEST_VERTEX_SHADER", "res/shaders/waterRippleVertex.hlsl");

		//-------------------------------------------------------------------------------------

		curr = Framebuffer::CreateFramebuffer();
		curr->Init(winSize, PV_TEXTURE_FORMAT_R32F, false);
		curr->Clear(Vec4(0.0f));

		prev = Framebuffer::CreateFramebuffer();
		prev->Init(winSize, PV_TEXTURE_FORMAT_R32F, false);
		prev->Clear(Vec4(0.0f));

		temp = Framebuffer::CreateFramebuffer();
		temp->Init(winSize, PV_TEXTURE_FORMAT_R32F, false);
		temp->Clear(Vec4(0.0f));

		waterRippleTex = ShaderManager::Ref().LoadPixelShaderFromFile("TEST_WATER_RIPPLE_PIXEL_SHADER", "res/shaders/waterRippleTexPixel.hlsl");
		waterRippleVisualizer = ShaderManager::Ref().LoadPixelShaderFromFile("TEST_WATER_RIPPLE_VIS_PIXEL_SHADER", "res/shaders/waterRippleVisualizer.hlsl");
		pixelSetter = ShaderManager::Ref().LoadPixelShaderFromFile("pixelSetter", "res/shaders/pixelSetter.hlsl");

		////////////////////////////////////////TESTING////////////////////////////////////////
	}

	Application::~Application() {

		framebuffer = nullptr;
		texture = nullptr;
		pShader = nullptr;
		vShader = nullptr;

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

			//////////////////////////////////////TESTING////////////////////////////////////////

			temp->Bind();
			temp->Clear();

			curr->GetTexture()->SetTextureSlot(1);
			curr->GetTexture()->Bind();

			prev->GetTexture()->SetTextureSlot(2);
			prev->GetTexture()->Bind();

			FramebufferPass::Ref().Pass(temp, nullptr, waterRippleTex);

			temp->UnBind();

			auto temp2 = curr;
			curr = temp;
			temp = temp2;

			framebuffer->Bind();
			framebuffer->Clear();

			static ScissorBox sb;
			sb.Left = 0.0f;
			sb.Right = 1600.0f;
			sb.Bottom = 450.0f;
			sb.Top = 900.0f;

			RenderState::Ref().SetScissorBox(sb);

			//////////////////////////////////////TESTING////////////////////////////////////////

			VirtualMachine::Ref().Update();
			VirtualMachine::Ref().Render();

			PROFILER_BEGIN("App::Present");
			Renderer::Ref().Present();
			PROFILER_END("App::Present");

			////////////////////////////////////////TESTING////////////////////////////////////////
			
			framebuffer->UnBind();
			framebuffer2->Bind();
			framebuffer2->Clear();
			
			sb.Bottom = 0.0f;
			sb.Top = 450.0f;
			
			RenderState::Ref().SetScissorBox(sb);
			
			texture->SetTextureSlot(1);
			texture->Bind();
			
			curr->GetTexture()->SetTextureSlot(2);
			curr->GetTexture()->Bind();

			{
				static float dis = 0.0f;
				static float amp = 0.0f;
				static float speed = 50.0f;
			
				ImGui::Begin("Test");
				ImGui::DragFloat("Distortion", &dis, 0.01f);
				ImGui::DragFloat("Amplitude", &amp, 0.01f);
				ImGui::DragFloat("Speed", &speed, 0.01f, 1.0f, 100.0f);
				ImGui::End();
			
				pShader->SetUniform("Time", &Vec4(Timer::GetTime() / speed, dis, amp, dis)[0], sizeof(Vec4));
			}
			
			FramebufferPass::Ref().Pass(framebuffer, vShader, pShader);
			
			framebuffer2->UnBind();
			
			RenderState::Ref().DisableScissors();
			
			FramebufferPass::Ref().Pass(framebuffer);
			FramebufferPass::Ref().Pass(framebuffer2);

			Renderer::Ref().Present();
			//-------------------------------------------------------------------------------------

			if (Input::Ref().GetMouseDeltaPosition() != Vec2(0.0f, 0.0f)) {
				curr->Bind();
				ImmediateGFX::Ref().Color(Vec4(2.0f));
				ImmediateGFX::Ref().DrawCircle(Input::Ref().GetMousePosition(), 0.01f);
				curr->UnBind();
			}

			//FramebufferPass::Ref().Pass(curr, nullptr, waterRippleVisualizer);

			temp2 = curr;
			curr = prev;
			prev = temp2;

			////////////////////////////////////////TESTING////////////////////////////////////////

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