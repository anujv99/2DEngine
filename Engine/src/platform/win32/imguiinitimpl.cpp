#include "pch.h"

#ifdef IMGUI_ENABLED

#include "d3dhelper.h"

#include "imgui/imguilayer.h"

#include "imgui.h"

#include "systemres/resource.h"

#if 0

#undef IDC_ARROW
#define IDC_ARROW MAKEINTRESOURCEA(IDC_CARROW)

#undef IDC_IBEAM
#define IDC_IBEAM MAKEINTRESOURCEA(IDC_CTEXTINPUT)

#undef IDC_SIZEALL
#define IDC_SIZEALL MAKEINTRESOURCEA(IDC_CSIZEALL)

#undef IDC_SIZEWE
#define IDC_SIZEWE MAKEINTRESOURCEA(IDC_CSIZEWE)

#undef IDC_SIZENS
#define IDC_SIZENS MAKEINTRESOURCEA(IDC_CSIZENS)

#undef IDC_SIZENESW
#define IDC_SIZENESW MAKEINTRESOURCEA(IDC_CSIZENESW)

#undef IDC_SIZENWSE
#define IDC_SIZENWSE MAKEINTRESOURCEA(IDC_CSIZENWSE)

#undef LoadCursor
#define LoadCursor(h, c) LoadCursorA(GetModuleHandleA(NULL), c)

#endif

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

#endif