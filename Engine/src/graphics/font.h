#pragma once

#include "math/vec2.h"
#include "texture2d.h"

namespace ftgl {
	struct texture_atlas_t;
	struct texture_font_t;
}

namespace prev {

	class Font : public HandledObject<Font> {
		friend class Renderer;
	public:
		Font(const std::string & name, const std::string & fileName, float size);
		~Font();

		inline StrongHandle<Texture2D> GetTexture() { return m_Texture; }

		inline void SetScale(Vec2 scale) { m_Scale = scale; }
	private:
		void UpdateAtlas();
	private:
		ftgl::texture_atlas_t * m_FontAtlas;
		ftgl::texture_font_t * m_Font;

		float m_Size;
		Vec2 m_Scale;
		std::string m_FontName;
		std::string m_FontFile;

		mutable StrongHandle<Texture2D> m_Texture;
	};

}