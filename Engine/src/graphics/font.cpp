#include "pch.h"
#include "font.h"

#include <freetype-gl.h>
#include "application.h"

#include "math/screenspace.h"

namespace prev {

	static constexpr const unsigned int TEXTURE_SIZE = 1024;

	Font::Font(const std::string & name, const std::string & fileName, float size) :
		m_FontName(name), m_FontFile(fileName), m_Size(size), m_Texture(nullptr), m_FontAtlas(nullptr), m_Font(nullptr) {

		size = ScreenToPixels(Vec2(size)).y;
		m_Size = size;

		m_FontAtlas = ftgl::texture_atlas_new(TEXTURE_SIZE, TEXTURE_SIZE, 1);
		m_Font = ftgl::texture_font_new_from_file(m_FontAtlas, size, fileName.c_str());

		m_Texture = Texture2D::CreateTexture2D();
		Texture2DDesc desc;
		desc.TexFormat = PV_TEXTURE_FORMAT_RGBA8;
		desc.TextureSize = Vec2i(TEXTURE_SIZE, TEXTURE_SIZE);

		TextureParams params;
		params.Filtering = PV_TEXTURE_FILTER_LINEAR;
		params.Wrapping = PV_TEXTURE_WRAP_CLAMP;

		m_Texture->Init(desc, params, 0);
		m_Texture->SetData(m_FontAtlas->data);

		for (unsigned int i = 0; i < 128; i++) {
			char c = (char)i;
			ftgl::texture_font_get_glyph(m_Font, &c);
		}
		m_Texture->SetData(m_FontAtlas->data);

	}

	Font::~Font() {
		ftgl::texture_atlas_delete(m_FontAtlas);
		ftgl::texture_font_delete(m_Font);
	}

	float Font::GetWidth(const Label & label) {
		float width = 0.0f;
		const Vec2 & scale = label.Dimension;
		for (int i = 0; i < label.GetText().size(); i++) {
			ftgl::texture_glyph_t * glyph = ftgl::texture_font_get_glyph(m_Font, &label.GetText()[i]);
			if (i > 0) {
				float kerning = ftgl::texture_glyph_get_kerning(glyph, &label.GetText()[i - 1]);
				width += PixelsToScreenX(kerning) / scale.x;
			}
			width += PixelsToScreenX(glyph->advance_x) / scale.x;
		}
		return width;
	}

	float Font::GetHeight(const Label & label) {
		const Vec2 & scale = label.Dimension;
		float min = 0.0f;
		float max = 0.0f;
		for (int i = 0; i < label.GetText().size(); i++) {
			ftgl::texture_glyph_t * glyph = ftgl::texture_font_get_glyph(m_Font, &label.GetText()[i]);
			float height = PixelsToScreenY((float)glyph->height) / scale.y;
			float offset = PixelsToScreenY((float)glyph->offset_y) / scale.y - height;
			if (offset < min)
				min = offset;
			if (height > max)
				max = height;
		}
		return abs(min) + abs(max);
	}

	prev::Vec2 Font::GetSize(const Label & label) {
		return Vec2(GetWidth(label), GetHeight(label));
	}

	void Font::UpdateAtlas() {
		//m_Texture->SetData(m_FontAtlas->data);
	}

}