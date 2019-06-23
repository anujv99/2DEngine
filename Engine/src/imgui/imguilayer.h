#pragma once

#include "utils/layer.h"

#define IMGUI_LAYER_NAME "IMGUI_PREV_LAYER"

namespace prev {

	typedef std::function<void(void)> GuiFunc;

	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void StartFrame();
		void EndFrame();

		void AddGuiFunction(GuiFunc func);

		virtual void OnImGuiUpdate() override;
	private:
		std::vector<GuiFunc> m_GuiFunctions;
	private:
		static void InitImGui();
		static void Start();
		static void End();
	};

}