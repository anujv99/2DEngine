#pragma once

#include "datatypes.h"
#include "bindable.h"

namespace prev {

	struct Sampler2DDesc {
		TextureWrapType Wrapping;
		TextureFilterType Filtering;
	};

	class Sampler2D : public HandledObject<Sampler2D>, public Bindable {
	protected:
		Sampler2D() {}
	public:
		~Sampler2D() {}

		virtual void Bind() override = 0;
		virtual void UnBind() override = 0;

		virtual void Init(const Sampler2DDesc samplerDesc) = 0;

		virtual BindableType GetType() const override {
			return BindableType::SAMPLER_2D;
		}
	public:
		static StrongHandle<Sampler2D> CreateSampler2D();
	};

}
