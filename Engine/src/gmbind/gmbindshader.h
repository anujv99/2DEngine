#pragma once

#include "gmbind.h"

GM_REG_NAMESPACE(VertexShader) {
	GM_BIND_TYPEID(VertexShader);
	GM_BIND_DECL(VertexShader);
}

GM_REG_NAMESPACE(PixelShader) {
	GM_BIND_TYPEID(PixelShader);
	GM_BIND_DECL(PixelShader);
}

// No Type ID because ShaderManager is not a type. It is just a container for few functions
void gmBindShaderManagerLib(gmMachine * a_machine);