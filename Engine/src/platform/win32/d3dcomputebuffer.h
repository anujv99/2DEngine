#pragma once

#include "d3dhelper.h"

#include "graphics/computebuffer.h"

namespace prev {

	class D3DComputeBuffer : public ComputeBuffer {
	public:
		D3DComputeBuffer();

		// Inherited via ComputeBuffer
		virtual void Bind() override;
		virtual void UnBind() override;
		virtual void BindToPixelShader(unsigned int slot) override;
		virtual void Init(const void * data, unsigned int numElements, unsigned int strideBytes) override;
		virtual void SubData(const void * data, unsigned int numBytes, unsigned int byteOffset) override;
		virtual void * Map() override;
		virtual void UnMap() override;

		inline virtual void SetBindSlot(unsigned int bindSlot) override { m_BindSlot = bindSlot; }
		inline virtual unsigned int GetBindSlot() const override { return m_BindSlot; }
	private:
		bool CreateBuffer(const void * data);
		bool CreateShaderResource();
	private:
		UINT m_StrideBytes;
		UINT m_NumElements;
		UINT m_TotalBytes;
		UINT m_BindSlot;
		bool m_IsInitialized;
		bool m_IsMapped;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_UnorderedView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ShaderResource;
		D3D11_MAPPED_SUBRESOURCE m_MappedBuffer;
	};

}
