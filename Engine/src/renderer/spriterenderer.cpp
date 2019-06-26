#include "pch.h"
#include "spriterenderer.h"

namespace prev {

	static const unsigned int MAX_NUM_SPRITES = 4096;
	static const unsigned int MAX_NUM_VERTICES_PER_SPRTIE = 6;
	static const unsigned int MAX_NUM_VERTICES = MAX_NUM_SPRITES * MAX_NUM_VERTICES_PER_SPRTIE;

	SpriteRenderer::SpriteVertices::SpriteVertices(Vec2 center, Vec2 dimension) {
		// 1x1 square
		static Vec2 squareVertices[] = {
			{ -0.5f,  0.5f }, //ToLeft
			{ -0.5f, -0.5f }, //BottomLeft
			{  0.5f, -0.5f }, //BottomRight
			{  0.5f,  0.5f }, //TopRight
		};

		Vec2 * vertArr = reinterpret_cast<Vec2 *>(this);

		for (unsigned int i = 0; i < 4u; i++) {
			vertArr[i].x = (squareVertices[i].x * dimension.x) + center.x;
			vertArr[i].y = (squareVertices[i].y * dimension.y) + center.y;
		}

	}

	SpriteRenderer::TextureCoordinates::TextureCoordinates(Vec2 x, Vec2 y) :
		TopLeft(x.x, 1.0f - y.y),
		TopRight(x.y, 1.0f - y.y),
		BottomLeft(x.x, 1.0f - y.x),
		BottomRight(x.y, 1.0f - y.x)
	{}

	SpriteRenderer::SpriteRenderer() {
		AddDrawGroup();
		CreateShaders();
		CreateVertexLayout();
	}

	SpriteRenderer::~SpriteRenderer() {

	}

	void SpriteRenderer::Submit(const Sprite & sprite) {
		SpriteVertices vertices(sprite.Position, sprite.Dimension);
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

		//size_t index = m_DrawGroups[0].Vertices.size();
		//m_DrawGroups[0].Vertices.resize(index + 6);
		//std::memcpy(&m_DrawGroups[0].Vertices[index], drawVertices, sizeof(drawVertices));

		unsigned int drawGroup = 0u;

		std::memcpy(
			m_DrawGroups[drawGroup].MappedBuffer + m_DrawGroups[drawGroup].MappedBufferIndex,
			drawVertices,
			sizeof(drawVertices)
		);

		m_DrawGroups[drawGroup].MappedBufferIndex += std::size(drawVertices);
	}

	void SpriteRenderer::Render() {
		unsigned int drawGroup = 0u;

		StrongHandle<VertexBuffer> & drawBuffer = m_DrawGroups[drawGroup].DrawBuffer;
		unsigned int & numVertices = m_DrawGroups[drawGroup].MappedBufferIndex;
	
		drawBuffer->UnMap();

		drawBuffer->Bind();
		m_VertexLayout->Bind();
		m_VertexShader->Bind();
		m_PixelShader->Bind();
		m_VertexShader->UpdateMVP();

 		drawBuffer->Draw(numVertices, 0);

		m_DrawGroups[drawGroup].MappedBuffer = reinterpret_cast<SpriteVertex *>(m_DrawGroups[drawGroup].DrawBuffer->Map());
		numVertices = 0u;
	}

	void SpriteRenderer::CreateVertexLayout() {
		m_VertexLayout = VertexLayout::CreateVertexLayout();
		m_VertexLayout->BeginEntries();
		m_VertexLayout->AddEntry(PV_DATA_TYPE_FLOAT_32, 2, offsetof(SpriteVertex, Position), "POSITION", false);
		m_VertexLayout->AddEntry(PV_DATA_TYPE_FLOAT_32, 2, offsetof(SpriteVertex, UV), "TEXCOORDS", false);
		m_VertexLayout->AddEntry(PV_DATA_TYPE_UINT_8, 4, offsetof(SpriteVertex, Color), "COLOR", true);
		m_VertexLayout->EndEntries(m_VertexShader);
	}

	void SpriteRenderer::CreateShaders() {
		m_VertexShader = ShaderManager::Ref().LoadVertexShaderFromFile("SPRITE_VERTEX_SHADER", "../Engine/res/shaders/spriteDefaultVertex.hlsl");
		m_PixelShader = ShaderManager::Ref().LoadPixelShaderFromFile("SPRITE_PIXEL_SHADER", "../Engine/res/shaders/spriteDefaultPixel.hlsl");
	}

	void SpriteRenderer::AddDrawGroup() {
		size_t index = m_DrawGroups.size();

		m_DrawGroups.resize(index + 1);
		m_DrawGroups[index].DrawBuffer = VertexBuffer::CreateVertexBuffer();
		m_DrawGroups[index].DrawBuffer->Init(nullptr, MAX_NUM_VERTICES, sizeof(SpriteVertex), BUFFER_USAGE_STREAM);
		m_DrawGroups[index].MappedBuffer = reinterpret_cast<SpriteVertex *>(m_DrawGroups[index].DrawBuffer->Map());
		m_DrawGroups[index].MappedBufferIndex = 0u;
	}

}