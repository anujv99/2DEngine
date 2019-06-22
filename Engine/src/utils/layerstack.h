#pragma once

#include "layer.h"

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

		Layer * GetImGuiLayer() { return m_ImGuiLayer; }
	public:
		void OnUpdate();
		void OnEvent(Event & e);
		void OnImGuiUpdate();
	private:
		std::vector<StrongHandle<Layer>> m_Layers;
		std::vector<StrongHandle<Layer>> m_Overlays;

		Layer * m_ImGuiLayer = nullptr;
	};

}