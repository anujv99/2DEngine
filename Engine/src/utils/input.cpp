#include "pch.h"
#include "input.h"

namespace prev {

	bool Input::IsKeyDown(unsigned short keyCode) {
		return m_Keys[keyCode];
	}

	bool Input::IsKeyPressed(unsigned short keyCode) {
		return m_Keys[keyCode] && !m_PrevKeys[keyCode];
	}

	bool Input::IsKeyUp(unsigned short keyCode) {
		return !m_Keys[keyCode];
	}

	bool Input::IsKeyReleased(unsigned short keyCode) {
		return !m_Keys[keyCode] && m_PrevKeys[keyCode];
	}

	bool Input::IsMouseButtonDown(unsigned short keyCode) {
		return m_MouseButtons[keyCode];
	}

	bool Input::IsMouseButtonPressed(unsigned short keyCode) {
		return m_MouseButtons[keyCode] && !m_PrevMouseButtons[keyCode];
	}

	bool Input::IsMouseButtonUp(unsigned short keyCode) {
		return !m_MouseButtons[keyCode];
	}

	bool Input::IsMouseButtonReleased(unsigned short keyCode) {
		return !m_MouseButtons[keyCode] && m_PrevMouseButtons[keyCode];
	}

	bool Input::IsCharacterPressed() {
		return !m_CharacterPressedBuffer.empty();
	}

	Input::Input() {
		EventHandler::Ref().RegisterEventFunction(BIND_EVENT_FN(Input::OnEvent));

		for (UINT i = 0; i < MAX_KEYS; i++) {
			m_Keys[i] = 0;
		}

		for (UINT i = 0; i < MAX_MOUSE_BUTTONS; i++) {
			m_MouseButtons[i] = 0;
		}

		m_WindowSize = Window::Ref().GetDisplayMode().GetWindowSize();
	}

	void Input::OnEvent(Event & e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Input::KeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(Input::KeyReleased));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(Input::MouseMoved));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(Input::MouseScrolled));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(Input::MousePressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(Input::MouseReleased));
		dispatcher.Dispatch<CharacterEvent>(BIND_EVENT_FN(Input::CharacterPressed));
	}

	bool Input::KeyPressed(KeyPressedEvent & e) {
		m_Keys[e.GetKeyCode()] = true;
		return false;
	}

	bool Input::KeyReleased(KeyReleasedEvent & e) {
		m_Keys[e.GetKeyCode()] = false;
		return false;
	}

	bool Input::MouseMoved(MouseMovedEvent & e) {
		m_MousePos = e.GetMousePos();
		return false;
	}

	bool Input::MouseScrolled(MouseScrolledEvent & e) {
		m_MouseScrollDelta = e.GetScrollOffset();
		return false;
	}

	bool Input::MousePressed(MouseButtonPressedEvent & e) {
		m_MouseButtons[e.GetMouseButton()] = true;
		return false;
	}

	bool Input::MouseReleased(MouseButtonReleasedEvent & e) {
		m_MouseButtons[e.GetMouseButton()] = false;
		return false;
	}

	bool Input::CharacterPressed(CharacterEvent & e) {
		m_CharacterPressedBuffer.push_back(e.GetPressedChar());
		return false;
	}

	void Input::Update() {
		m_PrevMousePos = m_MousePos;
		m_MouseScrollDelta = Vec2i(0);
		std::memcpy(m_PrevKeys, m_Keys, sizeof(m_Keys));
		std::memcpy(m_PrevMouseButtons, m_MouseButtons, sizeof(m_MouseButtons));

		m_CharacterPressedBuffer.clear();
		m_CharacterPressedBuffer.reserve(MAX_CHARACTER_PER_FRAME);
	}

}