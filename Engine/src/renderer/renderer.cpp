#include "pch.h"
#include "renderer.h"
#include "graphics/renderstate.h"

#include <freetype-gl.h>
#include "math/screenspace.h"

namespace prev {

	static constexpr unsigned int MAX_NUM_SPRITES					= 1024 * 16;
	static constexpr unsigned int MAX_NUM_VERTICES_PER_SPRTIE		= 6;
	static constexpr unsigned int MAX_NUM_VERTICES					= MAX_NUM_SPRITES * MAX_NUM_VERTICES_PER_SPRTIE;
	static constexpr unsigned int MAX_NUM_TEXTURES					= 16;

	static constexpr float FONT_TEXTURE_PADDING						= 0.001f;

	Renderer::SpriteVertices::SpriteVertices(Vec2 center, Vec2 dimension, float rotation) {
		static const Vec2 squareVertices[] = {
			{ -0.5f,  0.5f }, //ToLeft
			{ -0.5f, -0.5f }, //BottomLeft
			{  0.5f, -0.5f }, //BottomRight
			{  0.5f,  0.5f }, //TopRight
		};

		Vec2 * vertArr = reinterpret_cast<Vec2 *>(this);

		if (rotation == 0) {
			for (unsigned int i = 0; i < 4u; i++) {
				vertArr[i].x = (squareVertices[i].x * dimension.x) + center.x;
				vertArr[i].y = (squareVertices[i].y * dimension.y) + center.y;
			}
		} else {
			for (unsigned int i = 0; i < 4u; i++) {
				vertArr[i].x = (squareVertices[i].x * dimension.x);
				vertArr[i].y = (squareVertices[i].y * dimension.y);
				vertArr[i] = Rotate(vertArr[i], rotation);
				vertArr[i] += center;
			}
		}
	}

	Renderer::TextureCoordinates::TextureCoordinates(Vec2 x, Vec2 y) :
		TopLeft(x.x, 1.0f - y.y),
		TopRight(x.y, 1.0f - y.y),
		BottomLeft(x.x, 1.0f - y.x),
		BottomRight(x.y, 1.0f - y.x)
	{}

	Renderer::Renderer() {
		CreateShadersDefault();
		CreateVertexLayoutDefault();
	}

	Renderer::~Renderer() {

	}

	void Renderer::Submit(const Sprite & sprite, StrongHandle<Texture2D> texture, 
		StrongHandle<VertexShader> vShader, StrongHandle<PixelShader> pShader) {

		SpriteVertices vertices(sprite.Position, sprite.Dimension, sprite.Rotation);
		static TextureCoordinates defaultUvs(Vec2(0, 1), Vec2(0, 1));

		SpriteVertex drawVertices[6];
		drawVertices[0].Position = Vec3(vertices.TopLeft, sprite.Depth);
		drawVertices[1].Position = Vec3(vertices.BottomRight, sprite.Depth);
		drawVertices[2].Position = Vec3(vertices.BottomLeft, sprite.Depth);
		drawVertices[3].Position = Vec3(vertices.TopLeft, sprite.Depth);
		drawVertices[4].Position = Vec3(vertices.TopRight, sprite.Depth);
		drawVertices[5].Position = Vec3(vertices.BottomRight, sprite.Depth);

		drawVertices[0].UV = defaultUvs.TopLeft;
		drawVertices[1].UV = defaultUvs.BottomRight;
		drawVertices[2].UV = defaultUvs.BottomLeft;
		drawVertices[3].UV = defaultUvs.TopLeft;
		drawVertices[4].UV = defaultUvs.TopRight;
		drawVertices[5].UV = defaultUvs.BottomRight;

		drawVertices[0].Color = sprite.Color;
		drawVertices[1].Color = sprite.Color;
		drawVertices[2].Color = sprite.Color;
		drawVertices[3].Color = sprite.Color;
		drawVertices[4].Color = sprite.Color;
		drawVertices[5].Color = sprite.Color;

		SpriteGroup * drawGroup = GetDrawGroup(vShader, pShader);

		if (texture != nullptr) {
			auto texID = SubmitTexture(drawGroup, texture);
			drawVertices[0].TexID = texID;
			drawVertices[1].TexID = texID;
			drawVertices[2].TexID = texID;
			drawVertices[3].TexID = texID;
			drawVertices[4].TexID = texID;
			drawVertices[5].TexID = texID;
		}

		std::memcpy(
			drawGroup->MappedBuffer + drawGroup->MappedBufferIndex,
			drawVertices,
			sizeof(drawVertices)
		);

		drawGroup->MappedBufferIndex += (unsigned int)std::size(drawVertices);
	}

