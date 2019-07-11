#include "pch.h"
#include "pixelshader.h"
#include "shadermanager.h"

namespace prev {

	PixelShader::PixelShader(const std::string & shaderName) : m_ShaderName(shaderName) {
		ShaderManager::Ref().RegisterPixelShader(this, shaderName);
		LOG_TRACE("Pixel Shader : {} successfully loaded", shaderName);
	}

	void PixelShader::Bind() {
		ShaderBind();
		ShaderManager::Ref().m_BoundPixelShader = this;
	}

}