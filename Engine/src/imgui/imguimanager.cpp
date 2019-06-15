#include "pch.h"
#include "imguimanager.h"

#include "graphics/window.h"
#include "imgui.h"
#include "utils/input.h"

namespace prev {

	const int DEAD_WINDOW_NUM_FRAMES = 100000;

	ImGuiManager::ImGuiManager() {
		
		m_VertexShader = ShaderManager::Ref().LoadVertexShaderFromFile("FONT_DEFAULT_SHADER", "../Engine/res/shaders/fontDefaultVertex.hlsl");
		m_PixelShader = ShaderManager::Ref().LoadPixelShaderFromFile("FONT_DEFAULT_SHADER", "../Engine/res/shaders/fontDefaultPixel.hlsl");

		m_Font.Init("../Engine/res/font/coolville.png", 2.8f, m_VertexShader);

		ImGui::FONT_WIDTH = m_Font.GetCharacterWidth('M');
		ImGui::FONT_HEIGHT = m_Font.GetCharacterHeight('M');

		state.Reset();

		const Vec2i windowSize = Window::Ref().GetDisplayMode().GetWindowSize();
		state.MouseRegionStart = Vec2i(0, windowSize.y);
		state.MouseRegionDimensions = windowSize;

		m_Camera.SetScreenSpace(Vec2(0, 0), ToVec2(windowSize));
	}

	ImGuiManager::~ImGuiManager() {}

	void ImGuiManager::PreUpdate() {
		PruneVisibleWindow();
		UpdateWindowMap();
	}

	void ImGuiManager::PostUpdate() {
		m_ConsumeKeyPress.clear();
		m_ConsumeMouseButtons.clear();
	}

	void ImGuiManager::ClearActiveWindow() {
		state.ActiveWindow = nullptr;
		state.WidgetCount = 0;
		state.DoesWidgetConsumeTextInput = false;
	}

	void ImGuiManager::ClearAllWindow() {
		ClearActiveWindow();
		State().Reset();
		m_WindowMap.clear();
		m_VisibleWindows.clear();
	}

	void ImGuiManager::OnResizeScreen(Vec2i prevDimen, Vec2i newDimen) {
		throw std::logic_error("implement this method");
	}

	void ImGuiManager::SetActiveWindow(StrongHandle<ImGuiWindow> window) {
		if (window == nullptr) {
			ClearActiveWindow();
		}

		if (state.ActiveWindow != window) {
			SetActiveWidgetId(State::WIDGET_NULL);
		}

		if (window != nullptr && window != state.ActiveWindow) {
			BringWindowToFront(window);
		}

		state.ActiveWindow = window;
	}

	void ImGuiManager::SetActiveWidgetId(int id) {
		state.ActiveWidgetID = id;
		state.DoesWidgetConsumeTextInput = false;
	}

	int ImGuiManager::GetWindowOrderIndex(const std::string & name) const {
		uint32_t hash = HashString(name);
		const auto it = m_WindowMap.find(hash);
		if (it == m_WindowMap.end()) return -1;

		for (size_t i = 0; i < m_VisibleWindows.size(); i++) {
			const auto window = m_VisibleWindows[i];

			if (window == it->second) {
				return (int)i;
			}
		}

		return -1;
	}

	void ImGuiManager::BringWindowToFront(StrongHandle<ImGuiWindow> & window) {
		for (size_t i = 0; i < m_VisibleWindows.size(); i++) {
			auto currentWindow = m_VisibleWindows[i];

			if (window == currentWindow) {
				for (size_t j = i; j >= 1; j--) {
					m_VisibleWindows[j] = m_VisibleWindows[j - 1];
				}
			}
		}
	}

	bool ImGuiManager::IsPosInWindow(Vec2i pos, StrongHandle<ImGuiWindow> window) const {
		if (window == nullptr) return false;
		Vec2i windowPos = window->Position;
		Vec2i windowDimen = window->Dimension;

		if (window->Minimized || window->Locked) {
			windowDimen.y = ImGui::TITLE_BAR_HEIGHT;
		}

		const bool posInWindow = pos.x >= windowPos.x && pos.x <= windowPos.x + windowDimen.x
			&& pos.y <= windowPos.y && pos.y >= windowPos.y - windowDimen.y;

		return posInWindow;
	}

