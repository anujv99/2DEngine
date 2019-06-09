#pragma once

#include "bindable.h"

namespace prev {

	class VertexShader : public HandledObject<VertexShader>, public Bindable {
	protected:
		VertexShader(const std::string & shaderName);
	public:
		virtual ~VertexShader() {}

		virtual void Bind() override = 0;
		virtual void UnBind() override = 0;

		virtual void Init(const std::string & shaderCode) = 0;

		virtual BindableType GetType() const override {
			return BindableType::VERTEX_SHADER;
		}

		const std::string & GetShaderName() const {
			return m_ShaderName;
		}

		static StrongHandle<VertexShader> CreateVertexShader(const std::string & shaderName);
	private:
		std::string m_ShaderName;
	};

}
