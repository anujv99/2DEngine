
#include "pch.h"
#include "bloom.h"

#include "common/log.h"
#include "renderer/fbopass.h"
#include "graphics/renderstate.h"
#include <imgui.h>

namespace prev {

  static constexpr const char BLOOM_VERTEX_SHADER[]          = "res/shaders/bloomVertex.hlsl";
  static constexpr const char BLOOM_PREFILTER_PIXEL_SHADER[] = "res/shaders/bloomPrefilter.hlsl";
  static constexpr const char BLOOM_BLUR_PIXEL_SHADER[]      = "res/shaders/bloomBlurPixel.hlsl";
  static constexpr const char BLOOM_FINAL_PIXEL_SHADER[]     = "res/shaders/bloomFinalPixel.hlsl";
  static constexpr const char BLOOM_TO_SCREEN_PIXEL_SHADER[] = "res/shaders/bloomToScreenPixel.hlsl";

  static constexpr const char DITHER_TEXTURE[] = "res/textures/dither.png";

  static float BLOOM_THRESHOLD = 0.6f;
  static float BLOOM_SOFT_KNEE = 0.7f;
  static float BLOOM_INTENSITY = 0.0f;

  BloomPass::BloomPass(Vec2 screenSize) {
    Vec2 res = m_BloomResolution;

    m_BloomFramebuffer = Framebuffer::CreateFramebuffer();
    m_BloomFramebuffer->Init(res, TextureFormat::PV_TEXTURE_FORMAT_RGBA8, FRAMEBUFFER_DISABLE_MSAA);

    m_IntermediateBuffer = Framebuffer::CreateFramebuffer();
    m_IntermediateBuffer->Init(screenSize, TextureFormat::PV_TEXTURE_FORMAT_RGBA8, FRAMEBUFFER_DISABLE_MSAA);

    for (int i = 0; i < m_BloomIterations; i++) {
      res.x = (int)res.x >> 1;
      res.y = (int)res.y >> 1;

      if ( res.x < 2.0f || res.y < 2.0f ) break;

      StrongHandle<Framebuffer> fbo = Framebuffer::CreateFramebuffer();
      fbo->Init(res, TextureFormat::PV_TEXTURE_FORMAT_RGBA8, FRAMEBUFFER_DISABLE_MSAA);

      m_IterationsFramebuffers.push_back(fbo);
    }

    m_BloomVert = ShaderManager::Ref().LoadVertexShaderFromFile("BLOOM_VERTEX_SHADER", BLOOM_VERTEX_SHADER);
    m_BloomPrefilter = ShaderManager::Ref().LoadPixelShaderFromFile("BLOOM_PREFILTER_PIXEL_SHADER", BLOOM_PREFILTER_PIXEL_SHADER);
    m_BloomBlur = ShaderManager::Ref().LoadPixelShaderFromFile("BLOOM_BLUR_PIXEL_SHADER", BLOOM_BLUR_PIXEL_SHADER);
    m_BloomFinal = ShaderManager::Ref().LoadPixelShaderFromFile("BLOOM_FINAL_PIXEL_SHADER", BLOOM_FINAL_PIXEL_SHADER);
    m_BloomToScreen = ShaderManager::Ref().LoadPixelShaderFromFile("BLOOM_TO_SCREEN_PIXEL_SHADER", BLOOM_TO_SCREEN_PIXEL_SHADER);

    m_DitherTex = Texture2D::CreateTexture2D();
    m_DitherTex->Init( DITHER_TEXTURE, { PV_TEXTURE_WRAP_DEFAULT, PV_TEXTURE_FILTER_LINEAR }, 2);

    LOG_INFO("Created bloom postprocessing pass");
  }

  BloomPass::~BloomPass() {}

