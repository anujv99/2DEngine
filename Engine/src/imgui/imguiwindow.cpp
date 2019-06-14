#include "pch.h"
#include "imguiwindow.h"

namespace prev {

	ImGuiWindow::ImGuiWindow() {
		Position = Vec2i(-1);
		Dimension = Vec2i(0);
		DimensionAutoSize = Vec2i(0);
		DimensionAutoSizePrev = Vec2i(0);
		ScrollPos = Vec2i(0);

		TitleBarMinWidth = 0;

		BackgroundAlpha = 0.5f;

		AutoSize = false;
		Minimized = false;
		IsNewWindow = true;
		Locked = false;

		FramesSinceUpdate = 0;

		Name = "";
	}

}