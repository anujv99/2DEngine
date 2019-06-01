#pragma once

#include "src/math/vec2i.h"

namespace prev {

	enum class WindowStyle {
		WINDOWED,
		BORDERLESS,
		FULLSCREEN
	};

	struct DisplayMode {
	public:
		Vec2i GetWindowSize() const { return WindowSize; }
		WindowStyle GetWindowStyle() const { return Style; }
		bool IsWindowFullscreen() const { return IsFullscreen; }

		void SetFullscreen(bool fullscreen) { IsFullscreen = fullscreen; }
		void SetWindowStyle(WindowStyle style) { Style = style; }
	public:
		DisplayMode() :
			WindowSize(1280, 720), Style(WindowStyle::WINDOWED), IsFullscreen(false) {}
	private:
		Vec2i WindowSize;
		WindowStyle Style;
		bool IsFullscreen;
	};

	class Window : public Singleton<Window> {
		friend class Application;
		friend class Singleton<Window>;
	public:
		DisplayMode GetDisplayMode() const { return m_DisplayMode; }
	protected:
		virtual bool GetKeyboardKeyState(int keyCode) = 0;
		virtual bool GetMouseButtonState(int buttonCode) = 0;
		virtual Vec2i GetMousePosition() = 0;
		virtual void PollEvents() = 0;
	protected:
		Window(const DisplayMode & displayMode) : m_DisplayMode(displayMode) { }
		virtual ~Window() {}
		static Window * CreateEngineWindow(const DisplayMode & displayMode);
	private:
		DisplayMode m_DisplayMode;
	};

	template<>
	template<>
	inline void Singleton<Window>::CreateInst(const DisplayMode & displayMode) {
		if (!s_Instance) s_Instance = Window::CreateEngineWindow(displayMode);
	}

}
