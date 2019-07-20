#pragma once

#include "graphics/vertexlayout.h"
#include "graphics/vertexbuffer.h"
#include "graphics/shadermanager.h"
#include "graphics/font.h"

#include "game/label.h"
#include "game/sprite.h"
#include "game/particlesystem.h"

namespace prev {

	// batches will be separated by shaders

	static const unsigned int DEFAULT_DRAW_GROUP = 0u;

	class Renderer : public Singleton<Renderer> {
		struct SpriteGroup;
		friend class Singleton<Renderer>;
	private:
		Renderer();
		~Renderer();
	public:
		void Submit(const Sprite & sprite, StrongHandle<Texture2D> texture = nullptr, 
			StrongHandle<VertexShader> vShader = nullptr, StrongHandle<PixelShader> pShader = nullptr);

		void Submit(const ParticleSystem & system, StrongHandle<VertexShader> vShader = nullptr, StrongHandle<PixelShader> pShader = nullptr);

		void Submit(const Label & label, StrongHandle<Font> font, 
			StrongHandle<VertexShader> vShader = nullptr, StrongHandle<PixelShader> pShader = nullptr);
		void Present();
	private:
		void CreateVertexLayoutDefault();
		void CreateShadersDefault();
		SpriteGroup * GetDrawGroup(StrongHandle<VertexShader> vShader, StrongHandle<PixelShader> pShader);

		void DrawGroup(SpriteGroup * group);

		unsigned int SubmitTexture(SpriteGroup * group, StrongHandle<Texture2D> texture);
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
			int TexID = -1;
		};

		int a = sizeof(SpriteVertex);

		//each group will have it's own texture
		struct SpriteGroup {
			SpriteVertex * MappedBuffer;
			unsigned int MappedBufferIndex;

			std::vector<StrongHandle<Texture2D>> Textures;

			StrongHandle<VertexLayout> VertexLayout;
			StrongHandle<VertexShader> VertexShader;
			StrongHandle<PixelShader> PixelShader;
			StrongHandle<VertexBuffer> DrawBuffer;
		};

		std::unordered_map<uint64_t, SpriteGroup> m_DrawGroups; // HashStringPair(VertexShader::m_Name, PixelShader::m_Name) -> SpriteGroup

		StrongHandle<VertexLayout> m_VertexLayoutDefault;

		// default shader for rendering sprite
		StrongHandle<VertexShader> m_SpriteVertexShaderDefault;
		StrongHandle<PixelShader> m_SpritePixelShaderDefault;

		// default shader for rendering particles
		StrongHandle<VertexShader> m_ParticleVertexShaderDefault;
		StrongHandle<PixelShader> m_ParticlePixelShaderDefault;
	};

}