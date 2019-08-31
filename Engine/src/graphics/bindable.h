#pragma once

namespace prev {

	enum BindableType {
		//Buffers
		VERTEX_BUFFER,
		COMPUTE_BUFFER,
		UNIFORM,

		//Shaders
		VERTEX_SHADER,
		PIXEL_SHADER,
		COMPUTE_SHADER,
		
		VERTEX_LAYOUT, //INPUT_LAYOUT
		
		TEXTURE_2D,
		FRAMEBUFFER,
	};

	class Bindable {
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;
		virtual BindableType GetType() const = 0;
	};

}
