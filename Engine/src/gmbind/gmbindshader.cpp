#include "pch.h"
#include "gmbindshader.h"

#include "gm/gmMachine.h"
#include "gm/gmThread.h"

#include "gmbindfuncgen.h"

#include "graphics/vertexshader.h"
#include "graphics/pixelshader.h"
#include "graphics/shadermanager.h"

using namespace prev;

GM_REG_NAMESPACE(VertexShader) {
	GM_MEMFUNC_DECL(CreateVertexShader) {
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM(name, 0);
		StrongHandle<VertexShader> vShader = VertexShader::CreateVertexShader(name);
		GM_PUSH_USER_HANDLED(VertexShader, vShader.Get());
		return GM_OK;
	}

	GM_GEN_MEMFUNC_VOID_STRING(VertexShader, Init)
	GM_GEN_MEMFUNC_VOID_VOID(VertexShader, Bind)
	GM_GEN_MEMFUNC_VOID_VOID(VertexShader, UnBind)
	GM_GEN_MEMFUNC_STRING_VOID(VertexShader, GetShaderName)
}

GM_REG_MEM_BEGIN(VertexShader)
GM_REG_HANDLED_DESTRUCTOR(VertexShader)
GM_REG_MEMFUNC(VertexShader, Init)
GM_REG_MEMFUNC(VertexShader, Bind)
GM_REG_MEMFUNC(VertexShader, UnBind)
GM_REG_MEMFUNC(VertexShader, GetShaderName)
GM_REG_MEM_END(VertexShader)
GM_BIND_DEFINE(VertexShader)

GM_REG_NAMESPACE(PixelShader) {
	GM_MEMFUNC_DECL(CreatePixelShader) {
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM(name, 0);
		StrongHandle<PixelShader> pShader = PixelShader::CreatePixelShader(name);
		GM_PUSH_USER_HANDLED(PixelShader, pShader.Get());
		return GM_OK;
	}

	GM_GEN_MEMFUNC_VOID_STRING(PixelShader, Init)
	GM_GEN_MEMFUNC_VOID_VOID(PixelShader, Bind)
	GM_GEN_MEMFUNC_VOID_VOID(PixelShader, UnBind)
	GM_GEN_MEMFUNC_STRING_VOID(PixelShader, GetShaderName)
}

GM_REG_MEM_BEGIN(PixelShader)
GM_REG_HANDLED_DESTRUCTOR(PixelShader)
GM_REG_MEMFUNC(PixelShader, Init)
GM_REG_MEMFUNC(PixelShader, Bind)
GM_REG_MEMFUNC(PixelShader, UnBind)
GM_REG_MEMFUNC(PixelShader, GetShaderName)
GM_REG_MEM_END(PixelShader)
GM_BIND_DEFINE(PixelShader)

struct gmfShaderManagerLib {
	GM_MEMFUNC_DECL(LoadVertexShader) {
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_STRING_PARAM(name, 0);
		GM_CHECK_STRING_PARAM(path, 1);
		StrongHandle<VertexShader> vShader = ShaderManager::Ref().LoadVertexShaderFromFile(name, path);
		GM_PUSH_USER_HANDLED(VertexShader, vShader);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(LoadPixelShader) {
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_STRING_PARAM(name, 0);
		GM_CHECK_STRING_PARAM(path, 1);
		StrongHandle<PixelShader> pShader = ShaderManager::Ref().LoadPixelShaderFromFile(name, path);
		GM_PUSH_USER_HANDLED(PixelShader, pShader);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(GetVertexShader) {
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM(name, 0);
		StrongHandle<VertexShader> vShader = ShaderManager::Ref().GetVertexShader(name);
		GM_PUSH_USER_HANDLED(VertexShader, vShader);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(GetPixelShader) {
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM(name, 0);
		StrongHandle<PixelShader> pShader = ShaderManager::Ref().GetPixelShader(name);
		GM_PUSH_USER_HANDLED(PixelShader, pShader);
		return GM_OK;
	}
};

static gmFunctionEntry s_ShaderManagerLib[] = {
	GM_LIBFUNC_ENTRY(LoadVertexShader, ShaderManager)
	GM_LIBFUNC_ENTRY(LoadPixelShader, ShaderManager)
	GM_LIBFUNC_ENTRY(GetVertexShader, ShaderManager)
	GM_LIBFUNC_ENTRY(GetPixelShader, ShaderManager)
};

void gmBindShaderManagerLib(gmMachine * a_machine) {
	a_machine->RegisterLibrary(s_ShaderManagerLib, GM_LIBFUNC_SIZE(s_ShaderManagerLib), "ShaderManager");
}