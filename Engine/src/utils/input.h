#pragma once

#include "src/common/singleton.h"
#include "src/graphics/window.h"
#include "src/events/event.h"
#include "pvkeycodes.h" // Contains defines for buttons
#include "math/vec2.h"

#define MAX_KEYS 0xFF
#define MAX_MOUSE_BUTTONS 5
#define MAX_CHARACTER_PER_FRAME 16

namespace prev {

	class Input : public Singleton<Input> {
		friend class Application;
		friend class Singleton<Input>;
	public:
		static const char KEY_BACKSPACE = '\b';
		static const char KEY_RETURN = '\r';
	public:
		bool IsKeyDown(unsigned short keyCode);
		bool IsKeyPressed(unsigned short keyCode);
		bool IsKeyUp(unsigned short keyCode);
		bool IsKeyReleased(unsigned short keyCode);

		bool IsMouseButtonDown(unsigned short keyCode);
		bool IsMouseButtonPressed(unsigned short keyCode);
		bool IsMouseButtonUp(unsigned short keyCode);
		bool IsMouseButtonReleased(unsigned short keyCode);

		bool IsCharacterPressed();

		inline const std::vector<char> & GetPressedCharacterBuffer() const { return m_CharacterPressedBuffer; }

		inline Vec2 GetMousePosition() const { return m_MousePos; }
		inline Vec2 GetMouseDeltaPosition() const { return m_MousePos - m_PrevMousePos; }
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
		bool CharacterPressed(CharacterEvent & e);

		void Update();
	private:
		bool m_Keys[MAX_KEYS];
		bool m_PrevKeys[MAX_KEYS]; //Previous Key States

		bool m_MouseButtons[MAX_MOUSE_BUTTONS];
		bool m_PrevMouseButtons[MAX_MOUSE_BUTTONS]; //Previous Key States

		Vec2 m_MousePos = Vec2(0);
		Vec2 m_PrevMousePos = Vec2(0);
		Vec2i m_MouseScrollDelta = Vec2i(0);
		Vec2i m_WindowSize = Vec2i(0);

		std::vector<char> m_CharacterPressedBuffer;
	};

}
