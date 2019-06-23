#include "pch.h"
#include "imguilayer.h" 

#include "imgui.h"

namespace prev {

	ImGuiLayer::ImGuiLayer() : Layer(IMGUI_LAYER_NAME) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO & io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		InitImGui();

		LOG_TRACE("ImGui Initialized");
	}

	ImGuiLayer::~ImGuiLayer() {

	}

	void ImGuiLayer::StartFrame() {
		Start();
		ImGui::NewFrame();
	}

	void ImGuiLayer::EndFrame() {
		ImGui::Render();
		End();
	}

	void ImGuiLayer::AddGuiFunction(GuiFunc func) {
		if (func)
			m_GuiFunctions.push_back(func);
		else {
			LOG_ERROR("Inavlid gui function");
		}
	}

	void ImGuiLayer::OnImGuiUpdate() {
		ImGui::ShowDemoWindow();
		for (auto & func : m_GuiFunctions)
			func();
	}

}