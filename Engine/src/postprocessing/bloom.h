#pragma once

#include <vector>

#include "common/singleton.h"
#include "common/stronghandle.h"

#include "graphics/framebuffer.h"
#include "graphics/vertexbuffer.h"
#include "graphics/shadermanager.h"
#include "graphics/vertexlayout.h"

namespace prev {

  class BloomPass : public Singleton<BloomPass> {
    friend class Singleton<BloomPass>;
  private:
    BloomPass(Vec2 screenSize);
    ~BloomPass();
  public:
    void Pass(StrongHandle<Framebuffer> source, StrongHandle<Framebuffer> destination);
    void Gui();
  private:
    Vec2 m_BloomResolution = { 1024.0f, 1024.0f };
    int  m_BloomIterations = 16;

    StrongHandle<VertexShader> m_BloomVert;
    StrongHandle<PixelShader>  m_BloomPrefilter;
    StrongHandle<PixelShader>  m_BloomBlur;
    StrongHandle<PixelShader>  m_BloomFinal;
    StrongHandle<PixelShader>  m_BloomToScreen;

    StrongHandle<Texture2D>    m_DitherTex;

    StrongHandle<Framebuffer>  m_IntermediateBuffer;
    StrongHandle<Framebuffer>  m_BloomFramebuffer;
    std::vector<StrongHandle<Framebuffer>> m_IterationsFramebuffers;
  };

}
