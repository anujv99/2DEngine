#pragma once

#include "bindable.h"
#include "uniform.h"

namespace prev {

	class ComputeShader : public HandledObject<ComputeShader>, public Bindable {
	protected:
		ComputeShader(const std::string & shaderName);
	public:
		static const unsigned int MVP_RESERVED_SLOT = 0;

		virtual ~ComputeShader();

		virtual void Bind() override;
		virtual void UnBind() override = 0;
		virtual void Init(const std::string & shaderCode) = 0;
		virtual void Dispatch(unsigned int x, unsigned int y, unsigned int z) = 0;
		virtual int GetUniformLocation(const std::string & uniformName) = 0;

		virtual BindableType GetType() const override {
			return BindableType::COMPUTE_SHADER;
		}

		const std::string & GetShaderName() const {
			return m_ShaderName;
		}

		static StrongHandle<ComputeShader> CreateComputeShader(const std::string & shaderName);
	public:
		void SetUniform(std::string uniformName, void * data, unsigned int dataSize);
	protected:
		virtual void ShaderBind() = 0;
	protected:
		std::string m_ShaderName;

		std::unordered_map<unsigned int, StrongHandle<Uniform>> m_ShaderUniforms; // UniformLocation -> Uniform
	};

}
