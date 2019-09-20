#include "pch.h"
#include "windowsimagecomponent.h"

#include <comutil.h>

#if defined(ENGINE_DEBUG)
	#define CHECK_HR(F, S) if (FAILED(F)) { ERROR_TRACE(ERR_WINDOWS_API_INTERNAL_ERROR, S); return false; }
#else if defined(ENGINE_RELEASE) || defined(ENGINE_DIST)
	#define CHECK_HR(F, S) (F)
#endif

namespace prev {

	using namespace Microsoft::WRL;

	ImageComponent * ImageComponent::CreateImageComponent() {
		return new WindowsImageComponent();
	}

	WindowsImageComponent::WindowsImageComponent() : m_IsInitialized(false), m_Factory(nullptr) {
		m_IsInitialized = InititalizeWIC();
		if (!m_IsInitialized) {
			LOG_ERROR("Unable to initialize windows imaging component");
		}
		return;
	}

	WindowsImageComponent::~WindowsImageComponent() {
		if (m_Thread.joinable()) {
			m_Thread.join();
		}

		if (m_ThreadGif.joinable()) {
			m_ThreadGif.join();
		}
	}

	bool WindowsImageComponent::InititalizeWIC() {
		CHECK_HR(CoInitialize(nullptr), "CoInitialize Failed");
		CHECK_HR(CoCreateInstance(CLSID_WICImagingFactory, 
			nullptr, CLSCTX_INPROC_SERVER, 
			IID_PPV_ARGS(m_Factory.GetAddressOf())), "Unable to create WIC Imaging Factory");

		return true;
	}

	void WindowsImageComponent::SaveAsImage(StrongHandle<Texture2D> texture, const std::string & fileName) {

		void * data = texture->Map();

		if (m_Thread.joinable()) {
			m_Thread.join();
		}

		m_Thread = std::thread(&WindowsImageComponent::SaveImage, this, fileName, data, texture->GetDesc().TextureSize,
			GetBytesPerPixel(texture->GetDesc().TexFormat), GetGUIDFormat(texture->GetDesc().TexFormat));

		texture->UnMap();

	}

	void WindowsImageComponent::SaveAsGIF(StrongHandle<Texture2D> texture, const std::string & fileName, unsigned int numFramesToSave, unsigned int fps) {

		if (numFramesToSave == 0)
			return;

		if (m_ThreadGif.joinable()) {
			m_ThreadGif.join();
		}

		auto it = m_Gifs.find(HashString(fileName));

		if (it != m_Gifs.end()) {
			if (it->second == nullptr) return;

			if (it->second->FrameNumber <= 0) {
				delete it->second;
				it->second = nullptr;
				return;
			}

			void * data = texture->Map();

			m_ThreadGif = std::thread(&GIFImage::AddFrame, it->second, data, texture->GetDesc().TextureSize,
				GetBytesPerPixel(texture->GetDesc().TexFormat), GetGUIDFormat(texture->GetDesc().TexFormat));

			texture->UnMap();
			return;
		}

		GIFImage * img = new GIFImage(m_Factory);
		bool stat = img->Initialize(fileName, numFramesToSave, fps);

		if (!stat)
			return;

		void * data = texture->Map();

		m_ThreadGif = std::thread(&GIFImage::AddFrame, img, data, texture->GetDesc().TextureSize,
			GetBytesPerPixel(texture->GetDesc().TexFormat), GetGUIDFormat(texture->GetDesc().TexFormat));

		texture->UnMap();

		m_Gifs.insert(std::make_pair(HashString(fileName), img));

		return;
	}

