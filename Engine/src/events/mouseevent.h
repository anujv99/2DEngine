#pragma once

#include "event.h"

#include "math/vec2i.h"

namespace prev {

	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(int x, int y) :
			m_MousePos(x, y) {}

		inline Vec2i GetMousePos() const { return m_MousePos; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MousePos.x << ", " << m_MousePos.y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		Vec2i m_MousePos;
	};

	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(int xOffset, int yOffset) :
			m_ScrollOffset(xOffset, yOffset) {}

		inline Vec2i GetScrollOffset() const { return m_ScrollOffset; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_ScrollOffset.x << ", " << m_ScrollOffset.y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		Vec2i m_ScrollOffset;
	};

	class MouseButtonEvent : public Event {
	public:
		inline int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)

	protected:
		MouseButtonEvent(int button) :
			m_Button(button) {}

		int m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int button) :
			MouseButtonEvent(button) {}

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int button) :
			MouseButtonEvent(button) {}

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}
