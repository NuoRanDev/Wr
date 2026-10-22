// statement
#include <core/string/wrU16String.hpp>
// base
#include <os/wrLogOutput.hpp>
// base
#include <os/wrAlloc.hpp>

namespace wr
{
	u16_string u16le_str_load_utf8_str(const u8_string u8_str) noexcept
	{
		const utf8_t* u8_src = u8_str.char_data;
		// without end of 0
		int64_t utf8_str_size = u8_str.char_data_size;
		int64_t len = utf8_str_size - 1;
		uint32_t c1;
		uint32_t c2;
		uint32_t c3;
		uint32_t c4;
		uint32_t code_point;
		utf16le_t u16_char;
		pod_dynamic_array u16str = pod_dynamic_void_array_create(sizeof(utf16le_t), utf8_str_size);

		u16_string out = { 0 };

		if (u8_src == nullptr || utf8_str_size == 0)
		{
			WR_INFO_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrCore", "The utf8 string is empty!");
			return out;
		}

		// is having BOM
		if (len > 3 && u8_src[0] == 0xEF && u8_src[1] == 0xBB && u8_src[2] == 0xBF)
		{
			u8_src += 3;
			len -= 3;
		}

		for (int64_t i = 0; i < len; ++i)
		{
			c1 = u8_src[i];	// Get UTF8
			if ((c1 & 0x80) == 0)
			{
				u16_char = static_cast<utf16le_t>(c1);
				u16str = pod_dynamic_array_push_back(u16str, &u16_char, 1);
				continue;
			}
			switch (c1 & 0xF0)
			{
			case 0xF0: // 4 Byte, 0x10000 TO 0x10FFFF
				c2 = u8_src[++i];
				c3 = u8_src[++i];
				c4 = u8_src[++i];
				code_point = ((c1 & 0x07U) << 18) | ((c2 & 0x3FU) << 12) | ((c3 & 0x3FU) << 6) | (c4 & 0x3FU);
				if (code_point >= 0x10000)
				{
					code_point -= 0x10000;
					u16_char = static_cast<utf16le_t>((code_point >> 10) | 0xD800U);
					u16str = pod_dynamic_array_push_back(u16str, &u16_char, 1);
					u16_char = static_cast<utf16le_t>((code_point & 0x03FFU) | 0xDC00U);
					u16str = pod_dynamic_array_push_back(u16str, &u16_char, 1);
				}
				else
				{
					u16_char = static_cast<utf16le_t>(code_point);
					u16str = pod_dynamic_array_push_back(u16str, &u16_char, 1);
				}
				break;

			case 0xE0:
				c2 = u8_src[++i];
				c3 = u8_src[++i];
				code_point = ((c1 & 0x0FU) << 12) | ((c2 & 0x3FU) << 6) | (c3 & 0x3FU);
				u16_char = static_cast<utf16le_t>(code_point);
				u16str = pod_dynamic_array_push_back(u16str, &u16_char, 1);
				break;

			case 0xD0:
			case 0xC0:
				c2 = u8_src[++i];
				code_point = ((c1 & 0x1FU) << 12) | ((c2 & 0x3FU) << 6);
				u16_char = static_cast<utf16le_t>(code_point);
				u16str = pod_dynamic_array_push_back(u16str, &u16_char, 1);
				break;
			default:
				break;
			}
		}
		if (u16str.item_num) [[likely]]
		{
			u16str = pod_dynamic_array_push_back(u16str, &END_OF_STRING, 1);
			if (u16str.item_num < u16str.capacity_num)
			{
				out.char_data = wr_realloc<utf16le_t>(u16str.pitems, u16str.item_num);
			}
			out.char_number = u16str.item_num;
		}
		return out;
	}

