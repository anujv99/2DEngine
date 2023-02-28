#include "pch.h"
#include "gmbindgraphics.h"

#include "gmbindgame.h"
#include "gmbindfuncgen.h"

#include "gm/gmMachine.h"
#include "gm/gmThread.h"

#include "renderer/renderer.h"

#include "graphics/font.h"
#include "graphics/shadermanager.h"
#include "graphics/computebuffer.h"

#include "game/label.h"

#include "graphics/texture2d.h"

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

// TEXTURE ----------------------------------------------------------------

using Texture = Texture2D;

GM_REG_NAMESPACE(Texture) {

	GM_MEMFUNC_DECL(CreateTexture) {
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM(fileName, 0);
		StrongHandle<Texture2D> texture = Texture2D::CreateTexture2D();
		texture->Init(fileName);
		GM_PUSH_USER_HANDLED(Texture, texture);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(GetSize) {
		GM_CHECK_NUM_PARAMS(0);
		GM_GET_THIS_PTR(Texture, tex);
		a_thread->PushVec2(ToVec2(tex->GetDesc().TextureSize));
		return GM_OK;
	}

}

GM_REG_MEM_BEGIN(Texture)
GM_REG_HANDLED_DESTRUCTOR(Texture)
GM_REG_MEMFUNC(Texture, GetSize)
GM_REG_MEM_END(Texture)
GM_BIND_DEFINE(Texture)

// COMPUTE BUFFER ---------------------------------------------------------

GM_REG_NAMESPACE(ComputeBuffer) {
	
	GM_MEMFUNC_DECL(CreateComputeBuffer) {
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_INT_PARAM(size, 0);
		GM_CHECK_INT_PARAM(bytes, 1);
		StrongHandle<ComputeBuffer> buffer = ComputeBuffer::CreateComputeBuffer();
		buffer->Init(nullptr, size, bytes);
		GM_PUSH_USER_HANDLED(ComputeBuffer, buffer);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(Bind) {
		GM_CHECK_NUM_PARAMS(0);
		GM_GET_THIS_PTR(ComputeBuffer, buffer);
		buffer->Bind();
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(UnBind) {
		GM_CHECK_NUM_PARAMS(0);
		GM_GET_THIS_PTR(ComputeBuffer, buffer);
		buffer->UnBind();
		return GM_OK;
	}

	GM_MEMFUNC_DECL(BindToPixelShader) {
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM(slot, 0);
		GM_GET_THIS_PTR(ComputeBuffer, buffer);
		buffer->BindToPixelShader(slot);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(UnBindFromPixelShader) {
		GM_CHECK_NUM_PARAMS(0);
		GM_GET_THIS_PTR(ComputeBuffer, buffer);
		buffer->UnBindFromPixelShader();
		return GM_OK;
	}

	GM_MEMFUNC_DECL(SetBindSlot) {
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM(slot, 0);
		GM_GET_THIS_PTR(ComputeBuffer, buffer);
		buffer->SetBindSlot(slot);
		return GM_OK;
	}
}

GM_REG_MEM_BEGIN(ComputeBuffer)
GM_REG_HANDLED_DESTRUCTOR(ComputeBuffer)
GM_REG_MEMFUNC(ComputeBuffer, Bind)
GM_REG_MEMFUNC(ComputeBuffer, UnBind)
GM_REG_MEMFUNC(ComputeBuffer, BindToPixelShader)
GM_REG_MEMFUNC(ComputeBuffer, UnBindFromPixelShader)
GM_REG_MEMFUNC(ComputeBuffer, SetBindSlot)
GM_REG_MEM_END(ComputeBuffer)
GM_BIND_DEFINE(ComputeBuffer)
