#pragma once

#include "graphics/vertexlayout.h"
#include "graphics/vertexbuffer.h"

#include "game/sprite.h"
#include "graphics/shadermanager.h"

namespace prev {

	// batches will be separated by texture id

	class SpriteRenderer : public Singleton<SpriteRenderer> {
		friend class Singleton<SpriteRenderer>;
	private:
		SpriteRenderer();
		~SpriteRenderer();
	public:
		void Submit(const Sprite & sprite);
		void Render();
	private:
		void CreateVertexLayout();
		void CreateShaders();
		void AddDrawGroup();
	private:

		struct SpriteVertices {
			SpriteVertices(Vec2 center, Vec2 dimension);
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
			std::vector<SpriteVertex> Vertices;
			StrongHandle<VertexBuffer> DrawBuffer;
		};

		std::vector<SpriteGroup> m_DrawGroups;

		StrongHandle<VertexLayout> m_VertexLayout;
		StrongHandle<VertexShader> m_VertexShader;
		StrongHandle<PixelShader> m_PixelShader;
	};

}