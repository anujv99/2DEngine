#include "pch.h"
#include "cameracontroller.h"

#include "graphics/window.h"

#include "math/vecconversion.h"
#include "utils/input.h"

#include <imgui.h>
#include "utils/layerstack.h"

#define CHECK_MOVEMENT(K, P) if (Input::Ref().IsKeyDown(K)) { m_Position += P; m_Camera->SetPosition(m_Position); }

namespace prev {

	CameraController::CameraController() :
		m_Camera(nullptr),
		m_AspectRatio(0.0f),
		m_ZoomLevel(1.0f),
		m_Rotation(0.0f),
		m_Position(0.0f),
		m_CameraTranslationSpeed(2.0f), m_CameraRotationSpeed(3.0f), m_CameraZoomStep(0.1f), 
		m_IsGuiOpen(false), m_IsFreeroam(false) {


		Vec2 winSize = ToVec2(Window::Ref().GetDisplayMode().GetWindowSize());
		m_AspectRatio = winSize.x / winSize.y;

		m_Camera = new Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, m_ZoomLevel, -m_ZoomLevel);

		EventHandler::Ref().RegisterEventFunction(BIND_EVENT_FN(CameraController::OnEvent));

		IMGUI_CALL(ImGuiLayer * imlayer = LayerStack::Ref().GetImGuiLayer());
		IMGUI_CALL(if (imlayer != nullptr))
		IMGUI_CALL(imlayer->AddGuiFunction(std::bind(&CameraController::GuiFunction, this)));
		IMGUI_CALL(imlayer->SetSettingBoolean("Canera Settings", &m_IsGuiOpen));
	}

	CameraController::~CameraController() {}

	void CameraController::Begin() {
		m_Camera->Begin();
	}

	void CameraController::End() {
		m_Camera->End();
	}

	void CameraController::Update() {
		if (!m_IsFreeroam)
			return;

		CHECK_MOVEMENT(PV_KEY_A, Vec2(-m_CameraTranslationSpeed * Timer::GetDeltaTime(), 0.0f));
		CHECK_MOVEMENT(PV_KEY_D, Vec2( m_CameraTranslationSpeed * Timer::GetDeltaTime(), 0.0f));
		CHECK_MOVEMENT(PV_KEY_S, Vec2( 0.0f,-m_CameraTranslationSpeed * Timer::GetDeltaTime()));
		CHECK_MOVEMENT(PV_KEY_W, Vec2( 0.0f, m_CameraTranslationSpeed * Timer::GetDeltaTime()));
	}

	prev::Vec2 CameraController::ScreenToPixels(Vec2 coords) const {
		Vec2 screenSize = ToVec2(Window::Ref().GetDisplayMode().GetWindowSize());

		float x = std::abs(GetScreenCoordsX().y - GetScreenCoordsX().x);
		float y = std::abs(GetScreenCoordsY().y - GetScreenCoordsY().x);

		return Vec2(screenSize.x / x, screenSize.y / y) * coords;
	}

	prev::Vec2 CameraController::PixelsToScreen(Vec2 coords) const {
		Vec2 screenSize = ToVec2(Window::Ref().GetDisplayMode().GetWindowSize());
		float x = GetScreenCoordsX().y - GetScreenCoordsX().x;
		float y = GetScreenCoordsY().y - GetScreenCoordsY().x;

		return Vec2(x / screenSize.x, y / screenSize.y) * coords;
	}

	void CameraController::OnEvent(Event & event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(CameraController::WindowResized));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(CameraController::MouseScrolled));
	}

	void CameraController::GuiFunction() {

		if (!m_IsGuiOpen)
			return;

		Vec2 pos = m_Camera->GetPosition();
		float rot = ToRadians(m_Camera->GetRotation());

		IMGUI_CALL(
			ImGui::Begin("Camera Settings", &m_IsGuiOpen);
			if (ImGui::DragFloat2("Position", (float *)& pos, 0.04f)) {
				m_Camera->SetPosition(pos);
			}
			if (ImGui::DragFloat("Zoom", &m_ZoomLevel, 0.04f)) {
				m_Camera->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, m_ZoomLevel, -m_ZoomLevel);
			}
			if (ImGui::SliderAngle("Rotatin", &rot)) {
				m_Camera->SetRotation(ToDegrees(rot));
			}
			ImGui::Checkbox("Freeroam", &m_IsFreeroam);
			ImGui::End();
		);
	}

	bool CameraController::WindowResized(WindowResizeEvent & e) {
		m_AspectRatio = (float)e.GetWindowSize().x / (float)e.GetWindowSize().y;

		m_Camera->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, m_ZoomLevel, -m_ZoomLevel);
		return false;
	}

	bool CameraController::MouseScrolled(MouseScrolledEvent & e) {
		if (!m_IsFreeroam)
			return false;

		int sign = e.GetScrollOffset().y / std::abs(e.GetScrollOffset().y);
		m_ZoomLevel += sign * m_CameraZoomStep;

		m_Camera->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, m_ZoomLevel, -m_ZoomLevel);
		return false;
	}

}