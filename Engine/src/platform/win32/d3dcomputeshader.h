#pragma once

#include "d3dhelper.h"

#include "graphics/computeshader.h"

namespace prev {

	class D3DComputeShader : public ComputeShader {
	public:
		D3DComputeShader(const std::string & shaderName) : ComputeShader(shaderName), m_IsShaderCreated(false) {}

		// Inherited via VertexShader
		virtual void ShaderBind() override;
		virtual void UnBind() override;
		virtual void Init(const std::string & shaderCode) override;
		virtual void Dispatch(unsigned int x, unsigned int y, unsigned int z) override;
		virtual int GetUniformLocation(const std::string & uniformName) override;

		inline Microsoft::WRL::ComPtr<ID3DBlob> GetShaderBytecode() { return m_ShaderBytecode; }
	private:
		bool CompileAndCreateShader(const std::string & shaderCode);
		void GetShaderReflection(const std::string & shaderName);
	private:
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_ComputeShader;
		Microsoft::WRL::ComPtr<ID3DBlob> m_ShaderBytecode;
		Microsoft::WRL::ComPtr<ID3D11ShaderReflection> m_ShaderInfo;
	private:
		bool m_IsShaderCreated;
	};

}
