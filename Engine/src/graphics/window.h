#pragma once

#include "src/math/vec2i.h"

namespace prev {

	enum WindowStyle {
		WINDOW_STYLE_WINDOWED,
		WINDOW_STYLE_BORDERLESS,
		WINDOW_STYLE_FULLSCREEN
	};

	struct DisplayMode : public HandledObject<DisplayMode> {
		friend class GraphicsContext;
	public:
		Vec2i GetWindowSize() const { return WindowSize; }
		WindowStyle GetWindowStyle() const { return Style; }
		bool IsWindowFullscreen() const { return Style == WINDOW_STYLE_FULLSCREEN; }

		inline void SetWindowStyle(WindowStyle style) const { Style = style; }
		inline void SetMultisample(unsigned int numSamples) const { Samples = numSamples; }
		inline unsigned int GetSamples() const {return Samples; }
		inline bool IsMultisampled() const { return Samples != 1; }
	protected:
		DisplayMode() :
			WindowSize(0, 0), Style(WINDOW_STYLE_BORDERLESS), Samples(4u) {}
	private:
		Vec2i WindowSize;
		mutable WindowStyle Style;
		mutable unsigned int Samples;
	};

	struct Monitor : public HandledObject<Monitor> {
		friend class GraphicsContext;
		friend class WindowsWindow;
	public:
		Vec2i GetResolution() const {
			return Vec2i(std::abs(TopLeft.x - BottomRight.x), std::abs(TopLeft.y - BottomRight.y));
		}
		const std::vector<StrongHandle<DisplayMode>> GetDisplayModes() const { return DisplayModes; }
	private:
		Monitor() : TopLeft(0), BottomRight(0) {}
	private:
		Vec2i TopLeft;
		Vec2i BottomRight;
		std::string MonitorDescription;
		std::vector<StrongHandle<DisplayMode>> DisplayModes;
	};

	struct GraphicsAdapter : public HandledObject<GraphicsAdapter> {
		friend class GraphicsContext;
	public:
		const std::vector<StrongHandle<Monitor>> GetMonitors() const { return Monitors; }
	protected:
		GraphicsAdapter() : DedicatedVideoMemory(0) {}
	private:
		std::string AdapterDescription;
		unsigned int DedicatedVideoMemory;
		std::vector<StrongHandle<Monitor>> Monitors;
	};

	class Window : public Singleton<Window> {
		friend class Application;
		friend class Singleton<Window>;
	public:
		const DisplayMode GetDisplayMode() const { return *(m_Monitor->GetDisplayModes()[m_DisplayMode]); }
		StrongHandle<DisplayMode> GetDisplayModePointer() {	return m_Monitor->GetDisplayModes()[m_DisplayMode];	}

		inline bool IsWindowReady() const { return m_IsWindowReady; }
	public:
		virtual void PollEvents() = 0;
		virtual void SetPosition(Vec2i pos) = 0;
		virtual void SetWindowSize(Vec2i size) = 0;
		virtual uintptr_t GetWindowRawPointer() = 0;

		virtual Vec2i GetPosition() = 0;
	protected:
		Window(const StrongHandle<Monitor> & monitor, unsigned int displayMode) : m_Monitor(monitor), m_DisplayMode(displayMode) { }
		virtual ~Window() {}
		static Window * CreateEngineWindow(const StrongHandle<Monitor> & monitor, unsigned int displayMode = 0);

		bool m_IsWindowReady = true;
	protected:
		StrongHandle<Monitor> m_Monitor;
		unsigned int m_DisplayMode;
	};

	template<>
	template<>
	inline void Singleton<Window>::CreateInst(const StrongHandle<Monitor> & monitor) {
		if (!s_Instance) s_Instance = Window::CreateEngineWindow(monitor);
	}

	template<>
	template<>
	inline void Singleton<Window>::CreateInst(const StrongHandle<Monitor> & monitor, const unsigned int & displayMode) {
		if (!s_Instance) s_Instance = Window::CreateEngineWindow(monitor, displayMode);
	}

}