  void BloomPass::Pass(StrongHandle<Framebuffer> source, StrongHandle<Framebuffer> destination) {
    if (m_BloomIterations < 2 || destination == nullptr || source == nullptr)
      return;

    StrongHandle<Framebuffer> last = m_IntermediateBuffer;

    Viewport vp = RenderState::Ref().GetViewport();
    Viewport temp;
    temp.TopLeft = Vec2(0.0f);
    temp.DepthValues = Vec2(0.0f, 1.0f);

    // bloom prefilter
    {
      float knee = BLOOM_THRESHOLD * BLOOM_SOFT_KNEE + 0.0001f;
      Vec3 curve(BLOOM_THRESHOLD - knee,
                 knee* 2,
                 0.25f / knee);
  
      Vec4 texelSize(0.0f);
      Vec4 PData(curve, BLOOM_THRESHOLD);
  
      m_BloomVert->SetUniform("VData", &texelSize, sizeof(texelSize)); // texelSize is unused in bloomPrefilter pass
      m_BloomPrefilter->SetUniform("PData", &PData, sizeof(PData));
  
      temp.Dimension = last->GetSize();
      RenderState::Ref().SetViewport(temp);
      last->Bind();
      FramebufferPass::Ref().Pass(source, m_BloomVert, m_BloomPrefilter);
    }

    // bloom blur
    {
      for (auto& dest : m_IterationsFramebuffers) {
        Vec4 texelSize(1.0f / last->GetSize().x, 1.0f / last->GetSize().y, 0.0f, 0.0f);
        m_BloomVert->SetUniform("VData", &texelSize, sizeof(texelSize));

        temp.Dimension = dest->GetSize();
        RenderState::Ref().SetViewport(temp);
        dest->Bind();
        FramebufferPass::Ref().Pass(last, m_BloomVert, m_BloomBlur);
        last = dest;
      }

      BlendFunction blendFunc;
      blendFunc.SrcBlend = PV_BLEND_ONE;
      blendFunc.DestBlend = PV_BLEND_ONE;
      blendFunc.Operation = PV_BLEND_OP_ADD;

      for (int i = (int)m_IterationsFramebuffers.size() - 2; i >= 0; i--) {
        StrongHandle<Framebuffer> baseTex = m_IterationsFramebuffers[i];
        Vec4 texelSize(1.0f / last->GetSize().x, 1.0f / last->GetSize().y, 0.0f, 0.0f);
        m_BloomVert->SetUniform("VData", &texelSize, sizeof(texelSize));

        temp.Dimension = baseTex->GetSize();
        RenderState::Ref().SetViewport(temp);
        baseTex->Bind();
        FramebufferPass::Ref().Pass(last, blendFunc, m_BloomVert, m_BloomBlur);
        last = baseTex;
      }

    }

    // bloom final pass
    {
      Vec4 texelSize(1.0f / last->GetSize().x, 1.0f / last->GetSize().y, 0.0f, 0.0f);
      Vec4 intensity(BLOOM_INTENSITY);
      m_BloomVert->SetUniform("VData", &texelSize, sizeof(texelSize));
      m_BloomFinal->SetUniform("PData", &intensity, sizeof(intensity));

      temp.Dimension = m_IntermediateBuffer->GetSize();
      RenderState::Ref().SetViewport(temp);
      m_IntermediateBuffer->Bind();
      FramebufferPass::Ref().Pass(last, m_BloomVert, m_BloomFinal);
      last = m_IntermediateBuffer;
    }

    // blit to screen
    {
      Vec4 texelSize(1.0f / last->GetSize().x, 1.0f / last->GetSize().y, 0.0f, 0.0f);
      Vec4 ditherTexel(ToVec2(m_DitherTex->GetDesc().TextureSize), texelSize.xy());
      m_BloomVert->SetUniform("VData", &texelSize, sizeof(texelSize));
      m_BloomToScreen->SetUniform("PData", &ditherTexel, sizeof(ditherTexel));

      StrongHandle<Texture2D> sourceTex = source->GetTexture();

      sourceTex->SetTextureSlot(1);
      sourceTex->Bind();
      m_DitherTex->Bind();

      temp.Dimension = destination->GetSize();
      RenderState::Ref().SetViewport(temp);
      destination->Bind();
      FramebufferPass::Ref().Pass(last, m_BloomVert, m_BloomToScreen);
    }

    RenderState::Ref().SetViewport(vp);

    destination->UnBind();
  }

  void BloomPass::Gui() {
    ImGui::Begin("Bloom Props");
    ImGui::DragFloat("BLOOM_THRESHOLD", &BLOOM_THRESHOLD, 0.01f);
    ImGui::DragFloat("BLOOM_SOFT_KNEE", &BLOOM_SOFT_KNEE, 0.01f);
    ImGui::DragFloat("BLOOM_INTENSITY", &BLOOM_INTENSITY, 0.01f);
    ImGui::End();
  }

}
