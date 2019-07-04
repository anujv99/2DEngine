#include "pch.h"
#include "spriterenderer.h"
#include "graphics/renderstate.h"

namespace prev {

	static const unsigned int MAX_NUM_SPRITES = 4096;
	static const unsigned int MAX_NUM_VERTICES_PER_SPRTIE = 6;
	static const unsigned int MAX_NUM_VERTICES = MAX_NUM_SPRITES * MAX_NUM_VERTICES_PER_SPRTIE;

	SpriteRenderer::SpriteVertices::SpriteVertices(Vec2 center, Vec2 dimension, float rotation) {
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

	SpriteRenderer::TextureCoordinates::TextureCoordinates(Vec2 x, Vec2 y) :
		TopLeft(x.x, 1.0f - y.y),
		TopRight(x.y, 1.0f - y.y),
		BottomLeft(x.x, 1.0f - y.x),
		BottomRight(x.y, 1.0f - y.x)
	{}

	SpriteRenderer::SpriteRenderer() {
		CreateShadersDefault();
		CreateVertexLayoutDefault();
		AddDrawGroup();
	}

	SpriteRenderer::~SpriteRenderer() {

	}

	unsigned int SpriteRenderer::AddEmptyDrawGroup() {
		AddDrawGroup();
		return (unsigned int)m_DrawGroups.size() - 1u;
	}

	void SpriteRenderer::SetVertexShader(StrongHandle<VertexShader> shader, unsigned int drawGroup) {
		ASSERT(drawGroup < m_DrawGroups.size());
		ASSERT(shader != nullptr);

		m_DrawGroups[drawGroup].VertexShader = shader;
	}

	void SpriteRenderer::SetPixelShader(StrongHandle<PixelShader> shader, unsigned int drawGroup) {
		ASSERT(drawGroup < m_DrawGroups.size());
		ASSERT(shader != nullptr);

		m_DrawGroups[drawGroup].PixelShader = shader;
	}

	void SpriteRenderer::SetVertexLayout(StrongHandle<VertexLayout> layout, unsigned int drawGroup) {
		ASSERT(drawGroup < m_DrawGroups.size());
		ASSERT(layout != nullptr);

		m_DrawGroups[drawGroup].VertexLayout = layout;
	}

	void SpriteRenderer::Submit(const Sprite & sprite, unsigned int drawGroup /*= DEFAULT_DRAW_GROUP*/) {
		SpriteVertices vertices(sprite.GetPosition(), sprite.GetDimension(), sprite.GetRotation());
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

		drawVertices[0].Color = sprite.GetColor();
		drawVertices[1].Color = sprite.GetColor();
		drawVertices[2].Color = sprite.GetColor();
		drawVertices[3].Color = sprite.GetColor();
		drawVertices[4].Color = sprite.GetColor();
		drawVertices[5].Color = sprite.GetColor();

		std::memcpy(
			m_DrawGroups[drawGroup].MappedBuffer + m_DrawGroups[drawGroup].MappedBufferIndex,
			drawVertices,
			sizeof(drawVertices)
		);

		m_DrawGroups[drawGroup].MappedBufferIndex += (unsigned int)std::size(drawVertices);
	}

	void SpriteRenderer::Render(unsigned int drawGroup /*= DEFAULT_DRAW_GROUP*/) {
		StrongHandle<VertexBuffer> & drawBuffer = m_DrawGroups[drawGroup].DrawBuffer;
		unsigned int & numVertices = m_DrawGroups[drawGroup].MappedBufferIndex;
	
		drawBuffer->UnMap();

		drawBuffer->Bind();

		RenderState::Ref().SetPrimitiveTopology(PV_PRIM_TRIANGLELIST);
		m_DrawGroups[drawGroup].VertexLayout->Bind();
		m_DrawGroups[drawGroup].VertexShader->Bind();
		m_DrawGroups[drawGroup].PixelShader->Bind();
		m_DrawGroups[drawGroup].VertexShader->UpdateMVP();

 		drawBuffer->Draw(numVertices, 0);

		m_DrawGroups[drawGroup].MappedBuffer = reinterpret_cast<SpriteVertex *>(m_DrawGroups[drawGroup].DrawBuffer->Map());
		numVertices = 0u;
	}

	void SpriteRenderer::CreateVertexLayoutDefault() {
		m_VertexLayoutDefault = VertexLayout::CreateVertexLayout();
		m_VertexLayoutDefault->BeginEntries();
		m_VertexLayoutDefault->AddEntry(PV_DATA_TYPE_FLOAT_32, 2, offsetof(SpriteVertex, Position), "POSITION", false);
		m_VertexLayoutDefault->AddEntry(PV_DATA_TYPE_FLOAT_32, 2, offsetof(SpriteVertex, UV), "TEXCOORDS", false);
		m_VertexLayoutDefault->AddEntry(PV_DATA_TYPE_UINT_8, 4, offsetof(SpriteVertex, Color), "COLOR", true);
		m_VertexLayoutDefault->EndEntries(m_VertexShaderDefault);
	}

	void SpriteRenderer::CreateShadersDefault() {
		m_VertexShaderDefault = ShaderManager::Ref().LoadVertexShaderFromFile("SPRITE_VERTEX_SHADER", "res/shaders/spriteDefaultVertex.hlsl");
		m_PixelShaderDefault = ShaderManager::Ref().LoadPixelShaderFromFile("SPRITE_PIXEL_SHADER", "res/shaders/spriteDefaultPixel.hlsl");
	}

	void SpriteRenderer::AddDrawGroup() {
		size_t index = m_DrawGroups.size();

		m_DrawGroups.resize(index + 1);
		m_DrawGroups[index].DrawBuffer = VertexBuffer::CreateVertexBuffer();
		m_DrawGroups[index].DrawBuffer->Init(nullptr, MAX_NUM_VERTICES, sizeof(SpriteVertex), BUFFER_USAGE_STREAM);
		m_DrawGroups[index].MappedBuffer = reinterpret_cast<SpriteVertex *>(m_DrawGroups[index].DrawBuffer->Map());
		m_DrawGroups[index].MappedBufferIndex = 0u;

		m_DrawGroups[index].PixelShader = m_PixelShaderDefault;
		m_DrawGroups[index].VertexShader = m_VertexShaderDefault;
		m_DrawGroups[index].VertexLayout = m_VertexLayoutDefault;
	}

}