	bool WindowsImageComponent::SaveImage(const std::string & fileName, const void * data, Vec2i size, UINT bitsPerPixel, WICPixelFormatGUID formatGUID) {
		
		if (!m_IsInitialized) {
			LOG_ERROR("WIC not initialized. Unable to save image file " + fileName);
			return false;
		}

		if (data == nullptr) {
			LOG_ERROR("Inavlid data provided for file " + fileName);
			return false;
		}

		if (bitsPerPixel == 0) {
			LOG_ERROR("Invalid pixel size for file " + fileName);
			return false;
		}

		ComPtr<IWICBitmapEncoder> encoder			= nullptr;
		ComPtr<IWICBitmapFrameEncode> bitmapFrame	= nullptr;
		ComPtr<IPropertyBag2> prop					= nullptr;
		ComPtr<IWICStream> stream					= nullptr;

		WICPixelFormatGUID guid = formatGUID;

		CHECK_HR(m_Factory->CreateStream(stream.GetAddressOf()), "Unable to create IWICStream");
		CHECK_HR(stream->InitializeFromFilename(_bstr_t(fileName.c_str()), GENERIC_WRITE), "Unable to initialize IWICStream");
		CHECK_HR(m_Factory->CreateEncoder(GUID_ContainerFormatPng, nullptr, encoder.GetAddressOf()), "Unable to create IWICBitmapEncoder");
		CHECK_HR(encoder->Initialize(stream.Get(), WICBitmapEncoderNoCache), "Unable to initialize IWICBitmapEncoder");
		CHECK_HR(encoder->CreateNewFrame(bitmapFrame.GetAddressOf(), prop.GetAddressOf()), "Unable to create new frame -> IWICBitmapFrameEncode");
		CHECK_HR(bitmapFrame->Initialize(prop.Get()), "Unable to initialize IWICBitmapFrameEncode");
		CHECK_HR(bitmapFrame->SetSize(size.x, size.y), "Unable to set size for IWICBitmapFrameEncode");
		CHECK_HR(bitmapFrame->SetPixelFormat(&guid), "Unable to set pixel format for IWICBitmapFrameEncode");
		CHECK_HR(IsEqualGUID(guid, formatGUID), "Pixel format not set properly");

		UINT stride = (size.x * bitsPerPixel + 7) / 8;
		UINT bufferSize = size.y * stride;

		BYTE * newData = new BYTE[bufferSize];

		memcpy_s(newData, bufferSize, data, bufferSize);

		for (unsigned int i = 0; i < bufferSize; i += 4) {
			BYTE temp = newData[i];
			newData[i] = newData[i + 2];
			newData[i + 2] = temp;
		}

		CHECK_HR(bitmapFrame->WritePixels(size.y, stride, bufferSize, newData), "Unable to write pixels to file " + fileName);
		CHECK_HR(bitmapFrame->Commit(), "Unable to commit bitman frame IWICBitmapFrameEncode");
		CHECK_HR(encoder->Commit(), "Unable to commit encoder IWICBitmapEncoder");

		LOG_INFO("Image saved successfully as file {}", fileName);

		delete[] newData;

		return true;
	}

	UINT WindowsImageComponent::GetBytesPerPixel(TextureFormat texFormat) {
		switch (texFormat) {
		case prev::PV_TEXTURE_FORMAT_RGBA8:
			return 32u;
		case prev::PV_TEXTURE_FORMAT_R32F:
		case prev::PV_TEXTURE_FORMAT_R32SINT:
		case prev::PV_TEXTURE_FORMAT_UNKNOWN:
		default:
			return 0u;
		}
	}

	WICPixelFormatGUID WindowsImageComponent::GetGUIDFormat(TextureFormat texFormat) {
		switch (texFormat) {
		case prev::PV_TEXTURE_FORMAT_RGBA8:
			return GUID_WICPixelFormat32bppRGBA;
		case prev::PV_TEXTURE_FORMAT_R32F:
		case prev::PV_TEXTURE_FORMAT_R32SINT:
		case prev::PV_TEXTURE_FORMAT_UNKNOWN:
		default:
			return GUID_WICPixelFormatDontCare;
		}
	}

	bool WindowsImageComponent::GIFImage::Initialize(const std::string & fileName, UINT numFrames, UINT fps) {
		CHECK_HR(Factory->CreateStream(Stream.GetAddressOf()), "Unable to create IWICStream");
		CHECK_HR(Stream->InitializeFromFilename(_bstr_t(fileName.c_str()), GENERIC_WRITE), "Unable to initialize IWICStream");
		CHECK_HR(Factory->CreateEncoder(GUID_ContainerFormatGif, nullptr, Encoder.GetAddressOf()), "Unable to create IWICBitmapEncoder");
		CHECK_HR(Encoder->Initialize(Stream.Get(), WICBitmapEncoderNoCache), "Unable to initialize IWICBitmapEncoder");
		CHECK_HR(Encoder->GetMetadataQueryWriter(MetaData.GetAddressOf()), "Unable to get metadata for IWICBitmapEncoder");
		CHECK_HR(Factory->CreatePalette(Palette.GetAddressOf()), "Unable to create palette IWICPalette");
		CHECK_HR(Palette->InitializePredefined(WICBitmapPaletteTypeFixedGray256, TRUE), "Unable to initialize palette IWICPalette");

		Fps = fps;
		FrameNumber = numFrames;

		PROPVARIANT val;
		PropVariantInit(&val);

		UCHAR data[] = {
			3, 1, 0, 0, 0
		};

		val.vt = VT_UI1 | VT_VECTOR;
		val.caub.cElems = std::size(data);
		val.caub.pElems = data;

		CHECK_HR(MetaData->SetMetadataByName(L"/appext/Data", &val), "Unable to set metadata appext Data for file : " + fileName);

		PropVariantInit(&val);

		UCHAR data2[] = "NETSCAPE2.0";

		val.vt = VT_UI1 | VT_VECTOR;
		val.caub.cElems = 11;
		val.caub.pElems = data2;

		CHECK_HR(MetaData->SetMetadataByName(L"/appext/Application", &val), "Unable to set metadata appext Application for file : " + fileName);

		return true;
	}

