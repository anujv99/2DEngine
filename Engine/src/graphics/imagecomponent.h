#pragma once

#include "texture2d.h"

namespace prev {

	class ImageComponent : public Singleton<ImageComponent> {
		friend class Singleton<ImageComponent>;
	protected:
		ImageComponent() {}
		virtual ~ImageComponent() {}
	public:
		virtual void SaveAsImage(StrongHandle<Texture2D> texture, const std::string & fileName) = 0;

		//Only save greyscale images for now
		virtual void SaveAsGIF(StrongHandle<Texture2D> texture, const std::string & fileName, unsigned int numFramesToSave, unsigned int fps) = 0;
	private:
		static ImageComponent * CreateImageComponent();
	};

	template<>
	template<>
	inline void Singleton<ImageComponent>::CreateInst() {
		if (!s_Instance) s_Instance = ImageComponent::CreateImageComponent();
	}

}
