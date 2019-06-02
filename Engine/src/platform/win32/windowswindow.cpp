#include "pch.h"
#include "windowswindow.h"

#include <comdef.h>

#define DEFAULT_WINDOW_TITLE "Retarded Window"
#define WINDOW_CLASS_NAME "Slightly_Retarded_Fart_Box"

#define LBUTTONCODE		0
#define MBUTTONCODE		1
#define RBUTTONCODE		2
#define X1BUTTONCODE	3
#define X2BUTTONCODE	4

namespace prev {

	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (msg) {
			case WM_MOUSEWHEEL:
			{
				short delta = GET_WHEEL_DELTA_WPARAM(wParam);
				MouseScrolledEvent e(0, (int)delta);
				REGISTER_EVENT(e);
				break;
			}
			case WM_MOUSEMOVE:
			{
				POINTS pt = MAKEPOINTS(lParam);
				MouseMovedEvent e((int)pt.x, (int)pt.y);
				REGISTER_EVENT(e);
				break;
			}
			case WM_LBUTTONDOWN:
			{
				MouseButtonPressedEvent e(LBUTTONCODE);
				REGISTER_EVENT(e);
				break;
			}
			case WM_RBUTTONDOWN:
			{
				MouseButtonPressedEvent e(RBUTTONCODE);
				REGISTER_EVENT(e);
				break;
			}
			case WM_MBUTTONDOWN:
			{
				MouseButtonPressedEvent e(MBUTTONCODE);
				REGISTER_EVENT(e);
				break;
			}
			case WM_XBUTTONDOWN:
			{
				WORD button = GET_XBUTTON_WPARAM(wParam);
				UINT buttonCode = X1BUTTONCODE;

				if (button == 0x0002)
					buttonCode = X2BUTTONCODE;

				MouseButtonPressedEvent e(buttonCode);
				REGISTER_EVENT(e);
				break;
			}
			case WM_LBUTTONUP:
			{
				MouseButtonReleasedEvent e(LBUTTONCODE);
				REGISTER_EVENT(e);
				break;
			}
			case WM_RBUTTONUP:
			{
				MouseButtonReleasedEvent e(RBUTTONCODE);
				REGISTER_EVENT(e);
				break;
			}
			case WM_MBUTTONUP:
			{
				MouseButtonReleasedEvent e(MBUTTONCODE);
				REGISTER_EVENT(e);
				break;
			}
			case WM_XBUTTONUP:
			{
				WORD button = GET_XBUTTON_WPARAM(wParam);
				UINT buttonCode = X1BUTTONCODE;

				if (button == 0x0002)
					buttonCode = X2BUTTONCODE;

				MouseButtonReleasedEvent e(buttonCode);
				REGISTER_EVENT(e);
				break;
			}
			case WM_KEYDOWN:
			{
				bool repeatCount = (lParam & 0x40000000);
				KeyPressedEvent e((int)wParam, repeatCount);
				REGISTER_EVENT(e);
				break;
			}
			case WM_KEYUP:
			{
				KeyReleasedEvent e((int)wParam);
				REGISTER_EVENT(e);
				break;
			}
			case WM_CHAR:
			{
				CharacterEvent e((unsigned char)wParam);
				REGISTER_EVENT(e);
				break;
			}
			case WM_SIZE:
			{
				POINTS pt = MAKEPOINTS(lParam);
				WindowResizeEvent e(pt.x, pt.y);
				REGISTER_EVENT(e);
				break;
			}
			case WM_MOVE:
			{
				POINTS pt = MAKEPOINTS(lParam);
				WindowMoveEvent e(pt.x, pt.y);
				REGISTER_EVENT(e);
				break;
			}
			case WM_CLOSE:
			{
				WindowCloseEvent e;
				REGISTER_EVENT(e);
				break;
			}
			default:
				return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		return 0;
	}

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