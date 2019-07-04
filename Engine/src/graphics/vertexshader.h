#pragma once

#include "bindable.h"

namespace prev {

	class VertexShader : public HandledObject<VertexShader>, public Bindable {
	protected:
		VertexShader(const std::string & shaderName);
	public:
		static const unsigned int MVP_RESERVED_SLOT = 0;

		virtual ~VertexShader() {}

		virtual void Bind() override = 0;
		virtual void UnBind() override = 0;
		virtual void Init(const std::string & shaderCode) = 0;
		virtual int GetUniformLocation(const std::string & uniformName) = 0;

		virtual BindableType GetType() const override {
			return BindableType::VERTEX_SHADER;
		}

		const std::string & GetShaderName() const {
			return m_ShaderName;
		}

		void UpdateMVP();

		static StrongHandle<VertexShader> CreateVertexShader(const std::string & shaderName);
	protected:
		std::string m_ShaderName;
	};

}
