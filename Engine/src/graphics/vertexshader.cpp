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

}