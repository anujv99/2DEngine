#include "pch.h"
#include "renderer.h"
#include "graphics/renderstate.h"

namespace prev {

	static const unsigned int MAX_NUM_SPRITES = 1024 * 16;
	static const unsigned int MAX_NUM_VERTICES_PER_SPRTIE = 6;
	static const unsigned int MAX_NUM_VERTICES = MAX_NUM_SPRITES * MAX_NUM_VERTICES_PER_SPRTIE;

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

	void Renderer::Submit(const Sprite & sprite, StrongHandle<VertexShader> vShader, StrongHandle<PixelShader> pShader) {
		SpriteVertices vertices(sprite.Position, sprite.Dimension, sprite.Rotation);
		static TextureCoordinates defaultUvs(Vec2(0, 1), Vec2(0, 1));

		SpriteVertex drawVertices[6];
		drawVertices[0].Position = vertices.TopLeft;
		drawVertices[1].Position = vertices.BottomRight;
		drawVertices[2].Position = vertices.BottomLeft;
		drawVertices[3].Position = vertices.TopLeft;
		drawVertices[4].Position = vertices.TopRight;
		drawVertices[5].Position = vertices.BottomRight;

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

		std::memcpy(
			drawGroup->MappedBuffer + drawGroup->MappedBufferIndex,
			drawVertices,
			sizeof(drawVertices)
		);

		drawGroup->MappedBufferIndex += (unsigned int)std::size(drawVertices);
	}

	void Renderer::Submit(const ParticleSystem & system, StrongHandle<VertexShader> vShader, StrongHandle<PixelShader> pShader) {
		if (vShader == nullptr) vShader = m_SpriteVertexShaderDefault;
		if (pShader == nullptr) pShader = m_ParticlePixelShaderDefault;
		auto drawGroup = GetDrawGroup(vShader, pShader);

		TextureCoordinates defaultUvs(Vec2(0, 1), Vec2(0, 1));

		for (auto & part : system.m_Particles) {
			SpriteVertices vertices(part.Position, Vec2(part.CurrentScale), 0);

			SpriteVertex drawVertices[6];
			drawVertices[0].Position = vertices.TopLeft;
			drawVertices[1].Position = vertices.BottomRight;
			drawVertices[2].Position = vertices.BottomLeft;
			drawVertices[3].Position = vertices.TopLeft;
			drawVertices[4].Position = vertices.TopRight;
			drawVertices[5].Position = vertices.BottomRight;

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

		drawBuffer->Draw(numVertices, 0);

		group->MappedBuffer = reinterpret_cast<SpriteVertex *>(group->DrawBuffer->Map());
		numVertices = 0u;
	}

	void Renderer::CreateVertexLayoutDefault() {
		m_VertexLayoutDefault = VertexLayout::CreateVertexLayout();
		m_VertexLayoutDefault->BeginEntries();
		m_VertexLayoutDefault->AddEntry(PV_DATA_TYPE_FLOAT_32, 2, offsetof(SpriteVertex, Position), "POSITION", false);
		m_VertexLayoutDefault->AddEntry(PV_DATA_TYPE_FLOAT_32, 2, offsetof(SpriteVertex, UV), "TEXCOORDS", false);
		m_VertexLayoutDefault->AddEntry(PV_DATA_TYPE_UINT_8, 4, offsetof(SpriteVertex, Color), "COLOR", true);
		m_VertexLayoutDefault->EndEntries(m_SpriteVertexShaderDefault);
	}

	void Renderer::CreateShadersDefault() {
		m_SpriteVertexShaderDefault = ShaderManager::Ref().LoadVertexShaderFromFile("SPRITE_VERTEX_SHADER", "res/shaders/spriteDefaultVertex.hlsl");
		m_SpritePixelShaderDefault = ShaderManager::Ref().LoadPixelShaderFromFile("SPRITE_PIXEL_SHADER", "res/shaders/spriteDefaultPixel.hlsl");

		m_ParticlePixelShaderDefault = ShaderManager::Ref().LoadPixelShaderFromFile("PARTICLE_DEFAULT_PIXEL", "res/shaders/particleDefaultPixel.hlsl");
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

		auto val = m_DrawGroups.insert(std::make_pair(key, group));
		return &val.first->second;
	}

}