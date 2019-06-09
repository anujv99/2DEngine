#pragma once

#include "bindable.h"

namespace prev {

	class Uniform : public HandledObject<Uniform>, public Bindable {
	protected:
		Uniform() {}
	public:
		virtual ~Uniform() {}

		virtual void Bind() override = 0;
		virtual void UnBind() override = 0;

		//Shader type used by DirectX
		//ID3D11DeviceContext::VSSetConstantBuffers
		//ID3D11DeviceContext::PSSetConstantBuffers
		virtual void Init(const void * data, unsigned int dataBytes, unsigned int uniformLocation, BindableType shaderType) = 0;
		virtual void Update(const void * data, size_t dataSize) = 0;
		//Used by DirectX
		virtual void ChangeBindingShader(BindableType shaderType) = 0;

		virtual BindableType GetType() const override {
			return BindableType::UNIFORM;
		}

		static StrongHandle<Uniform> CreateUniform();
	};

}
