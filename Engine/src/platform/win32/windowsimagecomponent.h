#pragma once

#include <Windows.h>
#include <wincodec.h>
#include <wrl.h>

#include "graphics/imagecomponent.h"

namespace prev {

	class WindowsImageComponent : public ImageComponent {
	public:
		WindowsImageComponent();
		~WindowsImageComponent();
	private:
		// Inherited via ImageComponent
		virtual void SaveAsImage(StrongHandle<Texture2D> texture, const std::string & fileName) override;
		virtual void SaveAsGIF(StrongHandle<Texture2D> texture, const std::string & fileName, unsigned int numFramesToSave, unsigned int fps) override;

		bool InititalizeWIC();
		bool SaveImage(const std::string & fileName, const void * data, Vec2i size, UINT bitsPerPixel, WICPixelFormatGUID formatGUID);

		UINT GetBytesPerPixel(TextureFormat texFormat);
		WICPixelFormatGUID GetGUIDFormat(TextureFormat texFormat);
	private:
		bool m_IsInitialized;
		std::thread m_Thread;
		std::thread m_ThreadGif;

		struct GIFImage {
			GIFImage(Microsoft::WRL::ComPtr<IWICImagingFactory> factory) : Factory(factory) {}
			~GIFImage();

			bool Initialize(const std::string & fileName, UINT numFrames, UINT fps);
			bool AddFrame(const void * data, Vec2i size, UINT bitsPerPixel, WICPixelFormatGUID formatGUID);

			Microsoft::WRL::ComPtr<IWICBitmapEncoder> Encoder			= nullptr;
			Microsoft::WRL::ComPtr<IWICPalette> Palette					= nullptr;
			Microsoft::WRL::ComPtr<IWICStream> Stream					= nullptr;
			Microsoft::WRL::ComPtr<IWICMetadataQueryWriter> MetaData	= nullptr;
			Microsoft::WRL::ComPtr<IWICImagingFactory> Factory			= nullptr;

			UINT Fps			= 0u;
			UINT FrameNumber	= 0u;
		};
	private:
		Microsoft::WRL::ComPtr<IWICImagingFactory> m_Factory;
		std::unordered_map<uint32_t, GIFImage *> m_Gifs;
	};

}
