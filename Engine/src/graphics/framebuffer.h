#pragma once

#include "bindable.h"
#include "texture2d.h"

#include "math/vec2.h"

namespace prev {

	class Framebuffer : public HandledObject<Framebuffer>, public Bindable {
	protected:
		Framebuffer() {}
	public:
		virtual ~Framebuffer() {}

		virtual void Bind() override = 0;
		virtual void UnBind() override = 0;

		virtual void Init(Vec2 size) = 0;
		virtual void Clear() = 0;
		virtual Vec2 GetSize() = 0;
		virtual StrongHandle<Texture2D> GetTexture() = 0;

		virtual BindableType GetType() const override {
			return FRAMEBUFFER;
		}


		static StrongHandle<Framebuffer> CreateFramebuffer();
	};

}
