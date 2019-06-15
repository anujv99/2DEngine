#include "pch.h"
#include "imguilayer.h"

#include "imguimanager.h"
#include "imgui.h"

namespace prev {

	ImGuiLayer::ImGuiLayer() {
		ImGuiManager::CreateInst();
	}

	ImGuiLayer::~ImGuiLayer() {
		ImGuiManager::DestroyInst();
	}

	void ImGuiLayer::StartFrame() {
		ImGuiManager::Ref().PreUpdate();
		ImGuiManager::Ref().DetectConsumeInputs();

		OnImGuiUpdate();
	}

	void ImGuiLayer::EndFrame() {
		ImGuiManager::Ref().PostUpdate();
	}

	void ImGuiLayer::OnImGuiUpdate() {
		ImGui::Begin("Demo Window");
		ImGui::End();
	}

}