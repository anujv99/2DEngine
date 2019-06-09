#pragma once

#include "d3dhelper.h"

#include "graphics/pixelshader.h"

namespace prev {

	class D3DPixelShader : public PixelShader {
	public:
		D3DPixelShader(const std::string & shaderName) : PixelShader(shaderName), m_IsShaderCreated(false) {}

		// Inherited via VertexShader
		virtual void Bind() override;
		virtual void UnBind() override;
		virtual void Init(const std::string & shaderCode) override;
		virtual void Reload() override;
	private:
		bool CompileAndCreateShader(const std::string & shaderCode);
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
		Microsoft::WRL::ComPtr<ID3DBlob> m_ShaderBytecode;
	private:
		bool m_IsShaderCreated;
	};

}
