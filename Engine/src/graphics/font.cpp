#include "pch.h"
#include "font.h"

#include <freetype-gl.h>
#include "application.h"
#include "game/cameracontroller.h"

namespace prev {

	static constexpr const unsigned int TEXTURE_SIZE = 1024;
	static constexpr float FONT_TEXTURE_PADDING = 0.001f;

	Font::Font(const std::string & name, const std::string & fileName, float size) :
		m_FontName(name), m_FontFile(fileName), m_Size(size), m_Texture(nullptr), m_FontAtlas(nullptr), m_Font(nullptr) {

		size = CameraController::Ref().ScreenToPixels(Vec2(size)).y;
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

		ftgl::texture_glyph_t * glyph;

		for (unsigned int i = 0; i < 128; i++) {
			char ch = (char)i;
			glyph = ftgl::texture_font_get_glyph(m_Font, &ch);

			ASSERT(glyph);

			FontCharacter * c = new FontCharacter(ch, glyph);

			m_Characters.insert(std::make_pair(ch, c));
		}
		m_Texture->SetData(m_FontAtlas->data);
	}

	Font::~Font() {
		for (auto & c : m_Characters) {
			delete (c.second);
		}

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
				width += kerning / scale.x;
			}
			width += glyph->advance_x / scale.x;
		}
		return width;
	}

	float Font::GetHeight(const Label & label) {
		const Vec2 & scale = label.Dimension;
		float min = 0.0f;
		float max = 0.0f;
		for (int i = 0; i < label.GetText().size(); i++) {
			ftgl::texture_glyph_t * glyph = ftgl::texture_font_get_glyph(m_Font, &label.GetText()[i]);
			float height = (float)glyph->height / scale.y;
			float offset = (float)glyph->offset_y / scale.y - height;
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

	const prev::FontCharacter * Font::GetCharacter(char c) {
		auto it = m_Characters.find(c);
		if (it == m_Characters.end()) {
			return nullptr;
		} else {
			return it->second;
		}
	}

	prev::Vec2 FontCharacter::GetTexCoordsX() const {
		return Vec2(m_Glyph->s0, m_Glyph->s1 - FONT_TEXTURE_PADDING);
	}

	prev::Vec2 FontCharacter::GetTexCoordsY() const {
		return Vec2(m_Glyph->t0, m_Glyph->t1 - FONT_TEXTURE_PADDING);
	}

	prev::Vec2 FontCharacter::GetSize() const {
		return CameraController::Ref().PixelsToScreen(Vec2((float)m_Glyph->width, (float)m_Glyph->height));
	}

	prev::Vec2 FontCharacter::GetOffset() const {
		return CameraController::Ref().PixelsToScreen(Vec2((float)m_Glyph->offset_x, (float)m_Glyph->offset_y));
	}

	float FontCharacter::GetKerning(const FontCharacter * previousCharacter) const {
		return CameraController::Ref().PixelsToScreen(Vec2(ftgl::texture_glyph_get_kerning(m_Glyph, &previousCharacter->m_C))).x;
	}

	float FontCharacter::GetXAdvance() const {
		return CameraController::Ref().PixelsToScreen(Vec2(m_Glyph->advance_x)).x;
	}

}