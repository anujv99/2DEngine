#pragma once

#include "math/vec4.h"

#include "texture2d.h"

namespace prev {

	class TiledTexture : public HandledObject<TiledTexture> {
	public:
		TiledTexture(const std::string & fileName, Vec2i division);
		TiledTexture(StrongHandle<Texture2D> texture, Vec2i division);
		~TiledTexture();

		Vec2 GetTextureCoordX(unsigned int texUnitX);
		Vec2 GetTextureCoordY(unsigned int texUnitY);

		Vec4 GetTextureCorrds(Vec2i texUnit);

		inline StrongHandle<Texture2D> & GetTexture() { return m_Texture; }
	private:
		void CalculateData();
	private:
		Vec2 m_UnitSize;
		Vec2i m_Division;
		StrongHandle<Texture2D> m_Texture;
	};

}