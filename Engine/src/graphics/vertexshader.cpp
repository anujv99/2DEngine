#include "pch.h"
#include "vertexshader.h"

#include "shadermanager.h"

namespace prev {

	VertexShader::VertexShader(const std::string & shaderName) : m_ShaderName(shaderName) {
		ShaderManager::Ref().RegisterVertexShader(this, shaderName);
	}

}