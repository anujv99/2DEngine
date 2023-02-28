#include "pch.h"
#include "immediategfx.h"

static const int IMMEDIATEGFX_MAX_VERTS = 8192;

namespace prev {

	void ImmediateGFX::BeginDefaultShaders(bool bindPixelShader) {
		m_VertexShader->Bind();
		if (bindPixelShader) {
			m_PixelShader->Bind();
		}
		m_VertexShader->UpdateMVP();
	}

	void ImmediateGFX::EndDefaultShaders() {
	}

	void ImmediateGFX::BeginDraw() {
		m_VertexBuffer->Bind();
		m_VertexLayout->Bind();
	}

	void ImmediateGFX::EndDraw() {
	}

	void ImmediateGFX::PolygonBegin(PrimitiveTopology primitive) {
		m_IsBuilding = true;
		m_DrawPrimitive = primitive;
		GetVertexBatch().clear();
	}

	void ImmediateGFX::PolygonEnd(bool bindPixelShader) {
		m_IsBuilding = false;
		DrawCurrentPrimitive(bindPixelShader);
	}

	ImmediateGFX::ImmediateGFX() : VertexBatcher(IMMEDIATEGFX_MAX_VERTS), m_IsBuilding(false) {
		LOG_TRACE("Immediate GFX initializing");

		LOG_TRACE("initializing Vertex Buffer");
		InitializeVertexBuffer();

		LOG_TRACE("initializing Shaders");
		m_VertexShader = ShaderManager::Ref().LoadVertexShaderFromFile(
			"IMMGFX_DEFAULT_VERTEX_SHADER", 
			"res/shaders/immGFXDefaultVertex.hlsl"
		);

		m_PixelShader = ShaderManager::Ref().LoadPixelShaderFromFile(
			"IMMGFX_DEFAULT_PIXEL_SHADER", 
			"res/shaders/immGFXDefaultPixel.hlsl"
		);

		LOG_TRACE("Creating Verex Layout");
		m_VertexLayout = CreateVertexLayout(m_VertexShader);

		LOG_TRACE("Immediate GFX initialized");
	}

	ImmediateGFX::~ImmediateGFX() {

	}

	void ImmediateGFX::InitializeVertexBuffer() {
		m_BufferIndex = 0u;

		m_VertexBuffer = VertexBuffer::CreateVertexBuffer();
		m_VertexBuffer->Init(nullptr, IMMEDIATEGFX_MAX_VERTS, GetNumBytesPerVertex(), BUFFER_USAGE_STATIC);
	}

	void ImmediateGFX::DrawCurrentPrimitive(bool bindPixelShader) {
		
		BeginDefaultShaders(bindPixelShader);
		BeginDraw();

		if (m_BufferIndex + GetVertexBatch().size() >= IMMEDIATEGFX_MAX_VERTS) {
			m_BufferIndex = 0;
		}

		if (!GetVertexBatch().empty()) {
			m_VertexBuffer->SubData(
				&GetVertexBatch().front(),
				(unsigned int)GetVertexBatch().size() * GetNumBytesPerVertex(),
				m_BufferIndex * GetNumBytesPerVertex()
			);

			RenderState::Ref().SetPrimitiveTopology(m_DrawPrimitive);
			m_VertexBuffer->Draw((unsigned int)GetVertexBatch().size(), m_BufferIndex);
			m_BufferIndex += (unsigned int)GetVertexBatch().size();
		}
	}

}