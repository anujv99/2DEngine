#include "pch.h"
#include "mvp.h"

#include "graphics/window.h"
#include "vecconversion.h"

namespace prev {

	const float MVP::s_Near			= -150.0f;
	const float MVP::s_Far			= 150.0f;
	const float MVP::s_YScale		= 10.0f;
	float MVP::s_XScale				= 0.0f;

	MVP::MVP() {
		m_WindowSize = ToVec2(Window::Ref().GetDisplayMode().GetWindowSize());

		float aspect = m_WindowSize.x / m_WindowSize.y;

		s_XScale = aspect * s_YScale;

		Mat4 proj = Ortho(-s_XScale / 2, s_XScale / 2, -s_YScale / 2, s_YScale / 2, s_Near, s_Far);

		Projection().Push();
		Projection().Load(proj);

		EventHandler::Ref().RegisterEventFunction(std::bind(&MVP::EventFunction, this, std::placeholders::_1));
	}

	MVP::~MVP() {
		Projection().Pop();
	}

	prev::Mat4 MVP::Get() {
		if (Model().IsDirty() || View().IsDirty() || Projection().IsDirty()) {
			Mat4 viewInv = prev::inverseRT(View().Get());
			m_MVPCache = Projection().Get() * Model().Get();
			Model().SetDirtyOff();
			Projection().SetDirtyOff();
			View().SetDirtyOff();
		}

		return m_MVPCache;
	}

	bool MVP::IsDirty() {
		return Model().IsDirty() || View().IsDirty() || Projection().IsDirty();
	}

	void MVP::EventFunction(Event & e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(MVP::WindowResize));
	}

	bool MVP::WindowResize(WindowResizeEvent & e) {
		m_WindowSize = ToVec2(e.GetWindowSize());
		float aspect = m_WindowSize.x / m_WindowSize.y;
		s_XScale = aspect * s_YScale;
		Mat4 proj = Ortho(-s_XScale / 2, s_XScale / 2, -s_YScale / 2, s_YScale / 2, s_Near, s_Far);

		Projection().Load(proj);

		return false;
	}

}