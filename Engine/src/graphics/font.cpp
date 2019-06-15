#include "pch.h"
#include "font.h"

#include "utils/filehandler.h"

#include "renderstate.h"

namespace prev {

	static const unsigned int MAX_VERTEX_PER_CHAR	= 6;
	static const unsigned int MAX_STRING_LENGTH		= 4096;
	static const unsigned int MAX_NUM_VERTICES		= MAX_STRING_LENGTH * MAX_VERTEX_PER_CHAR;

	static const float TEXT_DIVISOR					= 16;

	Font::Font() : m_IsInitialized(false), m_BufferIndex(0u), m_FontSize(0u), m_CapitalLetterHeight(0.0f) {}

	void Font::Init(const std::string & fileName, float fontSize, StrongHandle<VertexShader> vertexShader) {
		if (m_IsInitialized) {
			LOG_WARN("Font already initialized");
			return;
		}
		if (fileName.size() == 0) {
			LOG_WARN("Using invalid fileName for the font");
			return;
		}
		if (fontSize == 0) {
			LOG_WARN("Font size cannot be zero");
			return;
		}
		if (!FileExists(fileName)) {
			LOG_ERROR("Unable to find font file : " + fileName);
			return;
		}

		std::string secondFile;

		if (fileName.find(".png") != std::string::npos) {
			secondFile = fileName.substr(0, fileName.find_last_of(".")) + ".fnt";

			if (!FileExists(secondFile)) {
				LOG_ERROR("Unable to find font file : " + secondFile);
				return;
			}
			
			ReadFiles(fileName, secondFile);
		} else if (fileName.find(".fnt") != std::string::npos) {
			secondFile = fileName.substr(0, fileName.find_last_of(".")) + ".png";

			if (!FileExists(secondFile)) {
				LOG_ERROR("Unable to find font file : " + secondFile);
				return;
			}

			ReadFiles(secondFile, fileName);
		} else {
			LOG_ERROR("Unable to determine second file for font : " + fileName);
			return;
		}

		m_FontSize = fontSize;

		InitVertexBuffer(vertexShader);

		m_CapitalLetterHeight = m_CharacterUV[(int)'M'].w + m_CharacterPositions[(int)'M'].y;

		LOG_TRACE("Font : " + fileName + " successfully read");
	}

	void Font::Begin(int textureSlot /*= 0*/) {
		m_VertexBuffer->Bind();
		m_VertexLayout->Bind();
		m_FontTexture->Bind();
		m_FontSampler->Bind();
	}

	void Font::End() {

	}

	void Font::Print(const std::string & text, Vec2 pos) {
		if (text.size() == 0 || text.size() > MAX_STRING_LENGTH) {
			return;
		}

		m_CharacterBuffer.clear();
		m_CharacterBuffer.resize(text.size() * MAX_VERTEX_PER_CHAR);

		Vec2 cursorPos = pos;
		unsigned int vertex = 0;

		if (m_BufferIndex + text.size() > text.size())
			m_BufferIndex = 0;

		for (size_t i = 0; i < text.size(); i++) {
			int id = (int)text[i];

			if (text[i] == ' ') {
				cursorPos.x += (m_CharacterPositions[id].z * m_FontSize) / TEXT_DIVISOR;
				continue;
			}

			Vec2 charUv = m_CharacterUV[id].xy();
			Vec2 charSize = Vec2(m_CharacterUV[id].z, m_CharacterUV[id].w);
			Vec2 texSize = Vec2((float)m_FontTexture->GetDesc().TextureSize.x, (float)m_FontTexture->GetDesc().TextureSize.y);

			Vec2 uv[4] = {
				Vec2(charUv.x, charUv.y),															// TOP LEFT
				Vec2(charUv.x + (charSize.x / texSize.x), charUv.y),								// TOP RIGHT
				Vec2(charUv.x + (charSize.x / texSize.x), charUv.y + (charSize.y / texSize.y)),		// BOTTOM RIGHT
				Vec2(charUv.x, charUv.y + (charSize.y / texSize.y)),								// BOTTOM LEFT
			};

			Vec2 charOffset = m_CharacterPositions[id].xy();
			if (charOffset.y + charSize.y > m_CapitalLetterHeight) {
				charOffset.y = m_CapitalLetterHeight - (charOffset.y + charSize.y);
			} else {
				charOffset.y = 0;
			}

			charOffset *= m_FontSize / TEXT_DIVISOR;
			charSize *= m_FontSize / TEXT_DIVISOR;

			Vec2 pos[4] = {
				cursorPos + charOffset + Vec2(0, charSize.y),					// BOTTOM LEFT
				cursorPos + charOffset + Vec2(charSize.x, charSize.y),			// BOTTOM RIGHT
				cursorPos + charOffset + Vec2(charSize.x, 0),					// TOP RIGHT
				cursorPos + charOffset,											// TOP LEFT
			};

			cursorPos.x += (m_CharacterPositions[id].z * m_FontSize) / TEXT_DIVISOR;

			m_CharacterBuffer[vertex + 0].Position = pos[0];
			m_CharacterBuffer[vertex + 1].Position = pos[1];
			m_CharacterBuffer[vertex + 2].Position = pos[2];

			m_CharacterBuffer[vertex + 3].Position = pos[0];
			m_CharacterBuffer[vertex + 4].Position = pos[2];
			m_CharacterBuffer[vertex + 5].Position = pos[3];

			m_CharacterBuffer[vertex + 0].UV = uv[0];
			m_CharacterBuffer[vertex + 1].UV = uv[1];
			m_CharacterBuffer[vertex + 2].UV = uv[2];

			m_CharacterBuffer[vertex + 3].UV = uv[0];
			m_CharacterBuffer[vertex + 4].UV = uv[2];
			m_CharacterBuffer[vertex + 5].UV = uv[3];

			vertex += 6;
		}

		RenderState::Ref().SetPrimitiveTopology(PV_PRIM_TRIANGLELIST);
		m_VertexBuffer->SubData((void *)m_CharacterBuffer.data(), m_CharacterBuffer.size() * sizeof(Vertex), m_BufferIndex * sizeof(Vertex));
		m_VertexBuffer->Draw(m_CharacterBuffer.size(), m_BufferIndex);

		m_BufferIndex += m_CharacterBuffer.size();
	}

