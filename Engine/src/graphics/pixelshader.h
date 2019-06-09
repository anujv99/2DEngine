#pragma once

#include "pixelshader.h"
#include "bindable.h"

namespace prev {

	class PixelShader : public HandledObject<PixelShader>, public Bindable {
	protected:
		PixelShader(const std::string & shaderName);
	public:
		virtual ~PixelShader() {}

		virtual void Bind() override = 0;
		virtual void UnBind() override = 0;
		virtual void Init(const std::string & shaderCode) = 0;
		virtual void Reload() = 0;

		virtual BindableType GetType() const override {
			return BindableType::PIXEL_SHADER;
		}

		const std::string & GetShaderName() const {
			return m_ShaderName;
		}

		static StrongHandle<PixelShader> CreatePixelShader(const std::string & shaderName);
	private:
		std::string m_ShaderName;
	};

}
