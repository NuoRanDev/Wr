#include "image/wrImage.hpp"
#include "log/wrLogOutput.hpp"

#include <cstring>

namespace wr
{

	void gray_to_rgb(const byte_t* src_data, byte_t* out_data, int32_t x, int32_t y, int64_t channel_size) noexcept
	{
		const byte_t* src_cur = src_data;
		byte_t* dst_cur = out_data;

		for (int32_t i = 0; i < y * x; i++)
		{
			// R
			memcpy(dst_cur, src_cur, channel_size);
			dst_cur += channel_size;
			// G
			memcpy(dst_cur, src_cur, channel_size);
			dst_cur += channel_size;
			// B
			memcpy(dst_cur, src_cur, channel_size);
			dst_cur += channel_size;
			src_cur += channel_size;
		}
	}

	void ga_to_rgba(const byte_t* src_data, byte_t* out_data, int32_t x, int32_t y, int64_t channel_size) noexcept
	{
		const byte_t* src_cur = src_data;
		byte_t* dst_cur = out_data;

		for (int32_t i = 0; i < y * x; i++)
		{
			// R
			memcpy(dst_cur, src_cur, channel_size);
			dst_cur += channel_size;
			// G
			memcpy(dst_cur, src_cur, channel_size);
			dst_cur += channel_size;
			// B
			memcpy(dst_cur, src_cur, channel_size);
			dst_cur += channel_size;
			src_cur += channel_size;
			// A
			memcpy(dst_cur, src_cur, channel_size);
			dst_cur += channel_size;
			src_cur += channel_size;
		}
	}

	void Image::create_empty(const IMG_FORMAT src_format, const int32_t src_x, const int32_t src_y) noexcept
	{
		x = src_x, y = src_y;
		format = src_format;
		data_size = get_img_size(format, x, y);
		img_data = alloc_data_memory(data_size);
	}

	bool Image::load_data(IMG_FORMAT src_format, const byte_t* src_data, int32_t src_x, int32_t src_y) noexcept
	{
		create_empty(src_format, src_x, src_y);
		img_data = alloc_data_memory(data_size);
		memcpy(img_data, src_data, data_size);
		return true;
	}

	bool Image::load_data(byte_t* src_data, int32_t src_size) noexcept
	{
		if (src_data == nullptr || src_size == 0)
		{
			WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::APP,
				"wrDataSystem",
				"Image data is empty");
			return false;
		}
		if (src_size != data_size)
		{
			WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::APP,
				"wrDataSystem",
				"Image data size is not match");
			return false;
		}
		memcpy(img_data, src_data, src_size);
		data_size = src_size;
		return true;
	}

	const byte_t* const Image::get_data(int64_t& out_data_size) const noexcept
	{
		if (img_data == nullptr || data_size == 0)
		{
			WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::APP,
				"wrDataSystem",
				"Image data is empty");
			return nullptr;
		}
		out_data_size = data_size;
		return img_data;
	}

	byte_t* Image::unsafe_data() noexcept { return img_data; }
} // namespace wr is end