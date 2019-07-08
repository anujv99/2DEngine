#include "pch.h"
#include "gmbindgame.h"

#include "gm/gmMachine.h"
#include "gm/gmThread.h"
#include "gm/gmVariable.h"

#include "game/sprite.h"
#include "renderer/spriterenderer.h"

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
		SpriteRenderer::Ref().Submit(*ptr, 0);
		return GM_OK;
	}

	GM_SETDOT_FUNC_BEGIN(Sprite)
		GM_SETDOT_PARAM_CUSTOM("Position", GM_OP_VEC2(result, 1); ptr->SetPosition(result))
		GM_SETDOT_PARAM_CUSTOM("Dimension", GM_OP_VEC2(result, 1); ptr->SetDimension(result))
		GM_SETDOT_PARAM_CUSTOM("Rotation", GM_OP_FLOAT(result, 1); ptr->SetRotation(result))
		GM_SETDOT_PARAM_CUSTOM("Color", GM_OP_VEC4(result, 1); ptr->SetColor(Vec4(result)))
	GM_SETDOT_FUNC_END(Sprite)

	GM_GETDOT_FUNC_BEGIN(Sprite)
		GM_GETDOT_PARAM_CUSTOM("Position", a_operands[0].SetVec2(ptr->GetPosition()))
		GM_GETDOT_PARAM_CUSTOM("Dimension", a_operands[0].SetVec2(ptr->GetDimension()))
		GM_GETDOT_PARAM_CUSTOM("Rotation", a_operands[0].SetFloat(ptr->GetRotation()))
		GM_GETDOT_PARAM_CUSTOM("Color", a_operands[0].SetVec4(ptr->GetColor().ToVec4()))
	GM_GETDOT_FUNC_END(Sprite)
}

GM_REG_MEM_BEGIN(Sprite)
	GM_REG_DESTRUCTOR(Sprite)
	GM_REG_MEMFUNC(Sprite, Draw)
	GM_REG_SET_GET_DOT_FUNC(Sprite)
GM_REG_MEM_END(Sprite)

GM_BIND_DEFINE(Sprite)
