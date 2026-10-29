#ifndef _WR_U8_STRING_CONVERT_HPP_
#define _WR_U8_STRING_CONVERT_HPP_

// base
#include <wrType.hpp>
// core
#include <core/string/wrU8String.hpp>

namespace wr
{
	u8_string unsigned_integral_to_dec_string_ptr(uint64_t number) noexcept;

	u8_string integral_to_dec_string_ptr(uint64_t abs_number, bool negative) noexcept;

	u8_string to_hex_caps_string(byte_t* data, uint32_t scan_size) noexcept;

	u8_string to_hex_no_caps_string(byte_t* data, uint32_t scan_size, const u8_string out_str) noexcept;

	int64_t dec_str_to_int64(const u8_string str) noexcept;

	uint64_t hex_str_to_uint64(const u8_string str) noexcept;

	constexpr u8_string to_hex_string(byte_t* data, size_t scan_size, bool is_caps) noexcept
	{
		if (is_caps)
			return to_hex_caps_string(data, scan_size);
		else
		{
			return to_hex_no_caps_string(data, scan_size);
		}
	}
} // namespace wr is end

#endif // _WR_U8_STRING_CONVERT_HPP_ IS EOF