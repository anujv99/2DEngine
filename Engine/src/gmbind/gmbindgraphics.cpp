#include "pch.h"
#include "gmbindgraphics.h"

#include "gmbindgame.h"
#include "gmbindfuncgen.h"

#include "gm/gmMachine.h"
#include "gm/gmThread.h"

#include "renderer/renderer.h"

#include "graphics/font.h"
#include "graphics/shadermanager.h"

#include "game/label.h"

#undef CreateFont

using namespace prev;

// FONT -------------------------------------------------------------------

GM_REG_NAMESPACE(Font) {

	GM_MEMFUNC_DECL(CreateFont) {
		GM_CHECK_NUM_PARAMS(3);
		GM_CHECK_STRING_PARAM(name, 0);
		GM_CHECK_STRING_PARAM(fileName, 1);
		GM_CHECK_FLOAT_OR_INT_PARAM(size, 2);
		StrongHandle<Font> font = new Font(name, fileName, size);
		GM_PUSH_USER_HANDLED(Font, font);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(Print) {
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_USER_PARAM(Label *, GM_TYPEID(Label), label, 0);
		GM_GET_THIS_PTR(Font, font);
		Renderer::Ref().Submit(*label, font);
		return GM_OK;
	}

}

GM_REG_MEM_BEGIN(Font)
GM_REG_HANDLED_DESTRUCTOR(Font)
GM_REG_MEMFUNC(Font, Print)
GM_REG_MEM_END(Font)
GM_BIND_DEFINE(Font)

// VERTEX SHADER -----------------------------------------------------------

GM_REG_NAMESPACE(VertexShader) {

	GM_MEMFUNC_DECL(CreateVertexShader) {
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_STRING_PARAM(name, 0);
		GM_CHECK_STRING_PARAM(path, 1);
		StrongHandle<VertexShader> vShader = ShaderManager::Ref().LoadVertexShaderFromFile(name, path);
		GM_PUSH_USER_HANDLED(VertexShader, vShader);
		return GM_OK;
	}

}