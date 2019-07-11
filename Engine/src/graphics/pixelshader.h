#pragma once

#include "pixelshader.h"
#include "bindable.h"

namespace prev {

	class PixelShader : public HandledObject<PixelShader>, public Bindable {
	protected:
		PixelShader(const std::string & shaderName);
	public:
		virtual ~PixelShader() {}

		virtual void Bind() override;
		virtual void UnBind() override = 0;
		virtual void Init(const std::string & shaderCode) = 0;
		virtual int GetUniformLocation(const std::string & uniformName) = 0;

		virtual BindableType GetType() const override {
			return BindableType::PIXEL_SHADER;
		}

		const std::string & GetShaderName() const {
			return m_ShaderName;
		}

		static StrongHandle<PixelShader> CreatePixelShader(const std::string & shaderName);
	protected:
		virtual void ShaderBind() = 0;
	protected:
		std::string m_ShaderName;
	};

}
