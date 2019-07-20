#include "pch.h"
#include "vertexshader.h"

#include "shadermanager.h"
#include "renderstate.h"

namespace prev {

	VertexShader::VertexShader(const std::string & shaderName) : m_ShaderName(shaderName) {
		ShaderManager::Ref().RegisterVertexShader(this, shaderName);
		LOG_TRACE("Vertex Shader : {} successfully loaded", shaderName);
	}

	VertexShader::~VertexShader() {
		LOG_TRACE("Vertex Shader : {} successfully Released", m_ShaderName);
	}

	void VertexShader::Bind() {
		ShaderManager::Ref().m_BoundVertexShader = this;
		ShaderBind();
	}

	void VertexShader::UpdateMVP() {
		if (MVP::Ref().IsDirty()) {
			RenderState::Ref().GetMVPUniform()->Update(&MVP::Ref().Get(), sizeof(Mat4));
		}
	}

	void VertexShader::SetUniform(std::string uniformName, void * data, unsigned int dataSize) {
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
		uniform->Init(data, dataSize, uniformLocation, VERTEX_SHADER);

		m_ShaderUniforms.insert(std::make_pair(uniformLocation, uniform));
	}

}