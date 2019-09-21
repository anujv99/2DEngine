#include "pch.h"
#include "tiledtexture.h"

#include "math/vecconversion.h"

namespace prev {

	TiledTexture::TiledTexture(const std::string & fileName, Vec2i division) :
		m_UnitSize(0.0f), m_Division(division), m_Texture(nullptr) {

		m_Texture = Texture2D::CreateTexture2D();
		TextureParams params;
		params.Filtering = PV_TEXTURE_FILTER_NEAREST;
		params.Wrapping = PV_TEXTURE_WRAP_CLAMP;
		m_Texture->Init(fileName, params);
		CalculateData();
	}

	TiledTexture::TiledTexture(StrongHandle<Texture2D> texture, Vec2i division) :
		m_UnitSize(0.0f), m_Division(division), m_Texture(texture) {
		CalculateData();
	}

	TiledTexture::~TiledTexture() {}

	prev::Vec2 TiledTexture::GetTextureCoordX(unsigned int texUnitX) {
		return Vec2(m_UnitSize.x) * Vec2((float)texUnitX, (float)texUnitX + 1.0f);
	}

	prev::Vec2 TiledTexture::GetTextureCoordY(unsigned int texUnitY) {
		return Vec2(m_UnitSize.y) * Vec2((float)texUnitY, (float)texUnitY + 1.0f);
	}

	prev::Vec4 TiledTexture::GetTextureCorrds(Vec2i texUnit) {
		return Vec4(GetTextureCoordX(texUnit.x), GetTextureCoordY(texUnit.y));
	}

	void TiledTexture::CalculateData() {
		m_UnitSize = Vec2(1.0f) / ToVec2(m_Division);
	}

}