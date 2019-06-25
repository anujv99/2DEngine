#include "pch.h"

#include "d3dhelper.h"

#include "imgui/imguilayer.h"

#include "imgui.h"

#include "examples/imgui_impl_win32.h"
#include "examples/imgui_impl_win32.cpp"

#include "examples/imgui_impl_dx11.h"
#include "examples/imgui_impl_dx11.cpp"

namespace prev {

	void ImGuiLayer::InitImGui() {
		ImGui_ImplWin32_Init((void *)Window::Ref().GetWindowRawPointer());
		ImGui_ImplDX11_Init(GetDevice(), GetDeviceContext());
	}

	void ImGuiLayer::DeleteImGui() {
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
	}

	void ImGuiLayer::Start() {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
	}

	void ImGuiLayer::End() {
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

}