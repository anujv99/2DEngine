#pragma once

#include "vecconversion.h"
#include "graphics/window.h"

namespace prev {

	// Just used for initialization
	static constexpr float S_DEFCAMERA_SCALE_Y		= 10.0f;
	static constexpr float S_DEFCAMERA_NEAR			= -150.0f;
	static constexpr float S_DEFCAMERA_FAR			= 150.0f;
	
	// only use for size
	static Vec2 PixelsToScreen(Vec2 pixels) {
		Vec2 winSize = ToVec2(Window::Ref().GetDisplayMode().GetWindowSize());
		float aspect = winSize.x / winSize.y;
		float xScale = aspect * S_DEFCAMERA_SCALE_Y;

		return (pixels / winSize) * Vec2(xScale, S_DEFCAMERA_SCALE_Y);
	}

	static float PixelsToScreenX(float pixels) {
		Vec2 winSize = ToVec2(Window::Ref().GetDisplayMode().GetWindowSize());
		float aspect = winSize.x / winSize.y;
		float xScale = aspect * S_DEFCAMERA_SCALE_Y;

		return (pixels / winSize.x) * xScale;
	}

	static float PixelsToScreenY(float pixels) {
		Vec2 winSize = ToVec2(Window::Ref().GetDisplayMode().GetWindowSize());

		return (pixels / winSize.y) * S_DEFCAMERA_SCALE_Y;
	}

	static Vec2 ScreenToPixels(Vec2 screen) {
		Vec2 winSize = ToVec2(Window::Ref().GetDisplayMode().GetWindowSize());
		float aspect = winSize.x / winSize.y;
		float xScale = aspect * S_DEFCAMERA_SCALE_Y;

		Vec2 ratio = screen / Vec2(xScale, S_DEFCAMERA_SCALE_Y);

		return ratio * winSize;
	}

}
