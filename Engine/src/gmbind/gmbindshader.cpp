#include "pch.h"
#include "gmbindshader.h"

#include "gm/gmMachine.h"
#include "gm/gmThread.h"

using namespace prev;

GM_REG_NAMESPACE(VertexShader) {
	GM_MEMFUNC_DECL(CreateVertexShader) {
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM(name, 0);
		StrongHandle<VertexShader> vShader = VertexShader::CreateVertexShader(name);
		//GM_PUSH_USER_HANDLED(VertexShader, vShader.Get());
		{ VertexShader * _ptr = (vShader.Get()); _ptr->AddRef(); GM_THREAD_ARG->PushNewUser(_ptr, GM_TYPEID(VertexShader)); }
		return GM_OK;
	}

	GM_MEMFUNC_DECL(Init) {
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM(code, 0);

		GM_GET_THIS_PTR(VertexShader, ptr);
		ptr->Init(code);
		return GM_OK;
	}

}

GM_REG_MEM_BEGIN(VertexShader)
GM_REG_HANDLED_DESTRUCTOR(VertexShader)
GM_REG_MEMFUNC(VertexShader, Init)
GM_REG_MEM_END(VertexShader)
GM_BIND_DEFINE(VertexShader)