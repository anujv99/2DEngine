#pragma once

#include "gmbind.h"

// FONT

GM_REG_NAMESPACE(Font) {
	GM_BIND_TYPEID(Font);
	GM_BIND_DECL(Font);
}

// TEXTURE

GM_REG_NAMESPACE(Texture) {
	GM_BIND_TYPEID(Texture);
	GM_BIND_DECL(Texture);
}

// COMPUTE BUFFER
GM_REG_NAMESPACE(ComputeBuffer) {
	GM_BIND_TYPEID(ComputeBuffer);
	GM_BIND_DECL(ComputeBuffer);
}
