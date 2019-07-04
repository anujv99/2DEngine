#include "pch.h"
#include "d3dvertexshader.h"

#define VERTEX_SHADER_MODEL "vs_5_0"

namespace prev {

	StrongHandle<VertexShader> VertexShader::CreateVertexShader(const std::string & shaderName) {
		return new D3DVertexShader(shaderName);
	}

	void D3DVertexShader::Bind() {
		LOG_ON_CONDITION(m_IsShaderCreated, LOG_ERROR, "Binding uninitialized vertex shader", return);
		GetDeviceContext()->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	}

	void D3DVertexShader::UnBind() {
	}

	void D3DVertexShader::Init(const std::string & shaderCode) {
		LOG_ON_CONDITION(!m_IsShaderCreated, LOG_ERROR, "Vertex Shader already initialized", return);
		m_IsShaderCreated = CompileAndCreateShader(shaderCode);
		GetShaderReflection(m_ShaderName);
	}

	int D3DVertexShader::GetUniformLocation(const std::string & uniformName) {
		D3D11_SHADER_INPUT_BIND_DESC ibd;
		ZeroMemory(&ibd, sizeof(ibd));
		HRESULT hr = m_ShaderInfo->GetResourceBindingDescByName(uniformName.c_str(), &ibd);
		if (FAILED(hr)) {
			return -1;
		}
		return ibd.BindPoint;
	}

	bool D3DVertexShader::CompileAndCreateShader(const std::string & shaderCode) {
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
			VERTEX_SHADER_MODEL,
			flags,
			0u,
			m_ShaderBytecode.GetAddressOf(),
			errorMessages.GetAddressOf()
		);

		if (FAILED(hr)) {
			std::string errorMessage = (const char *)errorMessages->GetBufferPointer();

			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to compile vertex shader : \n" + errorMessage);
			return false;
		}

		hr = GetDevice()->CreateVertexShader(
			m_ShaderBytecode->GetBufferPointer(),
			m_ShaderBytecode->GetBufferSize(),
			nullptr, m_VertexShader.GetAddressOf()
		);

		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to create vertex shader");
			return false;
		}

		return true;
	}

	void D3DVertexShader::GetShaderReflection(const std::string & shaderName) {
		HRESULT hr = D3DReflect(m_ShaderBytecode->GetBufferPointer(), m_ShaderBytecode->GetBufferSize(), 
			IID_ID3D11ShaderReflection, (void **)m_ShaderInfo.GetAddressOf());
		if (FAILED(hr)) {
			LOG_WARN("Unable to get shader reflection {}", shaderName);
		}
		return;
	}

}