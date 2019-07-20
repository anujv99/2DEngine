#include "pch.h"
#include "pixelshader.h"
#include "shadermanager.h"

namespace prev {

	PixelShader::PixelShader(const std::string & shaderName) : m_ShaderName(shaderName) {
		ShaderManager::Ref().RegisterPixelShader(this, shaderName);
		LOG_TRACE("Pixel Shader : {} successfully loaded", shaderName);
	}

	void PixelShader::Bind() {
		if (ShaderManager::Ref().m_BoundPixelShader == (const PixelShader *)this) {
			return;
		}
		ShaderBind();
		for (auto & uniform : m_ShaderUniforms) {
			uniform.second->Bind();
		}
		ShaderManager::Ref().m_BoundPixelShader = this;
	}

	void PixelShader::SetUniform(std::string uniformName, void * data, unsigned int dataSize) {
		int uniformLocation = GetUniformLocation(uniformName);
		if (uniformLocation < 0) {
			LOG_WARN("Uniform with name : {}, not used", uniformName);
			return;
		}

		auto it = m_ShaderUniforms.find(uniformLocation);
		if (it != m_ShaderUniforms.end()) {
			it->second->Update(data, (size_t)dataSize);
			return;
		}

		StrongHandle<Uniform> uniform = Uniform::CreateUniform();
		uniform->Init(data, dataSize, uniformLocation, PIXEL_SHADER);

		m_ShaderUniforms.insert(std::make_pair(uniformLocation, uniform));
	}

}