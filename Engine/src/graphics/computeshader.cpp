#include "pch.h"
#include "computeshader.h"

#include "shadermanager.h"
#include "renderstate.h"

namespace prev {

	ComputeShader::ComputeShader(const std::string & shaderName) : m_ShaderName(shaderName) {
		ShaderManager::Ref().RegisterComputeShader(this, shaderName);
		LOG_TRACE("Compute Shader : {} successfully loaded", shaderName);
	}

	ComputeShader::~ComputeShader() {
		LOG_TRACE("Compute Shader : {} successfully Released", m_ShaderName);
	}

	void ComputeShader::Bind() {
		ShaderManager::Ref().m_BoundComputeShader = this;
		for (auto uniform : m_ShaderUniforms) {
			uniform.second->Bind();
		}
		ShaderBind();
	}

	void ComputeShader::SetUniform(std::string uniformName, void * data, unsigned int dataSize) {
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
		uniform->Init(data, dataSize, uniformLocation, COMPUTE_SHADER);

		m_ShaderUniforms.insert(std::make_pair(uniformLocation, uniform));
	}

}