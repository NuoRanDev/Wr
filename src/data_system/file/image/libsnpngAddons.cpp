#include "file/image/wrReadImage.hpp"

#include "log/wrLogOutput.hpp"
#include "type/wrOrdinals.hpp"

#include <cstdint>
#include <format>

#if defined(USE_PNG)
// Thired-party library: libspng
#include "spng.h"
namespace wr
{
	bool read_memory_png_image(const ImageFile& file, Image& img_out) noexcept
	{
		byte_t color_type = 0;
		spng_ihdr ihdr = { 0 };

		// need alloc memory
		size_t need_new_size = 0;

		//
		int32_t fmt = 0;
		int32_t ret = 0;

		size_t file_size;
		const byte_t* png_buffer;

		IMG_FORMAT chaanel_format;

		// Create image object
		png_buffer = file.get_file_data(file_size);
		if (png_buffer == nullptr || file_size == 0)
		{
			WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "DataSystem : libspngAddons", std::format("File:{0} image file data is empty", file.c_file_name()).c_str());
			return false;
		}

		spng_ctx* ctx = spng_ctx_new(0);
		if (ctx == nullptr)
		{
			WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "DataSystem : libspngAddons", std::format("File:{0} init context failed", file.c_file_name()).c_str());
			return false;
		}

		ret = spng_set_png_buffer(ctx, png_buffer, file_size);
		if (ret)
		{
			spng_ctx_free(ctx);
			WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "DataSystem : libspngAddons", std::format("File:{0} load data failed", file.c_file_name()).c_str());
			return false;
		}

		spng_get_ihdr(ctx, &ihdr);
		if (ret)
		{
			spng_ctx_free(ctx);
			WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "DataSystem : libspngAddons", std::format("File:{0} get header information data failed", file.c_file_name()).c_str());
			return false;
		}

		// Xe studio output data is 8 bit depth
		if (ihdr.bit_depth != 8 and ihdr.bit_depth != 16)
		{
			spng_ctx_free(ctx);
			WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "DataSystem : libspngAddons",
				std::format("File:{1} image data is broken. Image bit depth is {0}, WR support Bit depth is 8 and 16!", 
				ihdr.bit_depth, file.c_file_name()).c_str());
			return false;
		}

		// Get image bit depth and color type
		color_type = ihdr.color_type;

		// Get all piwrl png is Big-endian 
		switch (color_type)
		{
		case SPNG_COLOR_TYPE_GRAYSCALE:
			if(ihdr.bit_depth == 8)
			{
				spng_decoded_image_size(ctx, SPNG_FMT_G8, &need_new_size);
				fmt						= SPNG_FMT_RGBA8;
				chaanel_format			= IMG_FORMAT::RGBA_U8888;
				break;
			}
			// if (ihdr.bit_depth == 16) 16bit gray is not support, what's fucking crazy man use it!
			else goto COLOR_NOT_SUPPORT;
		case SPNG_COLOR_TYPE_TRUECOLOR:
			if(ihdr.bit_depth == 8)
			{
				spng_decoded_image_size(ctx, SPNG_FMT_RGB8, &need_new_size);
				fmt						= SPNG_FMT_RGB8;
				chaanel_format			= IMG_FORMAT::RGB_U888;
				break;
			}
			else goto COLOR_NOT_SUPPORT;
			// The fucking old color picture
		case SPNG_COLOR_TYPE_INDEWRD:
			if(ihdr.bit_depth == 8)
			{
				spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &need_new_size);
				fmt						= SPNG_FMT_RGBA8;
				chaanel_format			= IMG_FORMAT::RGBA_U8888;
				break;
			}
			if (ihdr.bit_depth == 16)
			{
				spng_decoded_image_size(ctx, SPNG_FMT_RGBA16, &need_new_size);
				fmt						= SPNG_FMT_RGBA16;
				chaanel_format			= IMG_FORMAT::RGBA_U16161616;
				break;
			}
			else goto COLOR_NOT_SUPPORT;
			// I think this format is used less, but my penis master thinks that can hide some sexy pictures....
		case SPNG_COLOR_TYPE_GRAYSCALE_ALPHA:
			if (ihdr.bit_depth == 8)
			{
				spng_decoded_image_size(ctx, SPNG_FMT_GA8, &need_new_size);
				fmt						= SPNG_FMT_GA8;
				chaanel_format			= IMG_FORMAT::GA_U8;
				break;
			}
			if (ihdr.bit_depth == 16)
			{
				spng_decoded_image_size(ctx, SPNG_FMT_GA16, &need_new_size);
				fmt						= SPNG_FMT_GA16;
				chaanel_format			= IMG_FORMAT::GA_U16;
				break;
			}
			else goto COLOR_NOT_SUPPORT;
		case SPNG_COLOR_TYPE_TRUECOLOR_ALPHA:
			if(ihdr.bit_depth == 8)
			{
				spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &need_new_size);
				fmt						= SPNG_FMT_RGBA8;
				chaanel_format			= IMG_FORMAT::RGBA_U8888;
				break;
			}
			if (ihdr.bit_depth == 16) 
			{
				spng_decoded_image_size(ctx, SPNG_FMT_RGBA16, &need_new_size);
				fmt						= SPNG_FMT_RGBA16;
				chaanel_format			= IMG_FORMAT::RGBA_U16161616;
				break;
			}
			else goto COLOR_NOT_SUPPORT;
		default:
			goto COLOR_NOT_SUPPORT;
		}

		img_out.create_empty(chaanel_format, ihdr.width, ihdr.height);
		
		// Get line data point start and read image
		if (need_new_size != img_out.get_data_size())
		{
			spng_ctx_free(ctx);
			WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "DataSystem : libspng", std::format("Image data is broken or alloc size failed",file.c_file_name()).c_str());
			return false;
		}
		spng_decode_image(ctx, img_out.unsafe_data(), need_new_size, fmt, 0);

		// Destory linspng struction
		spng_ctx_free(ctx);
		return true;
	COLOR_NOT_SUPPORT:
		spng_ctx_free(ctx);
		WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "DataSystem : libspng", std::format("Image data is broken.Not support this color format", file.c_file_name()).c_str());
		return false;
	}
}
#endif // USE_PNG