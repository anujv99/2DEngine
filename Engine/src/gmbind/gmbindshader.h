#pragma once

#include "gmbind.h"

#include "graphics/vertexshader.h"

GM_REG_NAMESPACE(VertexShader) {
	GM_BIND_TYPEID(VertexShader);
	GM_BIND_DECL(VertexShader);
}

namespace gmfVertexShader {
	class gmUserType {
	public:
		static int s_gmUserTypeId;
	};
}