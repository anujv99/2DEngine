#pragma once

#include "bindable.h"
#include "texture2d.h"

#include "math/vec2.h"
#include "math/vec4.h"

#define BIT(x) (1 << x)

namespace prev {

	enum FramebufferFlags {
		FRAMEBUFFER_ENABLE_MSAA				= BIT(0),
		FRAMEBUFFER_ALLOCATE_DEPTH_BUFFER	= BIT(1),
		FRAMEBUFFER_MSAA_16x				= BIT(2),
		FRAMEBUFFER_MSAA_8x					= BIT(3),
		FRAMEBUFFER_MSAA_4x					= BIT(4),
		FRAMEBUFFER_MSAA_2x					= BIT(5),
		FRAMEBUFFER_DISABLE_MSAA			= BIT(6),
	};

	class Framebuffer : public HandledObject<Framebuffer>, public Bindable {
	protected:
		Framebuffer() {}
	public:
		virtual ~Framebuffer() {}

		virtual void Bind() override = 0;
		virtual void UnBind() override = 0;

		//By default MSAA is determined by the window,
		//And no depth buffer is created
		virtual void Init(Vec2 size, TextureFormat format, unsigned int flags) = 0;
		virtual void Clear() = 0;
		virtual void Clear(Vec4 color) = 0;
		virtual Vec2 GetSize() = 0;
		virtual StrongHandle<Texture2D> GetTexture() = 0;
		virtual StrongHandle<Texture2D> GetDepthTexture() = 0;

		virtual BindableType GetType() const override {
			return FRAMEBUFFER;
		}

		static StrongHandle<Framebuffer> CreateFramebuffer();
	};

}
