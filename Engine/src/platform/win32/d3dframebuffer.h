#pragma once

#include "d3dhelper.h"
#include "d3dtexture2d.h"

#include "graphics/framebuffer.h"

namespace prev {

	class D3DFramebuffer : public Framebuffer {
	public:
		D3DFramebuffer() : m_IsCreated(false), m_ISMultisampled(false), m_TextureSize(0.0f) {}
		virtual ~D3DFramebuffer();

		// Inherited via Framebuffer
		virtual void Bind() override;
		virtual void UnBind() override;
		virtual void Init(Vec2 size, TextureFormat format, bool msaa) override;
		virtual void Clear() override;
		virtual void Clear(Vec4 color) override;
		virtual Vec2 GetSize() override;
		virtual StrongHandle<Texture2D> GetTexture() override;
	private:
		void ResolveTexture();
		bool CreateFramebufferMSAA(Vec2 size, unsigned int numSamples);
		bool CreateFramebufferNoMSAA(Vec2 size);
	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_MultisampledTexture;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
		
		StrongHandle<D3DTexture2D> m_AntialiasedTexture;

		Vec2 m_TextureSize;
		bool m_IsCreated;
		bool m_ISMultisampled;
		DXGI_FORMAT m_TextureFormat;
	};

}
