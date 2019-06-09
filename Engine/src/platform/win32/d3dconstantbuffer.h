#pragma once

#include "d3dhelper.h"

#include "graphics/uniform.h"

namespace prev {

	class D3DConstantBuffer : public Uniform {
	public:
		D3DConstantBuffer() : m_IsCreated(false), m_ShaderType(BindableType::VERTEX_SHADER), m_BufferSlot(0u), m_DataSize(0) {}

		// Inherited via Uniform
		virtual void Bind() override;
		virtual void UnBind() override;
		virtual void Init(const void * data, unsigned int dataBytes, unsigned int uniformLocation, BindableType shaderType) override;
		virtual void Update(const void * data, size_t dataSize) override;
		virtual void ChangeBindingShader(BindableType shaderType);
	private:
		bool CreateAndUpdateBuffer(const void * data, unsigned int dataBytes);
	private:
		BindableType m_ShaderType;
		bool m_IsCreated;
		unsigned int m_BufferSlot;
		size_t m_DataSize;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
	};

}