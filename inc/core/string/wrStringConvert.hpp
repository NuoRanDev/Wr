#ifndef _INC_WR_STRING_CONVERT_HPP_
#define _INC_WR_STRING_CONVERT_HPP_

// core
#include <string/wrString.hpp>
#include <type/wrTemplate.hpp>

namespace wr
{

	constexpr size_t INTEGRAL_STRING_MAX = sizeof("-9223372036854775807");
	constexpr size_t UNINTEGRAL_STRING_MAX = sizeof("18446744073709551615");

	namespace StringConvert
	{
		void exportfunc unsigned_integral_to_dec_string_ptr(uint64_t number, utf8_t* str, int64_t& str_size) noexcept;

		void exportfunc integral_to_dec_string_ptr(uint64_t abs_number, utf8_t* str, int64_t& str_size, bool negative) noexcept;

		template<SignedIntegral T> String to_dec_string(T number) noexcept
		{
			String out_str;
			utf8_t characters[INTEGRAL_STRING_MAX] = { 0 };
			int64_t out_str_size = 0;
			utf8_t* str = reinterpret_cast<utf8_t*>(&characters);

			integral_to_dec_string_ptr(static_cast<uint64_t>(std::abs(number)),
				str + INTEGRAL_STRING_MAX - 2,
				out_str_size,
				number < 0);
			out_str.load_wr_str_include0(str + (20 - out_str_size), out_str_size + 1, out_str_size);
			return out_str;
		}

		template<UnsignedIntegral T> String to_dec_string(T number) noexcept
		{
			String out_str;
			utf8_t characters[UNINTEGRAL_STRING_MAX] = { 0 };
			int64_t out_str_size = 0;
			utf8_t* str = reinterpret_cast<utf8_t*>(&characters);

			unsigned_integral_to_dec_string_ptr(static_cast<uint64_t>(number), str + UNINTEGRAL_STRING_MAX - 2, out_str_size);
			out_str.load_wr_str_include0(str + (20 - out_str_size), out_str_size + 1, out_str_size);
			return out_str;
		}

		int64_t exportfunc dec_str_to_int64(const String& str) noexcept;

		uint64_t exportfunc hex_str_to_uint64(const String& str) noexcept;

		void exportfunc to_hex_string(byte_t* data, size_t scan_size, String& out_str, bool is_caps) noexcept;

		template<baise_type_no_struct_or_class T> String to_hex_string(T data, bool is_caps = false) noexcept
		{
			String out_str;
			to_hex_string(reinterpret_cast<byte_t*>(&data), sizeof(T), out_str, is_caps);
			return out_str;
		}

	} // namespace StringConvert is end
} // namespace wr is end

#endif // _INC_WR_STRING_CONVERT_HPP_ IS EOF