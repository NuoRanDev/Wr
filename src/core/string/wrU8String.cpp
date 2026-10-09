// statement
#include <core/string/wrU8String.hpp>
// OS
#include <os/wrAlloc.hpp>
#include <os/wrLogOutput.hpp>
// dev
#include <dev/wrMemory.hpp>
// std
#include <algorithm>

namespace wr
{
	constexpr int64_t SHORT_STRING_SIZE = 12;

	constexpr utf8_t PREFIX[] = { 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

	constexpr unicode_t CODE_UP[] =
	{
		0x80,           // U+00000000 - U+0000007F  
		0x800,          // U+00000080 - U+000007FF  
		0x10000,        // U+00000800 - U+0000FFFF  
		0x200000,       // U+00010000 - U+001FFFFF  
		0x4000000,      // U+00200000 - U+03FFFFFF  
		0x80000000      // U+04000000 - U+7FFFFFFF  
	};

	inline bool compare_memory(const utf8_t* lhs, const utf8_t* rhs, int64_t length) noexcept
	{
		if (length == 0)
			return 0;
		return wr_memcmp(lhs, rhs, length) == 0;
	}

	static int32_t utf8_byte_type(utf8_t c) noexcept
	{
		if (c < 0x80)
			return 1;
		else if (c < 0xC0)
			return 0;
		else if (c >= 0xF5 || (c & 0xFE) == 0xC0)
		{
			// "octet values c0, c1, f5 to ff never appear"
			WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrCore", "octet values c0, c1, f5 to ff never appear");
			return -1;
		}
		else
		{
			int value = (((0XE5 << 24) >> ((unsigned)c >> 4 << 1)) & 3) + 1;
			// assert(value >= 2 && value <=4);
			return value;
		}
	}

	static bool utf8_bype_is_valid_leading_byte(int type) noexcept { return type > 0; }

	static bool utf8_byte_is_continuation(const utf8_t c) noexcept { return utf8_byte_type(c) == 0; }

	static int32_t utf32_to_utf8(unicode_t utf32_str, utf8_t* utf8_str) noexcept
	{
		if (utf32_str == 0) return 0;
		int i, len = 6;
		for (i = 0; i < len; ++i)
			if (utf32_str < CODE_UP[i]) break;
		if (i == len)
		{
			WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrCore", "The utf32 string is invalid");
			return 0;
		}
		len = i + 1;
		while (i > 0)
		{
			utf8_str[i] = static_cast<utf8_t>((utf32_str & 0x3F) | 0x80);
			utf32_str = utf32_str >> 6;
			i--;
		}
		utf8_str[0] = static_cast<utf8_t>(utf32_str | static_cast<unicode_t>(PREFIX[len - 1]));
		return len;
	}

	static void utf8_to_utf32(const utf8_t* src, unicode_t& des) noexcept
	{
		if (src == nullptr || (*src) == 0)
		{
			des = 0;
			return;
		}

		utf8_t b = src[0];

		if (b < 0x80)
		{
			des = static_cast<unicode_t>(b);
			return;
		}

		if (b < 0xC0 || b > 0xFD)
		{
			WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrCore", "The src is invalid");
			return;
		} // the src is invalid  

		size_t len;

		if (b < 0xE0)
		{
			des = b & 0x1F;
			len = 2;
		}
		else if (b < 0xF0)
		{
			des = b & 0x0F;
			len = 3;
		}
		else if (b < 0xF8)
		{
			des = b & 0x07;
			len = 4;
		}
		else if (b < 0xFC)
		{
			des = b & 0x03;
			len = 5;
		}
		else
		{
			des = b & 0x01;
			len = 6;
		}

		for (int i = 1; i < len; i++)
		{
			b = src[i];
			if (b < 0x80 || b > 0xBF)
			{
				WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrCore", "the src is invalid");
				return; // the src is invalid  
			}
			des = (des << 6) + (b & 0x3F);
		}
	}

	static int64_t count_utf8(const utf8_t* utf8, int64_t alloc_size) noexcept
	{
		int count = 0;
		if ((utf8 == nullptr) || (alloc_size == 0) || (utf8[0] == '\0'))
		{
			return count;
		}

		const utf8_t* stop = utf8 + alloc_size;

		while (utf8 < stop)
		{
			int type = utf8_byte_type(*utf8);
			if (!utf8_bype_is_valid_leading_byte(type) || utf8 + type > stop)
			{
				WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrCore", "Utf8 text not coherent");
				return 0;  // Sequence extends beyond end.
			}
			while ((type--) > 1) {
				++utf8;
				if (!utf8_byte_is_continuation(*utf8))
				{
					WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrCore", "Utf8 text not coherent");
					return 0;
				}
			}
			++utf8;
			++count;
		}
		return count;
	}

	u8_string load_all(const void* char_data, int64_t char_num, int64_t char_data_alloc_size) noexcept
	{	
		if(char_data_alloc_size) [[unlikely]]
		{
			return u8_string{};
		}
		else [[likely]]
		{
			u8_string str;
			str.char_num = char_num;
			str.char_data_size = char_data_alloc_size;
			str.is_short_string = char_data_alloc_size < SHORT_STRING_SIZE;
			str.char_data = wr_malloc<utf8_t>(char_data_alloc_size);
			wr_memcpy(str.char_data, char_data, char_data_alloc_size);
			return str;
		}
	}

	u8_string load_c_str(const void* char_data, int64_t char_data_alloc_size) noexcept
	{
		int64_t num = count_utf8(reinterpret_cast<const utf8_t*>(char_data), char_data_alloc_size - 1);
		return load_all(char_data, num, char_data_alloc_size);
	}

	bool str_cmp(const u8_string str1, const u8_string str2) noexcept
	{
		if (str1.char_data_size != str2.char_data_size) [[likely]]
			return false;
		else [[unlikely]]
		{
			return static_cast<bool>(wr_memcmp(str1.char_data, str2.char_data, str1.char_data_size));
		}
	}

	u8_string str_slice(u8_string str, int64_t start, int64_t end) noexcept
	{
		int64_t _cur_number = 0;
		utf8_t* start_str_ptr = nullptr;
		utf8_t* cur_str_ptr = str.char_data;
		int64_t need_copy_offset = 0;

		start = std::min(start, str.char_num);
		if (end != -1)
			end = std::clamp(end, start, str.char_num);
		else
		{
			end = str.char_num;
		}

		while (_cur_number != start)
		{
			int type = utf8_byte_type(*cur_str_ptr);
			while ((type--) > 1)
			{
				cur_str_ptr++;
				if (!utf8_bype_is_valid_leading_byte(*cur_str_ptr))
				{
					WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrCore", "Utf8 text not coherent");
					return create_empty_u8str();
				}
			}
			cur_str_ptr++;
			_cur_number++;
		}
		start_str_ptr = cur_str_ptr;

		while (_cur_number != end)
		{
			int type = utf8_byte_type(*cur_str_ptr);
			while ((type--) > 1)
			{
				cur_str_ptr++;
				need_copy_offset++;
				if (!utf8_bype_is_valid_leading_byte(*cur_str_ptr))
				{
					WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrCore", "Utf8 text not coherent");
					return create_empty_u8str();
				}
			}
			need_copy_offset++;
			cur_str_ptr++;
			_cur_number++;
		}
		return load_all(start_str_ptr, need_copy_offset + 1, end - start);
	}

	u8_string append_str(u8_string src, const u8_string append_str) noexcept
	{
		int64_t jump_str_offset = src.char_data_size - 1;
		utf8_t* append_ptr_start = nullptr;

		src.char_num += append_str.char_num;

		// exclude append string 's '\0'
		src.char_data_size = (src.char_data_size - 1) + append_str.char_data_size;

		// alloc
		src.char_data = wr_realloc<utf8_t>(src.char_data, src.char_data_size);
		append_ptr_start = src.char_data + jump_str_offset;
		//
		wr_memcpy(append_ptr_start, append_str.char_data, append_str.char_data_size);
		src.char_data[src.char_data_size - 1] = 0;
		src.is_short_string = (src.char_data_size < SHORT_STRING_SIZE);
		return src;
	}

	u8_string append_char(u8_string src, unicode_t character) noexcept
	{
		utf8_t output_utf8_str[4] = { 0 };
		int32_t output_utf8_str_size = utf32_to_utf8(character, output_utf8_str);
		if (output_utf8_str_size) [[likely]]
		{
			u8_string u8char =
			{
				.char_num = 1,
				.char_data_size = output_utf8_str_size,
				.is_short_string = true,
				.char_data = output_utf8_str,
			};
			return append_str(src, u8char);
		}
		else
		{
			return src;
		}
	}

	unicode_t at(const u8_string src, int64_t char_index) noexcept
	{
		int64_t _cur_index = 0;
		const utf8_t* cur_str_ptr = src.char_data;
		unicode_t output_character = U'\0';
		if ((char_index < 0) || (char_index >= src.char_num))
		{
			WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrCore", "Out of string");
			return output_character;
		}
		while (_cur_index != char_index)
		{
			int type = utf8_byte_type(*cur_str_ptr);
			while ((type--) > 1)
			{
				cur_str_ptr++;
				if (!utf8_bype_is_valid_leading_byte(*cur_str_ptr))
				{
					WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrCore", "Utf8 text not coherent");
					return U'\0';
				}
			}
			_cur_index++;
			cur_str_ptr++;
		}

		utf8_to_utf32(cur_str_ptr, output_character);
		return output_character;
	}

	void release_u8_string(u8_string str) noexcept
	{
		wr_check_null_free(str.char_data);
	}
	/*
	dynamic_array_data<int64_t> find_all_ofs(const unicode_t pattern) noexcept
	{
		utf8_t utf8_separator[4] = { 0 };
		auto out_size = utf32_to_utf8(pattern, utf8_separator);
		dynamic_array<int64_t> out;
		if (out_size == 0)
		{
			WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrCore", "Not input unicode");
			return out.move_pod();
		}
		if (out_size == 1)return find_all(utf8_separator[0]);
		else
		{
			return find_all(utf8_separator, out_size);
		}
	}

	dynamic_array_data<u8_string> split(const u8_string separator) noexcept
	{
		return dynamic_array_data<u8_string>();
	}*/
} // namespace wr is end