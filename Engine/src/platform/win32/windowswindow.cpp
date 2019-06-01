#include "pch.h"
#include "windowswindow.h"

#include <comdef.h>

#define DEFAULT_WINDOW_TITLE "Retarded Window"
#define WINDOW_CLASS_NAME "Slightly_Retarded_Fart_Box"

namespace prev {

	Window * Window::CreateEngineWindow(const DisplayMode & displayMode) {
		WindowsWindow * window = new WindowsWindow(displayMode);
		return (Window *)window;
	}

	WindowsWindow::WindowsWindow(const DisplayMode & displayMode) : Window(displayMode) {
		m_HInst = GetModuleHandle(nullptr);
		m_DisplaySize = Vec2i(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

		m_DisplayPos.x = (m_DisplaySize.x - displayMode.GetWindowSize().x) / 2;
		m_DisplayPos.y = (m_DisplaySize.y - displayMode.GetWindowSize().y) / 2;

		switch (displayMode.GetWindowStyle()) {
			case WindowStyle::WINDOWED:
				m_WindowClassStyle	= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
				m_WindowExStyle		= WS_EX_APPWINDOW;
				m_WindowStyle		= WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
				break;
			case WindowStyle::FULLSCREEN:
				m_DisplayPos = Vec2i(0u, 0u);
			case WindowStyle::BORDERLESS:
				m_WindowClassStyle	= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
				m_WindowExStyle		= WS_EX_APPWINDOW;
				m_WindowStyle		= WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
				break;
			default:
				ERROR_TRACE(ERR_WINDOW_CREATION_FAILED, "Invalid window style");
				m_IsWindowReady = false;
				return;
				break;
		}

		bool result = RegisterWindowClass(displayMode);
		if (!result) {
			m_IsWindowReady = false;
			return;
		}

		result = CreateWindowsWindow(displayMode);
		if (!result) {
			m_IsWindowReady = false;
			return;
		}

		return;
	}

	WindowsWindow::~WindowsWindow() {

	}

	bool WindowsWindow::GetKeyboardKeyState(int keyCode) {
		return false;
	}

	bool WindowsWindow::GetMouseButtonState(int buttonCode) {
		return false;
	}

	Vec2i WindowsWindow::GetMousePosition() {
		RECT r;
		auto g = GetClientRect(m_HWnd, &r);
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(m_HWnd, &pt);
		return Vec2i(pt.x, pt.y);
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (msg) {
			case WM_SIZE:
			{
				POINTS pt = MAKEPOINTS(lParam);
				int a = 0;
				break;
			}
			default:
				return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		return 0;
	}

	bool WindowsWindow::RegisterWindowClass(const DisplayMode & displayMode) {
		WNDCLASSEXA wc;
		ZeroMemory(&wc, sizeof(wc));

		wc.cbSize				= sizeof(wc);
		wc.style				= m_WindowClassStyle;
		wc.lpfnWndProc			= WndProc;
		wc.cbClsExtra			= 0;
		wc.cbWndExtra			= 0;
		wc.hInstance			= m_HInst;
		wc.hIcon				= LoadIcon(NULL, IDI_WINLOGO);
		wc.hCursor				= LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground		= (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName			= NULL;
		wc.lpszClassName		= WINDOW_CLASS_NAME;
		wc.hIconSm				= LoadIcon(NULL, IDI_WINLOGO);

		ATOM result = RegisterClassExA(&wc);

		if (result == 0) {
			auto errorCode = GetLastError();
			_com_error error(errorCode);
			std::string serror = _bstr_t(error.ErrorMessage());
			ERROR_TRACE(ERR_WINDOW_CREATION_FAILED, "Unable to register window class, " + serror);
			return false;
		}

		return true;
	}

	bool WindowsWindow::CreateWindowsWindow(const DisplayMode & displayMode) {

		RECT rect;
		rect.left		= 100;
		rect.top		= 100;
		rect.right		= rect.left + displayMode.GetWindowSize().x;
		rect.bottom		= rect.top + displayMode.GetWindowSize().y;

		BOOL result = AdjustWindowRectEx(&rect, m_WindowStyle, FALSE, m_WindowExStyle);

		if (result == FALSE) {
			auto errorCode = GetLastError();
			_com_error error(errorCode);
			std::string serror = _bstr_t(error.ErrorMessage());
			ERROR_TRACE(ERR_WINDOW_CREATION_FAILED, "Unable to adjust window client area, " + serror);
			return false;
		}

		m_HWnd = CreateWindowExA(
			m_WindowExStyle,
			WINDOW_CLASS_NAME, DEFAULT_WINDOW_TITLE, m_WindowStyle,
			m_DisplayPos.x, m_DisplayPos.y, rect.right - rect.left, rect.bottom - rect.top,
			nullptr, nullptr, m_HInst, nullptr
		);

		if (m_HWnd == nullptr) {
			auto errorCode = GetLastError();
			_com_error error(errorCode);
			std::string serror = _bstr_t(error.ErrorMessage());
			ERROR_TRACE(ERR_WINDOW_CREATION_FAILED, "Unable to create window, " + serror);
			return false;
		}

		RECT createdRect;
		GetClientRect(m_HWnd, &createdRect);
		
		ShowWindow(m_HWnd, SW_SHOWNORMAL);
		SetForegroundWindow(m_HWnd);
		SetFocus(m_HWnd);

		return true;
	}

	void WindowsWindow::PollEvents() {
		if (PeekMessageA(&m_Message, m_HWnd, 0u, 0u, PM_REMOVE)) {
			TranslateMessage(&m_Message);
			DispatchMessageA(&m_Message);
		}
	}

}