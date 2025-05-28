#include "image.h"
#include "filesystem/engine_io.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_STDIO
#include <stb_image.h>
#undef STBI_NO_STDIO
#undef STB_IMAGE_IMPLEMENTATION

using namespace resources;

void Image::_register_type() {
	using namespace engine_type_registry;
	using namespace ObjectRTTIModel;

	type_registry::register_new_class("Image", "Resource");
	type_registry::end_class();
}

Image* Image::CreateFromFile(string filePath) {
	EngineIO::File file = EngineIO::FileSystem::OpenFile(filePath, std::ios::in | std::ios::binary);
	vector<uint8_t> fileData = file.ReadAllBinary();
	Image* im = new Image();

	int32_t bitsPerPixel = 0;
	bool success = stbi_info_from_memory(fileData.data(), static_cast<int32_t>(fileData.size()), &im->_width, &im->_height, &im->_channels);
	if (!success) {
		Log.Error("Image", "Failed to create image from file '" + filePath + "'");
		delete im;
		return nullptr;
	}

	bitsPerPixel = stbi_is_16_bit_from_memory(fileData.data(), static_cast<int32_t>(fileData.size())) ? 16 : 8;
	switch (im->_channels) {
		case 1:
			im->_format = bitsPerPixel == 16 ? ImageFormat::FORMAT_16L : ImageFormat::FORMAT_8L;
			break;
		case 2: 
			im->_format = bitsPerPixel == 16 ? ImageFormat::FORMAT_16LA : ImageFormat::FORMAT_8LA;
			break;
		case 3:
			im->_format = bitsPerPixel == 16 ? ImageFormat::FORMAT_16RGB : ImageFormat::FORMAT_8RGB;
			break;
		case 4:
			im->_format = bitsPerPixel == 16 ? ImageFormat::FORMAT_16RGBA : ImageFormat::FORMAT_8RGBA;
			break;
	}

	unsigned char* pixels = stbi_load_from_memory(fileData.data(), static_cast<int32_t>(fileData.size()), &im->_width, &im->_height, &im->_channels, 0);
	if (!pixels) {
		string errMsg = string(stbi_failure_reason());
		Log.Error("Image", "Failed to create image from file '" + filePath + "' -" + errMsg);
		delete im;
		return nullptr;
	}

	int32_t imageSize = im->_width * im->_height * im->_channels;
	im->_pixelData.resize(imageSize);
	memcpy(im->_pixelData.data(), pixels, imageSize);

	stbi_image_free(pixels);

	return im;
}

Image::~Image()
{
}
