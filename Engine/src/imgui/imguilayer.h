#pragma once

#include "utils/layer.h"

namespace prev {

	static const std::string IMGUI_LAYER_NAME = "IMGUI_LAYER";

	class ImGuiLayer : public Layer, public HandledObject<ImGuiLayer> {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void BindGuiFunction(std::function<std::string(void)> guiFunc);

		virtual void OnUpdate() override;
		virtual std::string OnImGuiUpdate() override;
	private:
		void StartFrame();
		void EndFrame();

		std::string ImGuiDemoWindow();
	private:
		std::unordered_map<std::string, std::function<std::string(void)>> m_GUIFuncs;
	};

}
