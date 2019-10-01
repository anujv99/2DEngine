#pragma once

#include "layer.h"

#include "imgui/imguilayer.h"

namespace prev {

	class LayerStack : public Singleton<LayerStack> {
		friend class Singleton<LayerStack>;
	private:
		LayerStack();
		~LayerStack();
	public:
		void PushLayer(Layer * layer);
		void PushOverlay(Layer * layer);
		void PopLayer(Layer * layer);
		void PopOverlay(Layer * layer);

		IMGUI_CALL(inline ImGuiLayer * GetImGuiLayer() { return m_ImGuiLayer; })
	public:
		void OnUpdate();
		IMGUI_CALL(void OnImGuiUpdate());
		void OnEvent(Event & e);
	private:
		std::vector<StrongHandle<Layer>> m_Layers;
		std::vector<StrongHandle<Layer>> m_Overlays;

		IMGUI_CALL(ImGuiLayer * m_ImGuiLayer;)
	};

}