#include "pch.h"
#include "shadermanager.h"

namespace prev {

	ShaderManager::ShaderManager() {
		m_BoundVertexShader = nullptr;
		m_BoundPixelShader = nullptr;
		LOG_TRACE("Shader Manager Initialized");
	}

	ShaderManager::~ShaderManager() {
		m_VertexShaders.clear();
		m_PixelShaders.clear();
	}

	StrongHandle<VertexShader> ShaderManager::GetBoundVertexShader() {
		return m_BoundVertexShader;
	}

	StrongHandle<PixelShader> ShaderManager::GetBoundPixelShader() {
		return m_BoundPixelShader;
	}

	StrongHandle<VertexShader> ShaderManager::GetVertexShader(const std::string & shaderName) {
		uint32_t shaderHash = HashString(shaderName);

		auto it = m_VertexShaders.find(shaderHash);
		if (it != m_VertexShaders.end()) {
			return it->second;
		} else {
			ERROR_TRACE(ERR_SHADER_NOT_FOUND, "Vertex shader with the name : " + shaderName + " Not found");
			ASSERT(false);
			return nullptr;
		}

	}

	StrongHandle<PixelShader> ShaderManager::GetPixelShader(const std::string & shaderName) {
		uint32_t shaderHash = HashString(shaderName);

		auto it = m_PixelShaders.find(shaderHash);
		if (it != m_PixelShaders.end()) {
			return it->second;
		} else {
			ERROR_TRACE(ERR_SHADER_NOT_FOUND, "Pixel shader with the name : " + shaderName + " Not found");
			ASSERT(false);
			return nullptr;
		}

	}

	StrongHandle<VertexShader> ShaderManager::LoadVertexShaderFromFile(const std::string & shaderName, const std::string & filePath) {
		std::string vShaderSource = ReadFile(filePath);
		ASSERT(vShaderSource.size() > 0);
		StrongHandle<VertexShader> vShader = VertexShader::CreateVertexShader(shaderName);
		vShader->Init(vShaderSource);
		return vShader;
	}

	StrongHandle<PixelShader> ShaderManager::LoadPixelShaderFromFile(const std::string & shaderName, const std::string & filePath) {
		std::string pShaderSource = ReadFile(filePath);
		ASSERT(pShaderSource.size() > 0);
		StrongHandle<PixelShader> pShader = PixelShader::CreatePixelShader(shaderName);
		pShader->Init(pShaderSource);
		return pShader;
	}

	void ShaderManager::RegisterVertexShader(StrongHandle<VertexShader> vertexShader, const std::string & shaderName) {
		ASSERT(vertexShader != nullptr);
		ASSERT(shaderName.size() > 0);

		uint32_t shaderhash = HashString(shaderName);
		if (m_VertexShaders.find(shaderhash) != m_VertexShaders.end()) {
			ERROR_TRACE(ERR_SHADER_ALREADY_PRESENT, "Vertex shader with the name : " + shaderName + " already present in the map");
			ASSERT(false);
			return;
		} else {
			m_VertexShaders.insert(std::make_pair(shaderhash, vertexShader));
			return;
		}
	}

	void ShaderManager::ReleaseVertexShader(const std::string & shaderName) {
		uint32_t shaderhash = HashString(shaderName);

		auto it = m_VertexShaders.find(shaderhash);
		if (it != m_VertexShaders.end()) {
			m_VertexShaders.erase(it);
			return;
		} else {
			ERROR_TRACE(ERR_SHADER_NOT_FOUND, "Vertex shader with the name : " + shaderName + " not found");
			ASSERT(false);
			return;
		}
	}

	void ShaderManager::RegisterPixelShader(StrongHandle<PixelShader> pixelShader , const std::string & shaderName) {
		ASSERT(pixelShader != nullptr);
		ASSERT(shaderName.size() > 0);

		uint32_t shaderhash = HashString(shaderName);
		if (m_PixelShaders.find(shaderhash) != m_PixelShaders.end()) {
			ERROR_TRACE(ERR_SHADER_ALREADY_PRESENT, "Pixel shader with the name : " + shaderName + " already present in the map");
			ASSERT(false);
			return;
		} else {
			m_PixelShaders.insert(std::make_pair(shaderhash, pixelShader));
			return;
		}
	}

	void ShaderManager::ReleasePixelShader(const std::string & shaderName) {
		uint32_t shaderhash = HashString(shaderName);

		auto it = m_PixelShaders.find(shaderhash);
		if (it != m_PixelShaders.end()) {
			m_PixelShaders.erase(it);
			return;
		} else {
			ERROR_TRACE(ERR_SHADER_NOT_FOUND, "Pixel shader with the name : " + shaderName + " not found");
			ASSERT(false);
			return;
		}
	}

	std::string ShaderManager::ReadFile(const std::string & fileName) {
		std::stringstream shaderCode;
		std::ifstream inFile;

		inFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			inFile.open(fileName);
			shaderCode << inFile.rdbuf();
			inFile.close();
		} catch (std::ifstream::failure &) {
			ERROR_TRACE(ERR_FILE_NOT_FOUND, "Shader file with name : " + fileName + " not found");
			auto shaderString = shaderCode.str();
			shaderString.clear();
			return shaderString;
		}

		return shaderCode.str();
	}

}