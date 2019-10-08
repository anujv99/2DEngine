#pragma once

#include "graphics/vertexlayout.h"
#include "graphics/vertexbuffer.h"
#include "graphics/shadermanager.h"
#include "graphics/font.h"
#include "graphics/renderstate.h"

#include "game/label.h"
#include "game/sprite.h"
#include "game/particlesystem.h"
#include "game/drawable.h"

namespace prev {

	// batches will be separated by shaders

	static const unsigned int DEFAULT_DRAW_GROUP = 0u;
	static const std::string DEFAULT_RENDER_STATE_NAME = "DEFAULT_RENDER_STATE";

	class Renderer : public Singleton<Renderer> {
		struct SpriteGroup;
		friend class Singleton<Renderer>;
	private:
		Renderer();
		~Renderer();
	public:
		void Submit(const Sprite & sprite, StrongHandle<Texture2D> texture = nullptr, 
			StrongHandle<VertexShader> vShader = nullptr, StrongHandle<PixelShader> pShader = nullptr);

		void Submit(const Drawable & sprite, StrongHandle<VertexShader> vShader = nullptr, StrongHandle<PixelShader> pShader = nullptr);

		void Submit(const ParticleSystem & system, StrongHandle<VertexShader> vShader = nullptr, StrongHandle<PixelShader> pShader = nullptr);

		void Submit(const Label & label, StrongHandle<Font> font, 
			StrongHandle<VertexShader> vShader = nullptr, StrongHandle<PixelShader> pShader = nullptr);
		void Present();
	private:
		void CreateVertexLayoutDefault();
		void CreateShadersDefault();
		SpriteGroup * GetDrawGroup(StrongHandle<VertexShader> vShader, StrongHandle<PixelShader> pShader, const std::string & renderState = DEFAULT_RENDER_STATE_NAME);

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
			Vec3 Position;
			Vec2 UV;
			SpriteColor Color;
			int TexID = -1;
		};

		int a = sizeof(SpriteVertex);

		class RenderStateChanges : public HandledObject<RenderStateChanges> {
		public:
			RenderStateChanges() : m_IsPrimitiveTopologyChanged(false), m_IsViewportChanged(false), m_IsScissorBoxChanged(false), m_IsBlendChanged(false) {}

			void SetNewPrimitiveTopology(const PrimitiveTopology & pt) { m_Prim = pt; m_IsPrimitiveTopologyChanged = true; }
			void SetNewViewport(const Viewport & vp) { m_Viewport = vp; m_IsViewportChanged = true; }
			void SetNewScissorBox(const ScissorBox & sb) { m_ScissorBox = sb; m_IsScissorBoxChanged = true; }
			void SetNewBlendFunction(const BlendFunction & bf) { m_Blend = bf; m_IsBlendChanged = true; }

			void Bind() {
				if (m_IsPrimitiveTopologyChanged) {
					m_OldPrim = RenderState::Ref().GetPrimitiveTopology();
					RenderState::Ref().SetPrimitiveTopology(m_Prim);
				}
				if (m_IsViewportChanged) {
					m_OldViewport = RenderState::Ref().GetViewport();
					RenderState::Ref().SetViewport(m_Viewport);
				}
				if (m_IsScissorBoxChanged) {
					m_OldScissorBox = RenderState::Ref().GetScissorBox();
					RenderState::Ref().SetScissorBox(m_ScissorBox);
				}
				if (m_IsBlendChanged) {
					m_OldBlend = RenderState::Ref().GetBlendFunction();
					RenderState::Ref().SetBlendFunction(m_Blend);
				}
			}

			void UnBind() {
				if (m_IsPrimitiveTopologyChanged)
					RenderState::Ref().SetPrimitiveTopology(m_OldPrim);
				if (m_IsViewportChanged)
					RenderState::Ref().SetViewport(m_OldViewport);
				if (m_IsScissorBoxChanged)
					RenderState::Ref().SetScissorBox(m_OldScissorBox);
				if (m_IsBlendChanged)
					RenderState::Ref().SetBlendFunction(m_OldBlend);
			}
		private:
			PrimitiveTopology m_Prim, m_OldPrim;
			bool m_IsPrimitiveTopologyChanged;

			Viewport m_Viewport, m_OldViewport;
			bool m_IsViewportChanged;

			ScissorBox m_ScissorBox, m_OldScissorBox;
			bool m_IsScissorBoxChanged;

			BlendFunction m_Blend, m_OldBlend;
			bool m_IsBlendChanged;
		};

		//each group will have it's own texture
		struct SpriteGroup {
			SpriteVertex * MappedBuffer;
			unsigned int MappedBufferIndex;

			std::vector<StrongHandle<Texture2D>> Textures;

			StrongHandle<VertexLayout> VertexLayout;
			StrongHandle<VertexShader> VertexShader;
			StrongHandle<PixelShader> PixelShader;
			StrongHandle<VertexBuffer> DrawBuffer;
			StrongHandle<RenderStateChanges> NewRenderState;
		};

		// HashStringPair(VertexShader::m_Name, PixelShader::m_Name) -> SpriteGroup
		// In Case of different render state
		// HashStringPair(VertexShader::m_Name + PixelShader::m_Name, RenderStateName) -> SpriteGroup
		std::unordered_map<uint64_t, SpriteGroup> m_DrawGroups;

		StrongHandle<VertexLayout> m_VertexLayoutDefault;

		// default shader for rendering sprite
		StrongHandle<VertexShader> m_SpriteVertexShaderDefault;
		StrongHandle<PixelShader> m_SpritePixelShaderDefault;

		// default shader for rendering particles
		StrongHandle<VertexShader> m_ParticleVertexShaderDefault;
		StrongHandle<PixelShader> m_ParticlePixelShaderDefault;

		// default texture to disable EXECUTION WARNING #352: DEVICE_DRAW_SAMPLER_NOT_SET
		StrongHandle<Texture2D> m_DefaultTexture;
	};

}