#pragma once

#include "datatypes.h"
#include "bindable.h"

#include "math/vec2.h"

namespace prev {

	struct Texture2DDesc {
		Texture2DDesc() : TextureSize(Vec2i(0)), TexFormat(PV_TEXTURE_FORMAT_RGBA8) {}
		Vec2i TextureSize;
		TextureFormat TexFormat;
	};

	class Texture2D : public HandledObject<Texture2D>, public Bindable {
	protected:
		Texture2D() : m_TextureDesc(Texture2DDesc()) {}
	public:
		const Texture2DDesc & GetDesc() const { return m_TextureDesc; }
	public:
		virtual ~Texture2D() {}

		virtual void Init(const std::string & fileName, unsigned int bindSlot) = 0;
		virtual void Init(const Texture2DDesc desc, unsigned int bindSlot) = 0;

		virtual void Bind() override = 0;
		virtual void UnBind() override = 0;

		virtual BindableType GetType() const override {
			return BindableType::TEXTURE_2D;
		}
	protected:
		Texture2DDesc m_TextureDesc;
	public:
		static StrongHandle<Texture2D> CreateTexture2D();
	};

}