	void Renderer::Submit(const ParticleSystem & system, StrongHandle<VertexShader> vShader, StrongHandle<PixelShader> pShader) {
		if (vShader == nullptr) vShader = m_ParticleVertexShaderDefault;
		if (pShader == nullptr) pShader = m_ParticlePixelShaderDefault;
		auto drawGroup = GetDrawGroup(vShader, pShader);

		TextureCoordinates defaultUvs(Vec2(0, 1), Vec2(0, 1));

		for (auto & part : system.m_Particles) {
			SpriteVertices vertices(part.Position, Vec2(part.CurrentScale), 0);

			SpriteVertex drawVertices[6];
			drawVertices[0].Position = Vec3(vertices.TopLeft, 0.0f);
			drawVertices[1].Position = Vec3(vertices.BottomRight, 0.0f);
			drawVertices[2].Position = Vec3(vertices.BottomLeft, 0.0f);
			drawVertices[3].Position = Vec3(vertices.TopLeft, 0.0f);
			drawVertices[4].Position = Vec3(vertices.TopRight, 0.0f);
			drawVertices[5].Position = Vec3(vertices.BottomRight, 0.0f);

			drawVertices[0].UV = defaultUvs.TopLeft;
			drawVertices[1].UV = defaultUvs.BottomRight;
			drawVertices[2].UV = defaultUvs.BottomLeft;
			drawVertices[3].UV = defaultUvs.TopLeft;
			drawVertices[4].UV = defaultUvs.TopRight;
			drawVertices[5].UV = defaultUvs.BottomRight;

			drawVertices[0].Color = Vec4(part.CurrentColor, part.CurrentAlpha);
			drawVertices[1].Color = Vec4(part.CurrentColor, part.CurrentAlpha);
			drawVertices[2].Color = Vec4(part.CurrentColor, part.CurrentAlpha);
			drawVertices[3].Color = Vec4(part.CurrentColor, part.CurrentAlpha);
			drawVertices[4].Color = Vec4(part.CurrentColor, part.CurrentAlpha);
			drawVertices[5].Color = Vec4(part.CurrentColor, part.CurrentAlpha);

			std::memcpy(
				drawGroup->MappedBuffer + drawGroup->MappedBufferIndex,
				drawVertices,
				sizeof(drawVertices)
			);

			drawGroup->MappedBufferIndex += (unsigned int)std::size(drawVertices);
		}

		BlendFunction bf;
		bf.SrcBlend = PV_BLEND_SRC_ALPHA;
		bf.DestBlend = PV_BLEND_ONE;
		bf.Operation = PV_BLEND_OP_ADD;

		BlendFunction pbf = RenderState::Ref().GetBlendFunction();
		RenderState::Ref().SetBlendFunction(bf);

		DrawGroup(drawGroup);

		RenderState::Ref().SetBlendFunction(pbf);
	}

