#pragma once

#include "layer.h"

namespace prev {

	class LayerStack {
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(StrongHandle<Layer> layer);
		void PushOverlay(StrongHandle<Layer> layer);
		void PopLayer(StrongHandle<Layer> layer);
		void PopOverlay(StrongHandle<Layer> layer);

	public:
		void OnUpdate();
		void OnEvent(Event & e);
		void OnImGuiUpdate();
	private:
		std::vector<StrongHandle<Layer>> m_Layers;
		std::vector<StrongHandle<Layer>> m_Overlays;
	};

}