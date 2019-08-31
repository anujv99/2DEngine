#include "pch.h"
#include "d3dcomputeshader.h"

#define COMPUTE_SHADER_MODEL "cs_5_0"

namespace prev {

	StrongHandle<ComputeShader> ComputeShader::CreateComputeShader(const std::string & shaderName) {
		return new D3DComputeShader(shaderName);
	}

	void D3DComputeShader::ShaderBind() {
		LOG_ON_CONDITION(m_IsShaderCreated, LOG_ERROR, "Binding uninitialized vertex shader", return);
		GetDeviceContext()->CSSetShader(m_ComputeShader.Get(), nullptr, 0);
	}

	void D3DComputeShader::UnBind() {}

	void D3DComputeShader::Init(const std::string & shaderCode) {
		LOG_ON_CONDITION(!m_IsShaderCreated, LOG_ERROR, "Compute Shader already initialized", return);
		m_IsShaderCreated = CompileAndCreateShader(shaderCode);
		GetShaderReflection(m_ShaderName);
	}

	void D3DComputeShader::Dispatch(unsigned int x, unsigned int y, unsigned int z) {
		GetDeviceContext()->Dispatch(x, y, z);
	}

	int D3DComputeShader::GetUniformLocation(const std::string & uniformName) {
		D3D11_SHADER_INPUT_BIND_DESC ibd;
		ZeroMemory(&ibd, sizeof(ibd));
		HRESULT hr = m_ShaderInfo->GetResourceBindingDescByName(uniformName.c_str(), &ibd);
		if (FAILED(hr)) {
			return -1;
		}
		return ibd.BindPoint;
	}

	bool D3DComputeShader::CompileAndCreateShader(const std::string & shaderCode) {
		UINT flags = 0;
#ifdef ENGINE_DEBUG
		flags |= D3DCOMPILE_DEBUG;
#endif
		Microsoft::WRL::ComPtr<ID3DBlob> errorMessages;

		HRESULT hr = D3DCompile(
			shaderCode.c_str(),
			shaderCode.length(),
			nullptr,
			nullptr,
			nullptr,
			"main",
			COMPUTE_SHADER_MODEL,
			flags,
			0u,
			m_ShaderBytecode.GetAddressOf(),
			errorMessages.GetAddressOf()
		);

		if (FAILED(hr)) {
			std::string errorMessage = (const char *)errorMessages->GetBufferPointer();

			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to compile compute shader : \n" + errorMessage);
			return false;
		}

		hr = GetDevice()->CreateComputeShader(
			m_ShaderBytecode->GetBufferPointer(),
			m_ShaderBytecode->GetBufferSize(),
			nullptr, m_ComputeShader.GetAddressOf()
		);

		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to create compute shader");
			return false;
		}

		return true;
	}

	void D3DComputeShader::GetShaderReflection(const std::string & shaderName) {
		HRESULT hr = D3DReflect(m_ShaderBytecode->GetBufferPointer(), m_ShaderBytecode->GetBufferSize(), 
			IID_ID3D11ShaderReflection, (void **)m_ShaderInfo.GetAddressOf());
		if (FAILED(hr)) {
			LOG_WARN("Unable to get shader reflection {}", shaderName);
		}
		return;
	}

}