	void ImGuiManager::DetectConsumeInputs() {
		const int minMouseButton = 0;
		const int maxMouseButton = 2;
		const Vec2i mouse_pos = Input::Get()->GetMousePosition();

		bool mouseClicked = false;
		bool mouseOverAnyWindow = false;

		for (auto window : m_VisibleWindows) {
			// is mouse in 
			const bool mouseInWindow = IsPosInWindow(mouse_pos, window);
			mouseOverAnyWindow |= mouseInWindow;

			// check all mouse button presses if is in
			for (int i = minMouseButton; i <= maxMouseButton; i += 1) {
				if (Input::Ref().IsMouseButtonPressed(i)) {
					mouseClicked = true;

					// if did not click on active window
					auto active_window = state.ActiveWindow;
					if (active_window == nullptr || (mouseInWindow && window != active_window && !IsPosInWindow(mouse_pos, active_window))) {
						SetActiveWindow(window);
					}

					if (mouseInWindow) {
						m_ConsumeMouseButtons.insert(i);
					}
				}
			}
		}

		// if clicked but missed all windows, clear the active window
		if (mouseClicked && !mouseOverAnyWindow) {
			ClearActiveWindow();
		}

		// if the widget consumes input, check to see if user is typing in widget
		if (state.DoesWidgetConsumeTextInput) {
			LOG_ERROR("Not imlemented");
		}
	}

	prev::StrongHandle<prev::ImGuiWindow> ImGuiManager::GetWindow(const std::string & name) {
		uint32_t hash = HashString(name);
		auto it = m_WindowMap.find(hash);

		if (it == m_WindowMap.end()) {
			StrongHandle<ImGuiWindow> result = new ImGuiWindow();
			result->Name = name;
			m_WindowMap[hash] = result;
			return result;
		}

		return it->second;
	}

	void ImGuiManager::PruneVisibleWindow() {
		for (size_t i = 0; i < m_VisibleWindows.size(); i++) {
			auto window = m_VisibleWindows[i];

			if (window->FramesSinceUpdate > 0) {
				m_VisibleWindows[i] = m_VisibleWindows.back();
				m_VisibleWindows.resize(m_VisibleWindows.size() - 1);
			}
		}
	}

	void ImGuiManager::UpdateWindowMap() {
		auto it = m_WindowMap.begin();

		while (it != m_WindowMap.end()) {
			auto window = it->second;

			if (window->FramesSinceUpdate > 1) {
				if (state.ActiveWindow == window) {
					ClearActiveWindow();
				}

				if (window->FramesSinceUpdate > DEAD_WINDOW_NUM_FRAMES) {
					m_WindowMap.erase(it++);
					continue;
				}

			} else {
				bool isInVisibleList = false;

				for (auto visibleIt : m_VisibleWindows) {
					if (visibleIt == window) {
						isInVisibleList = true;
						break;
					}
				}

				if (!isInVisibleList) {
					m_VisibleWindows.push_back(window);
				}

			}

			window->FramesSinceUpdate++;
			it++;
		}
	}

	void ImGuiManager::DebugPrintVisibleWindows() {
		static int frame = 0;
		LOG_TRACE("------[{}]------", frame++);

		for (size_t i = 0; i < m_VisibleWindows.size(); i++) {
			auto window = m_VisibleWindows[i];
			LOG_TRACE("[{}] {} - FramesSinceUpdate : {}", i, window->Name, window->FramesSinceUpdate);
		}
	}

	void ImGuiManager::State::Reset() {
		ActiveWindow = nullptr;
		WorkingWindow = nullptr;

		Data.FVec[0] = 0.0f;
		Data.FVec[1] = 0.0f;
		Data.FVec[2] = 0.0f;
		Data.FVec[3] = 0.0f;

		ActiveWidgetID = WIDGET_NULL;
		WidgetCount = 0;
		DoesWidgetConsumeTextInput = false;
	}

}