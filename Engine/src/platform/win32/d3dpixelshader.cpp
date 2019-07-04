#include "pch.h"
#include "d3dpixelshader.h"

#define PIXEL_SHADER_MODEL "ps_5_0"

namespace prev {

	StrongHandle<PixelShader> PixelShader::CreatePixelShader(const std::string & shaderName) {
		return new D3DPixelShader(shaderName);
	}

	void D3DPixelShader::Bind() {
		LOG_ON_CONDITION(m_IsShaderCreated, LOG_ERROR, "Binding uninitialized pixel shader", return);
		GetDeviceContext()->PSSetShader(m_PixelShader.Get(), nullptr, 0);
	}

	void D3DPixelShader::UnBind() {
		GetDeviceContext()->PSSetShader(nullptr, nullptr, 0);
	}

	void D3DPixelShader::Init(const std::string & shaderCode) {
		LOG_ON_CONDITION(!m_IsShaderCreated, LOG_ERROR, "Pixel Shader already initialized", return);
		m_IsShaderCreated = CompileAndCreateShader(shaderCode);
		GetShaderReflection(m_ShaderName);
	}

	int D3DPixelShader::GetUniformLocation(const std::string & uniformName) {
		D3D11_SHADER_INPUT_BIND_DESC ibd;
		ZeroMemory(&ibd, sizeof(ibd));
		HRESULT hr = m_ShaderInfo->GetResourceBindingDescByName(uniformName.c_str(), &ibd);
		if (FAILED(hr)) {
			return -1;
		}
		return ibd.BindPoint;
	}

	bool D3DPixelShader::CompileAndCreateShader(const std::string & shaderCode) {
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
			PIXEL_SHADER_MODEL,
			flags,
			0u,
			m_ShaderBytecode.GetAddressOf(),
			errorMessages.GetAddressOf()
		);

		if (FAILED(hr)) {
			std::string errorMessage = (const char *)errorMessages->GetBufferPointer();

			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to compile pixel shader : \n" + errorMessage);
			return false;
		}

		hr = GetDevice()->CreatePixelShader(
			m_ShaderBytecode->GetBufferPointer(),
			m_ShaderBytecode->GetBufferSize(),
			nullptr, m_PixelShader.GetAddressOf()
		);

		if (FAILED(hr)) {
			ERROR_TRACE(ERR_D3D11_INTERNAL_ERROR, "Unable to create pixel shader");
			return false;
		}

		return true;
	}

	void D3DPixelShader::GetShaderReflection(const std::string & shaderName) {
		HRESULT hr = D3DReflect(m_ShaderBytecode->GetBufferPointer(), m_ShaderBytecode->GetBufferSize(),
			IID_ID3D11ShaderReflection, (void **)m_ShaderInfo.GetAddressOf());
		if (FAILED(hr)) {
			LOG_WARN("Unable to get shader reflection {}", shaderName);
		}
		return;
	}

}