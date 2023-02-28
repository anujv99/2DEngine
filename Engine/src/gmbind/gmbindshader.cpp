#include "pch.h"
#include "gmbindshader.h"

#include "gm/gmMachine.h"
#include "gm/gmThread.h"

#include "gmbindfuncgen.h"

#include "graphics/vertexshader.h"
#include "graphics/pixelshader.h"
#include "graphics/computeshader.h"
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

	GM_MEMFUNC_DECL(SetUniform) {
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_STRING_PARAM(name, 0);
		int numParams = GM_NUM_PARAMS;
		std::vector<float> params;
		for (int i = 1; i < numParams; i++) {
			GM_CHECK_FLOAT_PARAM(p, i);
			params.push_back(p);
		}
		GM_GET_THIS_PTR(PixelShader, ptr);
		ptr->SetUniform(name, &params[0], sizeof(params[0]) * params.size());
		return GM_OK;
	}
}

GM_REG_MEM_BEGIN(PixelShader)
GM_REG_HANDLED_DESTRUCTOR(PixelShader)
GM_REG_MEMFUNC(PixelShader, Init)
GM_REG_MEMFUNC(PixelShader, Bind)
GM_REG_MEMFUNC(PixelShader, UnBind)
GM_REG_MEMFUNC(PixelShader, GetShaderName)
GM_REG_MEMFUNC(PixelShader, SetUniform)
GM_REG_MEM_END(PixelShader)
GM_BIND_DEFINE(PixelShader)

GM_REG_NAMESPACE(ComputeShader) {
	GM_MEMFUNC_DECL(CreateComputeShader) {
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM(name, 0);
		StrongHandle<ComputeShader> cShader = ComputeShader::CreateComputeShader(name);
		GM_PUSH_USER_HANDLED(ComputeShader, cShader.Get());
		return GM_OK;
	}

	GM_GEN_MEMFUNC_VOID_STRING(ComputeShader, Init)
	GM_GEN_MEMFUNC_VOID_VOID(ComputeShader, Bind)
	GM_GEN_MEMFUNC_VOID_VOID(ComputeShader, UnBind)
	GM_GEN_MEMFUNC_STRING_VOID(ComputeShader, GetShaderName)

	GM_MEMFUNC_DECL(Dispatch) {
		GM_CHECK_NUM_PARAMS(3);
		GM_CHECK_INT_PARAM(x, 0);
		GM_CHECK_INT_PARAM(y, 1);
		GM_CHECK_INT_PARAM(z, 2);
		GM_GET_THIS_PTR(ComputeShader, ptr);
		ptr->Dispatch(x, y, z);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(SetUniform) {
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_STRING_PARAM(name, 0);
		int numParams = GM_NUM_PARAMS;
		std::vector<float> params;
		for (int i = 1; i < numParams; i++) {
			GM_CHECK_FLOAT_PARAM(p, i);
			params.push_back(p);
		}
		GM_GET_THIS_PTR(ComputeShader, ptr);
		ptr->SetUniform(name, &params[0], sizeof(params[0]) * params.size());
		return GM_OK;
	}
}

GM_REG_MEM_BEGIN( ComputeShader )
GM_REG_HANDLED_DESTRUCTOR( ComputeShader )
GM_REG_MEMFUNC( ComputeShader, Init )
GM_REG_MEMFUNC( ComputeShader, Bind )
GM_REG_MEMFUNC( ComputeShader, UnBind )
GM_REG_MEMFUNC( ComputeShader, GetShaderName )
GM_REG_MEMFUNC( ComputeShader, Dispatch )
GM_REG_MEMFUNC( ComputeShader, SetUniform )
GM_REG_MEM_END( ComputeShader )
GM_BIND_DEFINE( ComputeShader )

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

	GM_MEMFUNC_DECL(LoadComputeShader) {
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_STRING_PARAM(name, 0);
		GM_CHECK_STRING_PARAM(path, 1);
		StrongHandle<ComputeShader> cShader = ShaderManager::Ref().LoadComputeShaderFromFile(name, path);
		GM_PUSH_USER_HANDLED(ComputeShader, cShader);
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

	GM_MEMFUNC_DECL(GetComputeShader) {
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM(name, 0);
		StrongHandle<ComputeShader> cShader = ShaderManager::Ref().GetComputeShader(name);
		GM_PUSH_USER_HANDLED(ComputeShader, cShader);
		return GM_OK;
	}
};

static gmFunctionEntry s_ShaderManagerLib[] = {
	GM_LIBFUNC_ENTRY(LoadVertexShader, ShaderManager)
	GM_LIBFUNC_ENTRY(LoadPixelShader, ShaderManager)
	GM_LIBFUNC_ENTRY(LoadComputeShader, ShaderManager)
	GM_LIBFUNC_ENTRY(GetVertexShader, ShaderManager)
	GM_LIBFUNC_ENTRY(GetPixelShader, ShaderManager)
	GM_LIBFUNC_ENTRY(GetComputeShader, ShaderManager)
};

void gmBindShaderManagerLib(gmMachine * a_machine) {
	a_machine->RegisterLibrary(s_ShaderManagerLib, GM_LIBFUNC_SIZE(s_ShaderManagerLib), "ShaderManager");
}