#include "pch.h"
#include "vertexshader.h"

#include "shadermanager.h"

#include "math/mvp.h"

namespace prev {

	VertexShader::VertexShader(const std::string & shaderName) : m_ShaderName(shaderName) {
		ShaderManager::Ref().RegisterVertexShader(this, shaderName);

		m_MVPUniform = Uniform::CreateUniform();
		m_MVPUniform->Init(&Identity(), sizeof(Mat4), MVP_RESERVED_SLOT, VERTEX_SHADER);
		m_MVPUniform->Bind();
	}

	void VertexShader::UpdateMVP() {
		if (MVP::Ref().IsDirty()) {
			m_MVPUniform->Update(&MVP::Ref().Get(), sizeof(Mat4));
			m_MVPUniform->Bind();
		}
	}

}