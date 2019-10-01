#pragma once

#ifdef IMGUI_ENABLED
	#define IMGUI_CALL(F) F
#else
	#define IMGUI_CALL(F)
#endif

#ifdef IMGUI_ENABLED

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

		inline void SetSettingBoolean(const std::string & name, bool * control) { if (!control) return; m_Settings.push_back(std::make_pair(name, control)); };

		virtual void OnImGuiUpdate() override;
	private:
		void SetupImGuiStyle(bool bStyleDark_ = true, float alpha_ = 1.0f);
	private:
		std::vector<GuiFunc> m_GuiFunctions;
		std::vector<std::pair<std::string, bool *>> m_Settings;
	private:
		static void InitImGui();
		static void DeleteImGui();
		static void Start();
		static void End();
	public:
		static bool IsImGuiInitialized();
	};

}

#endif