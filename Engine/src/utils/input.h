#pragma once

#include "src/common/singleton.h"
#include "src/graphics/window.h"
#include "src/events/event.h"


#define MAX_KEYS 0xFF
#define MAX_MOUSE_BUTTONS 5

namespace prev {

	class Input : public Singleton<Input> {
		friend class Application;
		friend class Singleton<Input>;
	public:
		bool IsKeyDown(unsigned short keyCode);
		bool IsKeyPressed(unsigned short keyCode);
		bool IsKeyUp(unsigned short keyCode);
		bool IsKeyReleased(unsigned short keyCode);

		bool IsMouseButtonDown(unsigned short keyCode);
		bool IsMouseButtonPressed(unsigned short keyCode);
		bool IsMouseButtonUp(unsigned short keyCode);
		bool IsMouseButtonReleased(unsigned short keyCode);

		inline Vec2i GetMousePosition() const { return Vec2i(m_MousePos.x, m_WindowSize.y - m_MousePos.y); }
		inline Vec2i GetMouseDeltaPosition() const { return m_MousePos - m_PrevMousePos; }
		inline Vec2i GetMouseScrollDelta() const { return m_MouseScrollDelta; }
	protected:
		Input();
	private:
		void OnEvent(Event & e);
		bool KeyPressed(KeyPressedEvent & e);
		bool KeyReleased(KeyReleasedEvent & e);
		bool MouseMoved(MouseMovedEvent & e);
		bool MouseScrolled(MouseScrolledEvent & e);
		bool MousePressed(MouseButtonPressedEvent & e);
		bool MouseReleased(MouseButtonReleasedEvent & e);

		void Update();
	private:
		bool m_Keys[MAX_KEYS];
		bool m_PrevKeys[MAX_KEYS]; //Previous Key States

		bool m_MouseButtons[MAX_MOUSE_BUTTONS];
		bool m_PrevMouseButtons[MAX_MOUSE_BUTTONS]; //Previous Key States

		Vec2i m_MousePos = Vec2i(0);
		Vec2i m_PrevMousePos = Vec2i(0);
		Vec2i m_MouseScrollDelta = Vec2i(0);
		Vec2i m_WindowSize = Vec2i(0);
	};

}
