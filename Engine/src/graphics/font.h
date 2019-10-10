#pragma once

#include "math/vec2.h"
#include "texture2d.h"
#include "game/label.h"

namespace ftgl {
	struct texture_atlas_t;
	struct texture_font_t;
	struct texture_glyph_t;
}

namespace prev {

	class FontCharacter {
		friend class Font;
	private:
		FontCharacter(char c, ftgl::texture_glyph_t * glyph) : m_C(c), m_Glyph(glyph) {
			ASSERT(glyph);
		}
		~FontCharacter() {}
	public:
		Vec2 GetTexCoordsX() const;
		Vec2 GetTexCoordsY() const;
		Vec2 GetSize() const;
		Vec2 GetOffset() const;
		float GetKerning(const FontCharacter * previousCharacter) const;
		float GetXAdvance() const;
	private:
		ftgl::texture_glyph_t * m_Glyph;
		char m_C;
	};

	class Font : public HandledObject<Font> {
		friend class Renderer;
	public:
		Font(const std::string & name, const std::string & fileName, float size);
		~Font();

		inline StrongHandle<Texture2D> GetTexture() { return m_Texture; }

		float GetWidth(const Label & label);
		float GetHeight(const Label & label);
		Vec2 GetSize(const Label & label);

		const FontCharacter * GetCharacter(char c);
	private:
		ftgl::texture_atlas_t * m_FontAtlas;
		ftgl::texture_font_t * m_Font;

		float m_Size;
		std::string m_FontName;
		std::string m_FontFile;

		StrongHandle<Texture2D> m_Texture;
		std::unordered_map<char, FontCharacter *> m_Characters;
	};

}