	float Font::GetCharacterWidth(char c) {
		return (m_CharacterUV[(int)c].z * m_FontSize) / TEXT_DIVISOR;
	}

	float Font::GetCharacterHeight(char c) {
		return (m_CharacterUV[(int)c].w * m_FontSize) / TEXT_DIVISOR;
	}

	void Font::InitVertexBuffer(StrongHandle<VertexShader> vertexShader) {
		m_VertexBuffer = VertexBuffer::CreateVertexBuffer();
		m_VertexBuffer->Init(nullptr, MAX_NUM_VERTICES, sizeof(Vertex), BUFFER_USAGE_DYNAMIC);

		m_VertexLayout = VertexLayout::CreateVertexLayout();
		m_VertexLayout->BeginEntries();
		m_VertexLayout->AddEntry(PV_DATA_TYPE_FLOAT_32, 2, offsetof(Vertex, Position), "FONT_POSITION", false);
		m_VertexLayout->AddEntry(PV_DATA_TYPE_FLOAT_32, 2, offsetof(Vertex, UV), "FONT_TEXTURECOORD", false);
		m_VertexLayout->EndEntries(vertexShader);
	}

	bool Font::ReadFiles(const std::string & imageFile, const std::string & fontFile) {
		m_FontTexture = Texture2D::CreateTexture2D();
		m_FontTexture->Init(imageFile, 0);

		m_FontSampler = Sampler2D::CreateSampler2D();
		Sampler2DDesc s2d;
		s2d.Filtering = PV_TEXTURE_FILTER_NEAREST;
		s2d.Wrapping = PV_TEXTURE_WRAP_DEFAULT;
		m_FontSampler->Init(s2d);

		std::ifstream inFile(fontFile);
		std::string line;

		while (std::getline(inFile, line)) {
			if (line.find("char id") == std::string::npos) continue;

			std::istringstream iss(line);
			std::string temp;

			unsigned int id = 0;
			Vec4 uv;
			Vec3 position;

			while (iss >> temp) {
				if (temp.find("id=") != std::string::npos) {
					id = std::atoi(temp.substr(temp.find_last_of("=") + 1).c_str());
				} else if (temp.find("x=") != std::string::npos) {
					uv.x = (float)std::atoi(temp.substr(temp.find_last_of("=") + 1).c_str());
				} else if (temp.find("y=") != std::string::npos) {
					uv.y = (float)std::atoi(temp.substr(temp.find_last_of("=") + 1).c_str());
				} else if (temp.find("width=") != std::string::npos) {
					uv.z = (float)std::atoi(temp.substr(temp.find_last_of("=") + 1).c_str());
				} else if (temp.find("height=") != std::string::npos) {
					uv.w = (float)std::atoi(temp.substr(temp.find_last_of("=") + 1).c_str());
				} else if (temp.find("xoffset=") != std::string::npos) {
					position.x = (float)std::atoi(temp.substr(temp.find_last_of("=") + 1).c_str());
				} else if (temp.find("yoffset=") != std::string::npos) {
					position.y = (float)std::atoi(temp.substr(temp.find_last_of("=") + 1).c_str());
				} else if (temp.find("xadvance=") != std::string::npos) {
					position.z = (float)std::atoi(temp.substr(temp.find_last_of("=") + 1).c_str());
				}
			}

			m_CharacterUV[id] = uv;
			m_CharacterPositions[id] = position;
		}

		NormalizeData(m_FontTexture->GetDesc().TextureSize);

		return true;
	}

	void Font::NormalizeData(Vec2i textureSize) {
		Vec2 fTextureSize = Vec2((float)textureSize.x, (float)textureSize.y);

		for (unsigned int i = 0; i < NUM_CHARS; i++) {
			m_CharacterUV[i].x /= fTextureSize.x;
			m_CharacterUV[i].y /= fTextureSize.y;
		}
	}

}