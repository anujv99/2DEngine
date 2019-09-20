#include "pch.h"
#include "fbopass.h"
#include "graphics/renderstate.h"

namespace prev {

	static constexpr const char DEFAULT_FBO_VERTEX_SHADER[] = "res/shaders/fboDefaultVertex.hlsl";
	static constexpr const char DEFAULT_FBO_PIXEL_SHADER[]	= "res/shaders/fboDefaultPixel.hlsl";

	FramebufferPass::FramebufferPass() {
		LoadShaders();
		CreateVertexBuffer();
		CreateVertexLayout();
	}

	FramebufferPass::~FramebufferPass() {

	}

	void FramebufferPass::LoadShaders() {
		m_DefaultFBOVertexShader = ShaderManager::Ref().LoadVertexShaderFromFile("DEFAULT_FBO_VERTEX_SHADER", DEFAULT_FBO_VERTEX_SHADER);
		m_DefaultFBOPixelShader = ShaderManager::Ref().LoadPixelShaderFromFile("DEFAULT_FBO_PIXEL_SHADER", DEFAULT_FBO_PIXEL_SHADER);
	}

	void FramebufferPass::CreateVertexBuffer() {
		static const float vertices[] = {
		//	Vertices	 TextureCoords
			-1.0f,  1.0f, 0.0f, 0.0f,		//TopLeft
			 1.0f, -1.0f, 1.0f, 1.0f,		//BottomRight
			-1.0f, -1.0f, 0.0f, 1.0f,		//BottomLeft

			-1.0f,  1.0f, 0.0f, 0.0f,		//TopLeft
			 1.0f,  1.0f, 1.0f, 0.0f,		//TopRight
			 1.0f, -1.0f, 1.0f, 1.0f,		//BottomRight
		};

		m_VertexBuffer = VertexBuffer::CreateVertexBuffer();
		m_VertexBuffer->Init((void *)vertices, 6, sizeof(float) * 4u, BUFFER_USAGE_STATIC);
	}

	void FramebufferPass::CreateVertexLayout() {
		m_VertexLayout = VertexLayout::CreateVertexLayout();
		m_VertexLayout->BeginEntries();
		m_VertexLayout->AddEntry(PV_DATA_TYPE_FLOAT_32, 2, 0, "POSITION", false);
		m_VertexLayout->AddEntry(PV_DATA_TYPE_FLOAT_32, 2, sizeof(float) * 2u, "TEXCOORDS", false);
		m_VertexLayout->EndEntries(m_DefaultFBOVertexShader);
	}

	void FramebufferPass::Pass(StrongHandle<Framebuffer> fbo, StrongHandle<VertexShader> vShader, StrongHandle<PixelShader> pShader) {
		if (fbo == nullptr) return;
		if (vShader == nullptr) vShader = m_DefaultFBOVertexShader;
		if (pShader == nullptr) pShader = m_DefaultFBOPixelShader;

		fbo->GetTexture()->SetTextureSlot(0);
		fbo->GetTexture()->Bind();
		m_VertexLayout->Bind();
		m_VertexBuffer->Bind();
		vShader->Bind();
		pShader->Bind();

		BlendFunction bf = RenderState::Ref().GetBlendFunction();
		BlendFunction nbf;
		nbf.DestBlend = PV_BLEND_ZERO;
		nbf.SrcBlend = PV_BLEND_ONE;
		nbf.Operation = PV_BLEND_OP_ADD;

		RenderState::Ref().SetBlendFunction(nbf);

		m_VertexBuffer->Draw(6, 0);

		RenderState::Ref().SetBlendFunction(bf);
	}

}