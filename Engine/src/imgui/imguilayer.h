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
		void SetupImGuiStyle(bool bStyleDark_ = true, float alpha_ = 1.0f);
	private:
		std::vector<GuiFunc> m_GuiFunctions;
	private:
		static void InitImGui();
		static void DeleteImGui();
		static void Start();
		static void End();
	public:
		static bool IsImGuiInitialized();
	};

}