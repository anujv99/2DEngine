#include "pch.h"
#include "windowswindow.h"

#include "systemres/resource.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "examples/imgui_impl_win32.h"

#include <comdef.h>

#define DEFAULT_WINDOW_TITLE "Retarded Window"
#define WINDOW_CLASS_NAME "Slightly_Retarded_Fart_Box"

#define LBUTTONCODE		0
#define MBUTTONCODE		1
#define RBUTTONCODE		2
#define X1BUTTONCODE	3
#define X2BUTTONCODE	4

#ifdef IMGUI_ENABLED
extern LRESULT CALLBACK ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

namespace prev {

	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		
#ifdef IMGUI_ENABLED
		if (GImGui) {
			if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
				return 0;
		}
#endif

		switch (msg) {
			
			/************************* RAW INPUT MESSAGES *************************/
			case WM_INPUT:
			{
				UINT size = 0u;
				GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));

				LPBYTE lpb = new BYTE[size];

				if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &size, sizeof(RAWINPUTHEADER)) != size) {
					LOG_ERROR("GetRawInputData does not return correct size");
					break;
				}

				RAWINPUT * raw = (RAWINPUT *)lpb;
				if (raw->header.dwType == RIM_TYPEMOUSE) {
					if (raw->data.mouse.lLastX != 0 || raw->data.mouse.lLastY != 0) {
						MouseMovedRawEvent e((int)raw->data.mouse.lLastX, -(int)raw->data.mouse.lLastY);
						REGISTER_EVENT(e);
					}
				}

				delete[] lpb;

				break;
			}
			/************************* END RAW INPUT MESSAGES *************************/

			/************************* MOUSE MESSAGES *************************/
			case WM_SETCURSOR:
			{
				break;
			}
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
			/************************* END MOUSE MESSAGES *************************/

			/************************* KEYBOARD MESSAGES *************************/
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
			/************************* END KEYBOARD MESSAGES *************************/

			/************************* WINDOW MESSAGES *************************/
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
			/************************* END WINDOW MESSAGES *************************/
		}

		return DefWindowProcA(hWnd, msg, wParam, lParam);
	}

	Window * Window::CreateEngineWindow(const StrongHandle<Monitor> & monitor, unsigned int displayMode) {
		WindowsWindow * window = new WindowsWindow(monitor, displayMode);
		return (Window *)window;
	}

	WindowsWindow::WindowsWindow(const StrongHandle<Monitor> & monitor, unsigned int displayMode) : Window(monitor, displayMode) {
		m_HInst = GetModuleHandle(nullptr);

		StrongHandle<DisplayMode> disMode = m_Monitor->GetDisplayModes()[displayMode];

		Vec2i diff = monitor->GetResolution() - monitor->GetDisplayModes()[displayMode]->GetWindowSize();
		m_DisplayPos = monitor->TopLeft + diff / 2;

		switch (disMode->GetWindowStyle()) {
			case WINDOW_STYLE_WINDOWED:
				m_WindowClassStyle	= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
				m_WindowExStyle		= WS_EX_APPWINDOW;
				m_WindowStyle		= WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_OVERLAPPEDWINDOW;
				break;
			case WINDOW_STYLE_FULLSCREEN:
				m_DisplayPos = Vec2i(0u, 0u);
			case WINDOW_STYLE_BORDERLESS:
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

		bool result = RegisterWindowClass(*disMode);
		if (!result) {
			m_IsWindowReady = false;
			return;
		}

		result = CreateWindowsWindow(*disMode);
		if (!result) {
			m_IsWindowReady = false;
			return;
		}

		RegisterRawInput();

		EventHandler::Ref().RegisterEventFunction(BIND_EVENT_FN(WindowsWindow::OnEvent));

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

	bool WindowsWindow::RegisterRawInput() {
		RAWINPUTDEVICE rid[1]	= {};
		rid[0].usUsagePage		= 0x01;
		rid[0].usUsage			= 0x02;
		rid[0].dwFlags			= 0l;
		rid[0].hwndTarget		= nullptr;

		if (RegisterRawInputDevices(rid, (UINT)std::size(rid), sizeof(rid[0])) == FALSE) {
			ERROR_TRACE(ERR_WINDOW_INTERNAL_ERROR, "Unable to register raw input device");
			return false;
		}

		return true;
	}

	void WindowsWindow::OnEvent(Event & event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(WindowsWindow::WindowResized));
		dispatcher.Dispatch<WindowMoveEvent>(BIND_EVENT_FN(WindowsWindow::WindowMoved));
	}

	bool WindowsWindow::WindowResized(WindowResizeEvent & e) {
		//m_DisplayMode.WindowSize = e.GetWindowSize();
		return false;
	}

	bool WindowsWindow::WindowMoved(WindowMoveEvent & e) {
		m_DisplayPos = e.GetWindowPos();
		return false;
	}

	void WindowsWindow::PollEvents() {
		while (PeekMessageA(&m_Message, m_HWnd, 0u, 0u, PM_REMOVE)) {
			TranslateMessage(&m_Message);
			DispatchMessageA(&m_Message);
		}
	}

	void WindowsWindow::SetPosition(Vec2i pos) {
		RECT rc;
		GetWindowRect(m_HWnd, &rc);
		BOOL result = MoveWindow(m_HWnd, pos.x, pos.y, std::abs(rc.right - rc.left), std::abs(rc.top - rc.bottom), FALSE);
		if (result == FALSE) {
			LOG_ERROR("Unable to set new window pos {}", pos);
		}
	}

	void WindowsWindow::SetWindowSize(Vec2i size) {
		RECT rc;
		GetWindowRect(m_HWnd, &rc);

		RECT newRc;
		newRc.left = 0;
		newRc.top = 0;
		newRc.right = size.x;
		newRc.bottom = size.y;

		AdjustWindowRectEx(&newRc, m_WindowStyle, FALSE, m_WindowExStyle);

		BOOL result = MoveWindow(m_HWnd, rc.left, rc.top, std::abs(newRc.right - newRc.left), std::abs(newRc.top - newRc.bottom), FALSE);
		if (result == FALSE) {
			LOG_ERROR("Unable to set new window size {}", size);
		}
	}

}