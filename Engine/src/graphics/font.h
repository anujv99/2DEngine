#pragma once

#include "texture2d.h"
#include "sampler2d.h"
#include "vertexbuffer.h"
#include "vertexlayout.h"
#include "vertexshader.h"

#include "math/vec2.h"
#include "math/vec2i.h"
#include "math/vec3.h"
#include "math/vec4.h"

namespace prev {

	class Font : public HandledObject<Font> {
	public:
		Font();
		void Init(const std::string & fileName, float fontSize, StrongHandle<VertexShader> vertexShader);

		void Begin(int textureSlot = 0);
		void End();
		void Print(const std::string & text, Vec2 pos);

		StrongHandle<Texture2D> GetTexture() const { return m_FontTexture; }

		float GetCharacterWidth(char c);
		float GetCharacterHeight(char c);
	private:
		void InitVertexBuffer(StrongHandle<VertexShader> vertexShader);
		bool ReadFiles(const std::string & imageFile, const std::string & fontFile);
		void NormalizeData(Vec2i textureSize);
	private:
		StrongHandle<Texture2D> m_FontTexture;
		StrongHandle<Sampler2D> m_FontSampler;
		StrongHandle<VertexBuffer> m_VertexBuffer;
		StrongHandle<VertexLayout> m_VertexLayout;

		struct Vertex {
			Vec2 Position;
			Vec2 UV;
		};

		std::vector<Vertex> m_CharacterBuffer;
		unsigned int m_BufferIndex;
		float m_FontSize;

		static const int NUM_CHARS = 256;

		Vec4 m_CharacterUV[NUM_CHARS];
		Vec3 m_CharacterPositions[NUM_CHARS];
		bool m_IsInitialized;

		float m_CapitalLetterHeight; // for calculating offset of lower case characters
	};

}
