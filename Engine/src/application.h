#pragma once

#include "utils/layerstack.h"
#include "imgui/imguilayer.h"

#include "math/cam2d.h"

namespace prev {

	class Application : public Singleton<Application> {
		friend class Singleton<Application>;
	private:
		Application();
		~Application();
	public:
		void Run();

		const Cam2D & GetDefaultCamera() const { return m_DefCamera; }
	private:
		void Gui();
	private:
		void OnEvent(Event & e);
		bool WindowClosed(WindowCloseEvent & e);
		bool WindowResized(WindowResizeEvent & e);
	private:
		ImGuiLayer * m_ImGuiLayer = nullptr;
		bool m_ApplicationRunning = true;

		//Default Camera
		Cam2D m_DefCamera;

	};

}