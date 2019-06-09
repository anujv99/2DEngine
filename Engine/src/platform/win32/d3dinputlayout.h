#pragma once

#include "d3dhelper.h"
#include "graphics/vertexlayout.h"

namespace prev {

	class D3DInputLayout : public VertexLayout {
	public:
		D3DInputLayout() {}
	protected:
		// Inherited via VertexLayout
		virtual void Bind() override;
		virtual void UnBind() override;
		virtual bool BuildVertexLayout(StrongHandle<VertexShader> vertexShader) override;
	private:
		DXGI_FORMAT GetLayoutFormat(LayoutDataType dataType, unsigned int numElements, bool normalized);
	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout = nullptr;
	};

}
