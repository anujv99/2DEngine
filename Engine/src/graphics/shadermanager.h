#pragma once

#include "vertexshader.h"
#include "pixelshader.h"

namespace prev {

	class ShaderManager : public Singleton<ShaderManager> {
		friend class Singleton<ShaderManager>;
	public:
		StrongHandle<VertexShader> GetBoundVertexShader();
		StrongHandle<PixelShader> GetBoundPixelShader();

		StrongHandle<VertexShader> GetVertexShader(const std::string & shaderName);
		StrongHandle<PixelShader> GetPixelShader(const std::string & shaderName);

		StrongHandle<VertexShader> LoadVertexShaderFromFile(const std::string & shaderName, const std::string & filePath);
		StrongHandle<PixelShader> LoadPixelShaderFromFile(const std::string & shaderName, const std::string & filePath);
	private:
		ShaderManager();
		~ShaderManager();

		//VertexShader
		friend class VertexShader;
		void RegisterVertexShader(StrongHandle<VertexShader> vertexShader, const std::string & shaderName);
		void ReleaseVertexShader(const std::string & shaderName);

		//PixelShader
		friend class PixelShader;
		void RegisterPixelShader(StrongHandle<PixelShader> pixelShader, const std::string & shaderName);
		void ReleasePixelShader(const std::string & shaderName);

		std::string ReadFile(const std::string & fileName);
	private:
		// map of hash(fileName) -> pair(shader, fileName)
		std::unordered_map<uint32_t, StrongHandle<VertexShader>> m_VertexShaders;
		std::unordered_map<uint32_t, StrongHandle<PixelShader>> m_PixelShaders;

		StrongHandle<VertexShader> m_BoundVertexShader;
		StrongHandle<PixelShader> m_BoundPixelShader;
	};

}
