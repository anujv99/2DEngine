#include "pch.h"
#include "font.h"

#include <freetype-gl.h>

namespace prev {

	Font::Font(const std::string & name, const std::string & fileName, float size) :
		m_FontName(name), m_FontFile(fileName), m_Size(size), m_Scale(1.0f), m_Texture(nullptr), m_FontAtlas(nullptr), m_Font(nullptr) {

		m_FontAtlas = ftgl::texture_atlas_new(512, 512, 1);
		m_Font = ftgl::texture_font_new_from_file(m_FontAtlas, size, fileName.c_str());

		m_Texture = Texture2D::CreateTexture2D();
		Texture2DDesc desc;
		desc.TexFormat = PV_TEXTURE_FORMAT_RGBA8;
		desc.TextureSize = Vec2i(512, 512);

		TextureParams params;
		params.Filtering = PV_TEXTURE_FILTER_LINEAR;
		params.Wrapping = PV_TEXTURE_WRAP_CLAMP;

		m_Texture->Init(desc, params, 0);
		m_Texture->SetData(m_FontAtlas->data);
	}

	Font::~Font() {
		ftgl::texture_atlas_delete(m_FontAtlas);
		ftgl::texture_font_delete(m_Font);
	}

}