	void Renderer::Submit(const Label & label, StrongHandle<Font> font, StrongHandle<VertexShader> vShader, StrongHandle<PixelShader> pShader) {
		
		SpriteGroup * group = GetDrawGroup(vShader, pShader);
		int texID = SubmitTexture(group, font->m_Texture);

		const Vec2 & scale = label.Dimension;
		float x = label.Position.x;

		ftgl::texture_font_t * ftFont = font->m_Font;

		std::string & text = label.GetText();

		float xSize = font->GetWidth(label);

		for (unsigned int i = 0; i < text.length(); i++) {
			char c = text[i];
			ftgl::texture_glyph_t * glyph = ftgl::texture_font_get_glyph(ftFont, &c);

			if (glyph) {
				if (i > 0) {
					float kerning = ftgl::texture_glyph_get_kerning(glyph, &text[i - 1]);
					x += PixelsToScreenX(kerning) * scale.x;
				}

				float x0 = x + PixelsToScreenX(glyph->offset_x) * scale.x;
				float y0 = label.Position.y + PixelsToScreenY(glyph->offset_y) * scale.y;
				float x1 = x0 + PixelsToScreenX(glyph->width) * scale.x;
				float y1 = y0 - PixelsToScreenY(glyph->height) * scale.y;

				float u0 = glyph->s0;
				float v0 = glyph->t0;
				float u1 = glyph->s1 - FONT_TEXTURE_PADDING;
				float v1 = glyph->t1 - FONT_TEXTURE_PADDING;

				switch (label.Alignment) {
				case PV_LABEL_ALIGNMENT_LEFT:
				{
					x0 -= xSize;
					x1 -= xSize;
					break;
				}
				case PV_LABEL_ALIGNMENT_MIDDLE:
				{
					x0 -= xSize / 2.0f;
					x1 -= xSize / 2.0f;
					break;
				}
				default:
					break;
				}

				SpriteVertex drawVertices[6];
				drawVertices[0].Position = { x0, y1, 0.0f };
				drawVertices[1].Position = { x1, y0, 0.0f };
				drawVertices[2].Position = { x0, y0, 0.0f };
				drawVertices[3].Position = { x0, y1, 0.0f };
				drawVertices[4].Position = { x1, y1, 0.0f };
				drawVertices[5].Position = { x1, y0, 0.0f };

				drawVertices[0].UV = { u0, v1 };
				drawVertices[1].UV = { u1, v0 };
				drawVertices[2].UV = { u0, v0 };
				drawVertices[3].UV = { u0, v1 };
				drawVertices[4].UV = { u1, v1 };
				drawVertices[5].UV = { u1, v0 };

				drawVertices[0].Color = label.Color;
				drawVertices[1].Color = label.Color;
				drawVertices[2].Color = label.Color;
				drawVertices[3].Color = label.Color;
				drawVertices[4].Color = label.Color;
				drawVertices[5].Color = label.Color;

				drawVertices[0].TexID = texID;
				drawVertices[1].TexID = texID;
				drawVertices[2].TexID = texID;
				drawVertices[3].TexID = texID;
				drawVertices[4].TexID = texID;
				drawVertices[5].TexID = texID;

				std::memcpy(
					group->MappedBuffer + group->MappedBufferIndex,
					drawVertices,
					sizeof(drawVertices)
				);

				group->MappedBufferIndex += (unsigned int)std::size(drawVertices);

				x += PixelsToScreenX(glyph->advance_x) * scale.x;
			}

		}

		if (label.GetDirty()) {
			font->UpdateAtlas();
			label.SetDirty(false);
		}

	}

	void Renderer::Present() {
		for (auto & it : m_DrawGroups) {
			DrawGroup(&it.second);
		}
	}

	void Renderer::DrawGroup(SpriteGroup * group) {

		StrongHandle<VertexBuffer> & drawBuffer = group->DrawBuffer;
		unsigned int & numVertices = group->MappedBufferIndex;

		if (numVertices == 0) return;

		drawBuffer->UnMap();

		drawBuffer->Bind();

		RenderState::Ref().SetPrimitiveTopology(PV_PRIM_TRIANGLELIST);
		group->VertexLayout->Bind();
		group->VertexShader->Bind();
		group->PixelShader->Bind();
		group->VertexShader->UpdateMVP();

		for (size_t i = 0; i < group->Textures.size(); i++) {
			group->Textures[i]->SetTextureSlot(i);
			group->Textures[i]->Bind();
		}
		
		drawBuffer->Draw(numVertices, 0);

		group->Textures.clear();
		group->Textures.reserve(MAX_NUM_TEXTURES);

		group->MappedBuffer = reinterpret_cast<SpriteVertex *>(group->DrawBuffer->Map());
		numVertices = 0u;
	}

