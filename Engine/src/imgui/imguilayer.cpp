#include "pch.h"
#include "imguilayer.h"

#include "imguimanager.h"
#include "imgui.h"

namespace prev {

	ImGuiLayer::ImGuiLayer() : Layer(IMGUI_LAYER_NAME) {
		ImGuiManager::CreateInst();
		BindGuiFunction(std::bind(&ImGuiLayer::ImGuiDemoWindow, this));
	}

	ImGuiLayer::~ImGuiLayer() {
		ImGuiManager::DestroyInst();
	}

	void ImGuiLayer::BindGuiFunction(std::function<std::string(void)> guiFunc) {
		if (!guiFunc) {
			LOG_ERROR("Inavid Gui Function Passed");
			return;
		}

		auto windowName = guiFunc();
		m_GUIFuncs.insert(std::make_pair(windowName, guiFunc));
	}

	void ImGuiLayer::OnUpdate() {
		
	}

	std::string ImGuiLayer::OnImGuiUpdate() {
		StartFrame();

		std::vector<std::function<std::string(void)>> guiFuncs;
		guiFuncs.resize(ImGuiManager::Ref().GetWindowList().size());

		for (auto & guiFunc : m_GUIFuncs) {
			auto index = ImGuiManager::Ref().GetWindowOrderIndex(guiFunc.first);
			if (index >= 0 && index < guiFuncs.size()) {
				guiFuncs[guiFuncs.size() - index - 1] = guiFunc.second;
			} else {
				LOG_ERROR("Invalid ImGui window name : {}", guiFunc.first);
			}
		}

		for (auto & func : guiFuncs) {
			if (func)
				func();
		}

		EndFrame();

		return "";
	}

	void ImGuiLayer::StartFrame() {
		ImGuiManager::Ref().PreUpdate();
	}

	void ImGuiLayer::EndFrame() {
		ImGuiManager::Ref().PostUpdate();
		ImGuiManager::Ref().DetectConsumeInputs();
	}

	std::string ImGuiLayer::ImGuiDemoWindow() {
		ImGui::Begin("ImGui Demo Window");
		ImGui::Print("This is the demo window");
		ImGui::End();

		return "ImGui Demo Window";
	}

}