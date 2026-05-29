#ifndef _WR_RWAD_IMAGE_HPP_
#define _WR_RWAD_IMAGE_HPP_

// core
#include <type/wrOrdinals.hpp>
// data system
#include <image/wrImage.hpp>
#include <file/image/wrImgaeFile.hpp>

namespace wr
{
#if defined(USE_PNG)
	bool read_memory_png_image(const ImageFile& file, Image& img_out) noexcept;
#endif // USE_PNG
#if defined(USE_JPEG)
	bool read_memory_jpg_image(const ImageFile& file, Image& img_out) noexcept;
#endif // USE_JPEG
#if defined(USE_AVIF)
	bool read_memory_avif_image(const ImageFile& file, Image& img_out) noexcept;
#endif // USE_AVIF
	bool read_memory_ppm_image(const ImageFile& file, Image& img_out) noexcept;
} // namespace wr is end

#endif // _WR_RWAD_IMAGE_HPP_ IS EOF