	unsigned int Renderer::SubmitTexture(SpriteGroup * group, StrongHandle<Texture2D> texture) {
		if ((unsigned int)group->Textures.size() >= MAX_NUM_TEXTURES)
			DrawGroup(group);

		for (size_t i = 0; i < group->Textures.size(); i++) {
			if (group->Textures[i] == texture) {
				return i;
			}
		}

		group->Textures.push_back(texture);
		return (unsigned int)group->Textures.size() - 1;
	}

	void Renderer::CreateVertexLayoutDefault() {
		m_VertexLayoutDefault = VertexLayout::CreateVertexLayout();
		m_VertexLayoutDefault->BeginEntries();
		m_VertexLayoutDefault->AddEntry(PV_DATA_TYPE_FLOAT_32, 3, offsetof(SpriteVertex, Position), "POSITION", false);
		m_VertexLayoutDefault->AddEntry(PV_DATA_TYPE_FLOAT_32, 2, offsetof(SpriteVertex, UV), "TEXCOORDS", false);
		m_VertexLayoutDefault->AddEntry(PV_DATA_TYPE_UINT_8, 4, offsetof(SpriteVertex, Color), "COLOR", true);
		m_VertexLayoutDefault->AddEntry(PV_DATA_TYPE_SINT_32, 1, offsetof(SpriteVertex, TexID), "TEXTUREID", false);
		m_VertexLayoutDefault->EndEntries(m_SpriteVertexShaderDefault);
	}

	void Renderer::CreateShadersDefault() {
		m_SpriteVertexShaderDefault = ShaderManager::Ref().LoadVertexShaderFromFile("SPRITE_VERTEX_SHADER", "res/shaders/spriteDefaultVertex.hlsl");
		m_SpritePixelShaderDefault = ShaderManager::Ref().LoadPixelShaderFromFile("SPRITE_PIXEL_SHADER", "res/shaders/spriteDefaultPixel.hlsl");

		m_ParticleVertexShaderDefault = ShaderManager::Ref().LoadVertexShaderFromFile("PARTICLE_DEFAULT_PIXEL", "res/shaders/particleDefaultVertex.hlsl");
		m_ParticlePixelShaderDefault = ShaderManager::Ref().LoadPixelShaderFromFile("PARTICLE_DEFAULT_PIXEL", "res/shaders/particleDefaultPixel.hlsl");

		m_DefaultTexture = Texture2D::CreateTexture2D();
		Texture2DDesc desc;
		desc.TexFormat = PV_TEXTURE_FORMAT_RGBA8;
		desc.TextureSize = Vec2i(10);
		m_DefaultTexture->Init(desc);

		for (unsigned int i = 0; i < MAX_NUM_TEXTURES; i++) {
			m_DefaultTexture->SetTextureSlot(i);
			m_DefaultTexture->Bind();
		}
	}

	prev::Renderer::SpriteGroup * Renderer::GetDrawGroup(StrongHandle<VertexShader> vShader, StrongHandle<PixelShader> pShader) {
		if (vShader == nullptr) vShader = m_SpriteVertexShaderDefault;
		if (pShader == nullptr) pShader = m_SpritePixelShaderDefault;

		uint64_t key = HashStringPair(vShader->GetShaderName(), pShader->GetShaderName());
		auto it = m_DrawGroups.find(key);

		if (it != m_DrawGroups.end()) {
			return &it->second;
		}

		SpriteGroup group;

		group.DrawBuffer = VertexBuffer::CreateVertexBuffer();
		group.DrawBuffer->Init(nullptr, MAX_NUM_VERTICES, sizeof(SpriteVertex), BUFFER_USAGE_STREAM);
		group.MappedBuffer = reinterpret_cast<SpriteVertex *>(group.DrawBuffer->Map());
		group.MappedBufferIndex = 0u;

		group.PixelShader = pShader;
		group.VertexShader = vShader;
		group.VertexLayout = m_VertexLayoutDefault;

		group.Textures.reserve(MAX_NUM_TEXTURES);

		auto val = m_DrawGroups.insert(std::make_pair(key, group));
		return &val.first->second;
	}

}