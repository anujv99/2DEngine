#include "pch.h"
#include "layerstack.h"

#include "imgui/imguilayer.h"

namespace prev {

	LayerStack::LayerStack() {}

	LayerStack::~LayerStack() {
		m_Layers.clear();
		m_Overlays.clear();
	}

	void LayerStack::PushLayer(Layer * layer) {
		m_Layers.push_back(layer);
		layer->OnAttach();

		if (m_ImGuiLayer == nullptr && layer->GetName() == IMGUI_LAYER_NAME) {
			m_ImGuiLayer = layer;
		} else if (m_ImGuiLayer != nullptr) {
			ImGuiLayer * guiLayer = dynamic_cast<ImGuiLayer *>(m_ImGuiLayer);
			guiLayer->BindGuiFunction(std::bind(&Layer::OnImGuiUpdate, layer));
		}
	}

	void LayerStack::PushOverlay(Layer * layer) {
		m_Overlays.push_back(layer);
		layer->OnAttach();
	}

	void LayerStack::PopLayer(Layer * layer) {
		for (int i = 0; i < m_Layers.size(); i++) {
			if (m_Layers[i].Get() == layer) {
				m_Layers.erase(m_Layers.begin() + i);
				layer->OnDetach();
			}
		}
	}

	void LayerStack::PopOverlay(Layer * layer) {
		for (int i = 0; i < m_Overlays.size(); i++) {
			if (m_Overlays[i].Get() == layer) {
				m_Overlays.erase(m_Overlays.begin() + i);
				layer->OnDetach();
			}
		}
	}

	void LayerStack::OnUpdate() {
		for (auto layer : m_Layers) {
			layer->OnUpdate();
		}

		for (auto layer : m_Overlays) {
			layer->OnUpdate();
		}
	}

	void LayerStack::OnEvent(Event & e) {
		for (auto layer : m_Layers) {
			layer->OnEvent(e);
			if (e.Handled())
				break;
		}

		for (auto layer : m_Overlays) {
			layer->OnEvent(e);
			if (e.Handled())
				break;
		}
	}

	void LayerStack::OnImGuiUpdate() {
		if (m_ImGuiLayer)
			m_ImGuiLayer->OnImGuiUpdate();
	}

}