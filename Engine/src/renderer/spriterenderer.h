#pragma once

#include "graphics/vertexlayout.h"
#include "graphics/vertexbuffer.h"

#include "game/sprite.h"
#include "graphics/shadermanager.h"

namespace prev {

	// batches will be separated by textures and shaders

	static const unsigned int DEFAULT_DRAW_GROUP = 0u;

	class SpriteRenderer : public Singleton<SpriteRenderer> {
		friend class Singleton<SpriteRenderer>;
	private:
		SpriteRenderer();
		~SpriteRenderer();
	public:
		unsigned int AddEmptyDrawGroup();
		void SetVertexShader(StrongHandle<VertexShader> shader, unsigned int drawGroup);
		void SetPixelShader(StrongHandle<PixelShader> shader, unsigned int drawGroup);
		void SetVertexLayout(StrongHandle<VertexLayout> layout, unsigned int drawGroup);

		void Submit(const Sprite & sprite, unsigned int drawGroup);
		void Render(unsigned int drawGroup);
	private:
		void CreateVertexLayoutDefault();
		void CreateShadersDefault();
		void AddDrawGroup();
	private:

		struct SpriteVertices {
			SpriteVertices(Vec2 center, Vec2 dimension, float rotation);
			// do not change the order
			const Vec2 TopLeft;
			const Vec2 BottomLeft;
			const Vec2 BottomRight;
			const Vec2 TopRight;
		};

		struct TextureCoordinates {
			TextureCoordinates(Vec2 x, Vec2 y);
			const Vec2 TopLeft;
			const Vec2 BottomLeft;
			const Vec2 BottomRight;
			const Vec2 TopRight;
		};

		struct SpriteVertex {
			Vec2 Position;
			Vec2 UV;
			SpriteColor Color;
		};

		//each group will have it's own texture
		struct SpriteGroup {
			SpriteVertex * MappedBuffer;
			unsigned int MappedBufferIndex;

			StrongHandle<VertexLayout> VertexLayout;
			StrongHandle<VertexShader> VertexShader;
			StrongHandle<PixelShader> PixelShader;
			StrongHandle<VertexBuffer> DrawBuffer;
		};

		std::vector<SpriteGroup> m_DrawGroups;

		StrongHandle<VertexLayout> m_VertexLayoutDefault;
		StrongHandle<VertexShader> m_VertexShaderDefault;
		StrongHandle<PixelShader> m_PixelShaderDefault;
	};

}