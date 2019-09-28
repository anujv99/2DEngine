#pragma once

#include "src/math/vec2i.h"

namespace prev {

	enum WindowStyle {
		WINDOW_STYLE_WINDOWED,
		WINDOW_STYLE_BORDERLESS,
		WINDOW_STYLE_FULLSCREEN
	};

	struct DisplayMode {
		friend class GraphicsContext;
	public:
		Vec2i GetWindowSize() const { return WindowSize; }
		WindowStyle GetWindowStyle() const { return Style; }
		bool IsWindowFullscreen() const { return Style == WINDOW_STYLE_FULLSCREEN; }

		inline void SetWindowStyle(WindowStyle style) { Style = style; }
		inline void SetMultisample(unsigned int numSamples) { Samples = numSamples; }
		inline unsigned int GetSamples() const {return Samples; }
		inline bool IsMultisampled() const { return Samples != 1; }
	private:
		DisplayMode() :
			WindowSize(0, 0), Style(WindowStyle::WINDOW_STYLE_WINDOWED), Samples(1u) {}
	private:
		Vec2i WindowSize;
		WindowStyle Style;
		unsigned int Samples;
	};

	class Window : public Singleton<Window> {
		friend class Application;
		friend class Singleton<Window>;
	public:
		DisplayMode GetDisplayMode() const { return m_DisplayMode; }
		inline bool IsWindowReady() const { return m_IsWindowReady; }
	public:
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
