#pragma once

#include "math/vec2.h"
#include "math/vec2i.h"

namespace prev {

	struct ImGuiWindow : public HandledObject<ImGuiWindow> {
		std::string Name;

		Vec2i Position;
		Vec2i Dimension;
		Vec2i DimensionAutoSize;
		Vec2i DimensionAutoSizePrev;
		Vec2i ScrollPos;

		int TitleBarMinWidth;

		float BackgroundAlpha;

		bool AutoSize;
		bool Minimized;
		bool IsNewWindow;
		bool Locked;

		int FramesSinceUpdate;

		ImGuiWindow();
	};

}
