#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include "graphics/window.h"

namespace prev {

	class WindowsWindow : public Window {
	public:
		WindowsWindow(const DisplayMode & displayMode);
		~WindowsWindow();

		// Inherited via Window
		virtual bool GetKeyboardKeyState(int keyCode) override;
		virtual bool GetMouseButtonState(int buttonCode) override;
		Vec2i GetMousePosition() override;
		virtual void PollEvents() override;
	private:
		bool RegisterWindowClass(const DisplayMode & displayMode);
		bool CreateWindowsWindow(const DisplayMode & displayMode);
	private:
		HWND m_HWnd					= nullptr;
		HINSTANCE m_HInst			= nullptr;
		UINT m_WindowClassStyle		= 0u;
		DWORD m_WindowStyle			= 0u;
		DWORD m_WindowExStyle		= 0u;
		MSG m_Message				= {};

		bool m_IsWindowReady = true;
		Vec2i m_DisplaySize = Vec2i(0u, 0u); //Monitor native resolution
		Vec2i m_DisplayPos  = Vec2i(0u, 0u);
	};

}