	u8_string u16le_str_to_utf8_str(const u16_string u16_str) noexcept
	{
		const utf16le_t* u16le_src = u16_str.char_data;
		int64_t utf16le_char_count = u16_str.char_number;
		u8_string out = { 0 };

		if (u16le_src == nullptr && utf16le_char_count == 0)
		{
			WR_INFO_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrCore", "The utf16 string is empty!");
			return out;
		}

		const utf16le_t* u16le_src_cur = u16le_src;
		int64_t u16_len = utf16le_char_count;
		pod_dynamic_array u8str;

		utf16le_t u16char;
		char u8c = 0;

		// If has bom
		if (u16le_src_cur[0] == 0xFEFF)
		{
			u16le_src_cur += 1;
			u16_len -= 1;
		}

		u8str = pod_dynamic_void_array_create(sizeof(utf8_t), utf16le_char_count * 3);

		for (int64_t i = 0; i < u16_len; ++i)
		{
			// le
			u16char = u16le_src_cur[i];

			if (u16char < 0x0080)
			{
				// u16char <= 0x007f
				// U- 0000 0000 ~ 0000 07ff : 0xxx xxxx
				u8c = static_cast<char>(u16char & 0x00FF);
				u8str = pod_dynamic_array_push_back(u8str, &u8c, 1);
				continue;
			}
			if (u16char >= 0x0080 && u16char <= 0x07FF)
			{
				// * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
				u8c = static_cast<char>(((u16char >> 6) & 0x1F) | 0xC0);
				u8str = pod_dynamic_array_push_back(u8str, &u8c, 1);
				u8c = static_cast<char>((u16char & 0x3F) | 0x80);
				u8str = pod_dynamic_array_push_back(u8str, &u8c, 1);
				continue;
			}
			// if use 2 wchar
			if (u16char >= 0xD800 && u16char <= 0xDBFF) 
			{
				// * U-00010000 - U-001FFFFF: 1111 0xxx 10xxxxxx 10xxxxxx 10xxxxxx
				uint32_t highSur = u16char;
				uint32_t lowSur = u16le_src_cur[++i];
				uint32_t codePoint = highSur - 0xD800;
				codePoint <<= 10;
				codePoint |= lowSur - 0xDC00;
				codePoint += 0x10000;
				// 2 * 2byte
				u8c = static_cast<char>((char)((codePoint >> 18) | 0xF0));
				u8str = pod_dynamic_array_push_back(u8str, &u8c, 1);
				u8c = static_cast<char>((char)(((codePoint >> 12) & 0x3F) | 0x80));
				u8str = pod_dynamic_array_push_back(u8str, &u8c, 1);
				u8c = static_cast<char>((char)(((codePoint >> 06) & 0x3F) | 0x80));
				u8str = pod_dynamic_array_push_back(u8str, &u8c, 1);
				u8c = static_cast<char>((char)((codePoint & 0x3F) | 0x80));
				u8str = pod_dynamic_array_push_back(u8str, &u8c, 1);
				continue;
			}
			else
			{
				// * U-0000E000 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
				u8c = static_cast<char>((char)(((u16char >> 12) & 0x0F) | 0xE0));
				u8str = pod_dynamic_array_push_back(u8str, &u8c, 1);
				u8c = static_cast<char>((char)(((u16char >> 6) & 0x3F) | 0x80));
				u8str = pod_dynamic_array_push_back(u8str, &u8c, 1);
				u8c = static_cast<char>((char)((u16char & 0x3F) | 0x80));
				u8str = pod_dynamic_array_push_back(u8str, &u8c, 1);
				continue;
			}
		}

		u8str = pod_dynamic_array_push_back(u8str, &END_OF_STRING, 1);
		
		if(u8str.item_num) [[likely]]
		{
			out.char_data_size = u8str.item_num;
			if (u8str.item_num < u8str.capacity_num)
			{
				out.char_data = wr_realloc<utf8_t>(u8str.pitems, u8str.item_num);
			}
			out = u8_str_recount(out);
		}
		return out;
	}

	u16_string u16le_str_release(u16_string u16_str) noexcept
	{
		wr_check_null_free(u16_str.char_data);
		u16_str = { 0 };
		return u16_str;
	}

} // namespace wr is end