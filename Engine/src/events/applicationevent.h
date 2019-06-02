#pragma once

#include "event.h"

#include "math/vec2i.h"

namespace prev {

	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) : 
			m_WindowSize(width, height) {}
		inline Vec2i GetWindowSize() const { return m_WindowSize; };
		
		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_WindowSize.x << ", " << m_WindowSize.y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		Vec2i m_WindowSize;
	};

	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowCloseEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowMoveEvent : public Event {
	public:
		WindowMoveEvent(unsigned int xpos, unsigned int ypos) :
			m_WindowPos(xpos, ypos) {}

		inline Vec2i GetWindowPos() const { return m_WindowPos; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowMovedEvent: " << m_WindowPos.x << ", " << m_WindowPos.y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowMoved)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		Vec2i m_WindowPos;
	};

	class AppTickEvent : public Event {
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event {
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

}