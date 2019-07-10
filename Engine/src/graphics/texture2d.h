#pragma once

#include "datatypes.h"
#include "bindable.h"

#include "math/vec2.h"

namespace prev {

	//Used for empty textures
	struct Texture2DDesc {
		Texture2DDesc() : TextureSize(Vec2i(0)), TexFormat(PV_TEXTURE_FORMAT_RGBA8) {}
		Vec2i TextureSize;
		TextureFormat TexFormat;
	};

	struct TextureParams {
		TextureWrapType Wrapping = PV_TEXTURE_WRAP_DEFAULT;
		TextureFilterType Filtering = PV_TEXTURE_FILTER_DEFAULT;
	};

	class Texture2D : public HandledObject<Texture2D>, public Bindable {
	protected:
		Texture2D() : m_TextureDesc(Texture2DDesc()) {}
	public:
		const Texture2DDesc & GetDesc() const { return m_TextureDesc; }
	public:
		virtual ~Texture2D() {}

		virtual void Init(const std::string & fileName, TextureParams texParams = TextureParams(), unsigned int texSlot = 0) = 0;
		virtual void Init(const Texture2DDesc desc, TextureParams texParams = TextureParams(), unsigned int texSlot = 0) = 0;
		virtual void SetTextureSlot(unsigned int texSlot) = 0;
		virtual void SetTextureParams(TextureParams texParams) = 0;

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
