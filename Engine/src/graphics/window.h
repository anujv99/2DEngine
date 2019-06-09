#pragma once

#include "src/math/vec2i.h"

namespace prev {

	enum class WindowStyle {
		WINDOWED,
		BORDERLESS,
		FULLSCREEN
	};

	struct DisplayMode {
		friend class GraphicsContext;
	public:
		Vec2i GetWindowSize() const { return WindowSize; }
		WindowStyle GetWindowStyle() const { return Style; }
		bool IsWindowFullscreen() const { return IsFullscreen; }

		void SetFullscreen(bool fullscreen) { IsFullscreen = fullscreen; }
		void SetWindowStyle(WindowStyle style) { Style = style; }
	private:
		DisplayMode() :
			WindowSize(0, 0), Style(WindowStyle::WINDOWED), IsFullscreen(false) {}
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
		inline bool IsWindowReady() const { return m_IsWindowReady; }
	protected:
		virtual void PollEvents() = 0;
		virtual uintptr_t GetWindowRawPointer() = 0;
	protected:
		Window(const DisplayMode & displayMode) : m_DisplayMode(displayMode) { }
		virtual ~Window() {}
		static Window * CreateEngineWindow(const DisplayMode & displayMode);

		bool m_IsWindowReady = true;
	private:
		DisplayMode m_DisplayMode;

	};

	template<>
	template<>
	inline void Singleton<Window>::CreateInst(const DisplayMode & displayMode) {
		if (!s_Instance) s_Instance = Window::CreateEngineWindow(displayMode);
	}

}
