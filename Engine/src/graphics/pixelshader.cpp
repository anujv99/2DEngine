#include "pch.h"
#include "pixelshader.h"
#include "shadermanager.h"

namespace prev {

	PixelShader::PixelShader(const std::string & shaderName) : m_ShaderName(shaderName) {
		ShaderManager::Ref().RegisterPixelShader(this, shaderName);
	}

}