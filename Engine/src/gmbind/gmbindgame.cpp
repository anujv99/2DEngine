#include "pch.h"
#include "gmbindgame.h"

#include "gm/gmMachine.h"
#include "gm/gmThread.h"
#include "gm/gmVariable.h"

#include "game/sprite.h"
#include "renderer/renderer.h"

#include "gmbindfuncgen.h"

using namespace prev;

GM_REG_NAMESPACE(Sprite) {

	GM_MEMFUNC_DECL(CreateSprite) {
		GM_CHECK_NUM_PARAMS(0);
		GM_PUSH_USER(Sprite, new Sprite());
		return GM_OK;
	}

	GM_MEMFUNC_DECL(Draw) {
		GM_CHECK_NUM_PARAMS(0);
		GM_GET_THIS_PTR(Sprite, ptr);
		Renderer::Ref().Submit(*ptr);
		return GM_OK;
	}

	GM_SETDOT_FUNC_BEGIN(Sprite)
		GM_SETDOT_PARAM_VEC2("Position", Position)
		GM_SETDOT_PARAM_VEC2("Dimension", Dimension)
		GM_SETDOT_PARAM_FLOAT("Rotation", Rotation)
		GM_SETDOT_PARAM_VEC4("Color", Color)
	GM_SETDOT_FUNC_END(Sprite)

	GM_GETDOT_FUNC_BEGIN(Sprite)
		GM_GETDOT_PARAM_VEC2("Position", Position)
		GM_GETDOT_PARAM_VEC2("Dimension", Dimension)
		GM_GETDOT_PARAM_FLOAT("Rotation", Rotation)
		GM_GETDOT_PARAM_VEC4("Color", Color.ToVec4())
	GM_GETDOT_FUNC_END(Sprite)
}

GM_REG_MEM_BEGIN(Sprite)
	GM_REG_DESTRUCTOR(Sprite)
	GM_REG_MEMFUNC(Sprite, Draw)
	GM_REG_SET_GET_DOT_FUNC(Sprite)
GM_REG_MEM_END(Sprite)

GM_BIND_DEFINE(Sprite)
