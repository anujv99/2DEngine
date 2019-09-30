#include "pch.h"
#include "cameracontroller.h"

#include "graphics/window.h"

#include "math/vecconversion.h"
#include "utils/input.h"

#define CHECK_MOVEMENT(K, P) if (Input::Ref().IsKeyDown(K)) { m_Position += P; m_Camera->SetPosition(m_Position); }

namespace prev {

	CameraController::CameraController() : 
		m_Camera(nullptr), 
		m_AspectRatio(0.0f),
		m_ZoomLevel(1.0f),
		m_Rotation(0.0f),
		m_Position(0.0f),
		m_CameraTranslationSpeed(2.0f), m_CameraRotationSpeed(3.0f), m_CameraZoomStep(0.1f) {


		Vec2 winSize = ToVec2(Window::Ref().GetDisplayMode().GetWindowSize());
		m_AspectRatio = winSize.x / winSize.y;

		m_Camera = new Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, m_ZoomLevel, -m_ZoomLevel);

		EventHandler::Ref().RegisterEventFunction(BIND_EVENT_FN(CameraController::OnEvent));
	}

	CameraController::~CameraController() {}

	void CameraController::Begin() {
		m_Camera->Begin();
	}

	void CameraController::End() {
		m_Camera->End();
	}

	void CameraController::Update() {
		CHECK_MOVEMENT(PV_KEY_A, Vec2(-m_CameraTranslationSpeed * Timer::GetDeltaTime(), 0.0f));
		CHECK_MOVEMENT(PV_KEY_D, Vec2( m_CameraTranslationSpeed * Timer::GetDeltaTime(), 0.0f));
		CHECK_MOVEMENT(PV_KEY_S, Vec2( 0.0f,-m_CameraTranslationSpeed * Timer::GetDeltaTime()));
		CHECK_MOVEMENT(PV_KEY_W, Vec2( 0.0f, m_CameraTranslationSpeed * Timer::GetDeltaTime()));
	}

	void CameraController::OnEvent(Event & event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(CameraController::WindowResized));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(CameraController::MouseScrolled));
	}

	bool CameraController::WindowResized(WindowResizeEvent & e) {
		m_AspectRatio = (float)e.GetWindowSize().x / (float)e.GetWindowSize().y;

		m_Camera->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, m_ZoomLevel, -m_ZoomLevel);
		return false;
	}

	bool CameraController::MouseScrolled(MouseScrolledEvent & e) {
		int sign = e.GetScrollOffset().y / std::abs(e.GetScrollOffset().y);
		m_ZoomLevel += sign * m_CameraZoomStep;

		m_Camera->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, m_ZoomLevel, -m_ZoomLevel);
		return false;
	}

}