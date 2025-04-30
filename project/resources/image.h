#pragma once
#include "core/types/resource.h"
#include <vector>
namespace resources {
	class Image : public Resource {
		GUS_DECLARE_CLASS(Image, Resource)
		
		private:

		vector<unsigned char> _pixelData {};
		public:

		enum class ImageFormat {
			FORMAT_ERR,

			FORMAT_8L,
			FORMAT_8LA,
			FORMAT_8RGB,
			FORMAT_8RGBA,
			FORMAT_16L,
			FORMAT_16LA,
			FORMAT_16RGB,
			FORMAT_16RGBA
		};

		ImageFormat format = ImageFormat::FORMAT_ERR;
		static Image* CreateFromFile(string filePath);


		~Image();
	};

}