// core
#include <log/wrLogOutput.hpp>
// data system
#include <file/image/wrReadImage.hpp>
// std
#include <cstring>
#include <format>

namespace wr
{
	static bool decoder_info(const ImageFile& file, Image& image)
	{
		// bmp color space
		uint16_t bpp;
		// bmp 
		uint32_t w;
		uint32_t h;
		// bmp header
		byte_t headers[54];
		//
		size_t sz;

		std::memcpy(headers, file.get_file_data(sz), 54);

		if (0x42 != headers[0] || 0x4d != headers[1])
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrDataSystem", "NOT BMP FILE");
			return false;
		}


		std::memcpy(&w, headers + 18, 4);
		std::memcpy(&h, headers + 22, 4);
		std::memcpy(&bpp, headers + 28, 2);
		switch (bpp) {
		case 16:
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrDataSystem", std::format("Not supported bpp: {0}", bpp).c_str());
			return false;
		case 24:
			image.create_empty(IMG_FORMAT::RGB_U888, w, h);
			break;
		case 32:
			image.create_empty(IMG_FORMAT::RGBA_U8888, w, h); //LV_COLOR_FORMAT_ARGB8888
			break;
		default:
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrDataSystem", std::format("Not supported bpp: {0}", bpp).c_str());
			return false;
		}
		return true;
	}
} // namespace wr is end
