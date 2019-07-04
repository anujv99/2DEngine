#pragma once

#include "d3dhelper.h"

#include "graphics/vertexshader.h"

namespace prev {

	class D3DVertexShader : public VertexShader {
	public:
		D3DVertexShader(const std::string & shaderName) : VertexShader(shaderName), m_IsShaderCreated(false) {}

		// Inherited via VertexShader
		virtual void Bind() override;
		virtual void UnBind() override;
		virtual void Init(const std::string & shaderCode) override;
		virtual int GetUniformLocation(const std::string & uniformName) override;

		inline Microsoft::WRL::ComPtr<ID3DBlob> GetShaderBytecode() { return m_ShaderBytecode; }
	private:
		bool CompileAndCreateShader(const std::string & shaderCode);
		void GetShaderReflection(const std::string & shaderName);
	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob> m_ShaderBytecode;
		Microsoft::WRL::ComPtr<ID3D11ShaderReflection> m_ShaderInfo;
	private:
		bool m_IsShaderCreated;
	};

}