	bool WindowsImageComponent::GIFImage::AddFrame(const void * data, Vec2i size, UINT bitsPerPixel, WICPixelFormatGUID formatGUID) {

		formatGUID = GUID_WICPixelFormat8bppIndexed;
		bitsPerPixel = 8;

		if (FrameNumber != 0) {
			FrameNumber--;
		} else {
			return false;
		}

		if (data == nullptr) {
			LOG_ERROR("Inavlid data provided for gif");
			return false;
		}

		if (bitsPerPixel == 0) {
			LOG_ERROR("Invalid pixel size for gif ");
			return false;
		}

		ComPtr<IWICBitmapFrameEncode> bitmapFrame = nullptr;
		ComPtr<IPropertyBag2> prop = nullptr;
		ComPtr<IWICMetadataQueryWriter> frameMeta = nullptr;

		WICPixelFormatGUID guid = formatGUID;

		CHECK_HR(Encoder->CreateNewFrame(bitmapFrame.GetAddressOf(), prop.GetAddressOf()), "Unable to create new frame -> IWICBitmapFrameEncode");
		CHECK_HR(bitmapFrame->Initialize(prop.Get()), "Unable to initialize IWICBitmapFrameEncode");
		CHECK_HR(bitmapFrame->GetMetadataQueryWriter(frameMeta.GetAddressOf()), "Unable to get metadata for IWICBitmapFrameEncode");

		PROPVARIANT frameVar;
		PropVariantInit(&frameVar);

		frameVar.vt = VT_UI2;
		frameVar.uintVal = ((UINT)(1.0f / (float)Fps) * 100u);

		CHECK_HR(frameMeta->SetMetadataByName(L"/grctlext/Delay", &frameVar), "Unable to set metadata grctlext Delay for GIF");

		CHECK_HR(bitmapFrame->SetSize(size.x, size.y), "Unable to set size for IWICBitmapFrameEncode");
		CHECK_HR(bitmapFrame->SetPixelFormat(&guid), "Unable to set pixel format for IWICBitmapFrameEncode");
		CHECK_HR(IsEqualGUID(guid, formatGUID), "Pixel format not set properly");

		UINT stride = (size.x * bitsPerPixel + 7) / 8;
		UINT bufferSize = size.y * stride;

		BYTE * newData = new BYTE[bufferSize];
		BYTE * temp = (BYTE *)data;

		WICColor colors[256] = { 0 };
		UINT colIndex = 0u;
		std::unordered_map<uint32_t, UINT> colorMap;

		for (unsigned int i = 0, j = 0; i < bufferSize; i++, j += 4) {

			UINT col = temp[j] + temp[j + 1] + temp[j + 2];

			newData[i] = (BYTE)(col / 3u);
		}

		ComPtr<IWICPalette> palette;
		CHECK_HR(Factory->CreatePalette(palette.GetAddressOf()), "Unable to create IWICPalette");
		//CHECK_HR(palette->InitializeCustom(colors, 256), "Unable to initialize IWICPalette");
		CHECK_HR(palette->InitializePredefined(WICBitmapPaletteTypeFixedGray256, FALSE), "Unable to initialize IWICPalette");
		CHECK_HR(bitmapFrame->SetPalette(palette.Get()), "Unable to set palette for IWICBitmapFrameEncode");

		CHECK_HR(bitmapFrame->WritePixels(size.y, stride, bufferSize, newData), "Unable to write pixels to GIF");
		CHECK_HR(bitmapFrame->Commit(), "Unable to commit bitman frame IWICBitmapFrameEncode");

		delete[] newData;

		return true;
	}

	WindowsImageComponent::GIFImage::~GIFImage() {
		Encoder->Commit();
	}

}