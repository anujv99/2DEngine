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
		m_DisplaySize = std::make_pair(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

		m_DisplayPos.first = (m_DisplaySize.first - displayMode.GetWindowSize().first) / 2;
		m_DisplayPos.second = (m_DisplaySize.second - displayMode.GetWindowSize().second) / 2;

		switch (displayMode.GetWindowStyle()) {
			case WindowStyle::WINDOWED:
				m_WindowClassStyle	= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
				m_WindowExStyle		= WS_EX_APPWINDOW;
				m_WindowStyle		= WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU;
				break;
			case WindowStyle::FULLSCREEN:
				m_DisplayPos = std::make_pair(0u, 0u);
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

	std::pair<int, int> WindowsWindow::GetMousePosition() {
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(m_HWnd, &pt);
		return std::pair<int, int>(pt.x, pt.y);
	}

	bool WindowsWindow::RegisterWindowClass(const DisplayMode & displayMode) {
		WNDCLASSEXA wc;
		ZeroMemory(&wc, sizeof(wc));

		wc.cbSize				= sizeof(wc);
		wc.style				= m_WindowClassStyle;
		wc.lpfnWndProc			= DefWindowProcA;
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
		rect.left		= 0;
		rect.top		= 0;
		rect.right		= displayMode.GetWindowSize().first;
		rect.bottom		= displayMode.GetWindowSize().second;

		BOOL hasMenu = displayMode.GetWindowStyle() == WindowStyle::WINDOWED;

		BOOL result = AdjustWindowRectEx(&rect, m_WindowStyle, hasMenu, m_WindowExStyle);

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
			m_DisplayPos.first, m_DisplayPos.second, rect.right - rect.left, rect.bottom - rect.top,
			nullptr, nullptr, m_HInst, nullptr
		);

		if (m_HWnd == nullptr) {
			auto errorCode = GetLastError();
			_com_error error(errorCode);
			std::string serror = _bstr_t(error.ErrorMessage());
			ERROR_TRACE(ERR_WINDOW_CREATION_FAILED, "Unable to create window, " + serror);
			return false;
		}
		
		ShowWindow(m_HWnd, SW_SHOW);
		SetForegroundWindow(m_HWnd);
		SetFocus(m_HWnd);

		ZeroMemory(&m_Message, sizeof(m_Message));

		return true;
	}

	void WindowsWindow::PollEvents() {
		if (PeekMessageA(&m_Message, m_HWnd, 0u, 0u, PM_REMOVE)) {
			TranslateMessage(&m_Message);
			DispatchMessageA(&m_Message);
		}
	}

}