#pragma once

#include "math/camera.h"

namespace prev {

	class CameraController : public Singleton<CameraController> {
		friend class Singleton<CameraController>;
	private:
		CameraController();
		~CameraController();
	public:
		inline StrongHandle<Camera> & GetCamera() { return m_Camera; }

		void Begin();
		void End();
		void Update();

		Vec2 GetScreenCoordsX() const { return Vec2(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel); }
		Vec2 GetScreenCoordsY() const { return Vec2(-m_ZoomLevel, m_ZoomLevel); }

		Vec2 ScreenToPixels(Vec2 coords) const;
		Vec2 PixelsToScreen(Vec2 coords) const;
	private:
		void OnEvent(Event & event);
		void GuiFunction();

		bool WindowResized(WindowResizeEvent & e);
		bool MouseScrolled(MouseScrolledEvent & e);
	private:
		StrongHandle<Camera> m_Camera;

		float m_AspectRatio;
		float m_ZoomLevel;

		float m_Rotation;
		Vec2 m_Position;
		float m_CameraTranslationSpeed, m_CameraRotationSpeed, m_CameraZoomStep;

		bool m_IsGuiOpen;
		bool m_IsFreeroam;
	};

}
