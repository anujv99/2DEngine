#pragma once

#include "math/vec2.h"

namespace prev {

	struct Viewport {
		Vec2 TopLeft;
		Vec2 Dimension;
		Vec2 DepthValues; // x = minDepth, y = maxDepth
	};

	struct ScissorBox {
		int Left;
		int Right;
		int Top;
		int Bottom;
	};

}
