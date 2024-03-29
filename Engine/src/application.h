#pragma once

#include "utils/layerstack.h"
#include "imgui/imguilayer.h"
#include "graphics/framebuffer.h"

namespace prev {
	
	struct DisplayMode;

	class Application : public Singleton<Application> {
		friend class Singleton<Application>;
	private:
		Application();
		~Application();
	public:
		void Run();
	private:
		void GuiUpdate();
		void Gui();

		void Render();
		void PreRender();
		void PostRender();

		void TestRender();
	private:
		void OnEvent(Event & e);
		bool WindowClosed(WindowCloseEvent & e);
		bool WindowResized(WindowResizeEvent & e);
	private:
		IMGUI_CALL(ImGuiLayer * m_ImGuiLayer = nullptr);
		bool m_ApplicationRunning = true;
		bool m_IsWindowReiszed = false;
		bool m_IsGuiOpen = false;
		std::vector<DisplayMode> m_DisplayModes;
		StrongHandle<Framebuffer> m_CaptureFBO;
		StrongHandle<Framebuffer> m_CaptureFBO2;
	};

}