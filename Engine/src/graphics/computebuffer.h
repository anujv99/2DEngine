#pragma once

#include "bindable.h"

namespace prev {

	class ComputeBuffer : public HandledObject<ComputeBuffer>, Bindable {
	protected:
		ComputeBuffer() {}
	public:
		virtual ~ComputeBuffer() {}

		virtual void Bind() override = 0;
		virtual void UnBind() override = 0;
		virtual void Init(const void * data, unsigned int numElements, unsigned int strideBytes) = 0;
		virtual void SubData(const void * data, unsigned int numBytes, unsigned int byteOffset) = 0;
		virtual void * Map() = 0;
		virtual void UnMap() = 0;
		virtual void SetBindSlot(unsigned int bindSlot) = 0;
		virtual unsigned int GetBindSlot() const = 0;

		virtual BindableType GetType() const override {
			return BindableType::COMPUTE_BUFFER;
		}

		static StrongHandle<ComputeBuffer> CreateComputeBuffer();

	};

}
