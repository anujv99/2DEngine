#include "pch.h"
#include "immediategfx.h"

#include "platform/win32/d3dhelper.h"

static const int IMMEDIATEGFX_MAX_VERTS = 8192;

namespace prev {

	void ImmediateGFX::BeginDefaultShaders() {

	}

	void ImmediateGFX::EndDefaultShaders() {

	}

	void ImmediateGFX::BeginDraw() {

	}

	void ImmediateGFX::EndDraw() {

	}

	void ImmediateGFX::PolygonBegin(PrimitiveTopology primitive) {
		m_IsBuilding = true;
		m_DrawPrimitive = primitive;
		GetVertexBatch().clear();
	}

	void ImmediateGFX::PolygonEnd() {
		m_IsBuilding = false;

		m_VertexBuffer->Bind();
		m_VertexShader->Bind();
		m_VertexLayout->Bind();
		m_PixelShader->Bind();

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
			GetDeviceContext()->Draw(GetVertexBatch().size(), m_BufferIndex);
			m_BufferIndex += GetVertexBatch().size();
		}
	}

	ImmediateGFX::ImmediateGFX() : VertexBatcher(IMMEDIATEGFX_MAX_VERTS), m_IsBuilding(false) {
		LOG_TRACE("Immediate GFX initializing");

		LOG_TRACE("\tinitializing Vertex Buffer");
		InitializeVertexBuffer();

		LOG_TRACE("\tinitializing Shaders");
		m_VertexShader = ShaderManager::Ref().LoadVertexShaderFromFile(
			"IMMGFX_DEFAULT_VERTEX_SHADER", 
			"../Engine/res/shaders/immGFXDefaultVertex.hlsl"
		);

		m_PixelShader = ShaderManager::Ref().LoadPixelShaderFromFile(
			"IMMGFX_DEFAULT_PIXEL_SHADER", 
			"../Engine/res/shaders/immGFXDefaultPixel.hlsl"
		);

		LOG_TRACE("\tCreating Verex Layout");
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

}