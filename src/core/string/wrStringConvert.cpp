// statement
#include <string/wrStringConvert.hpp>
// std
#include <cmath>

namespace wr
{
	constexpr size_t UNINTEGRAL_HEX_STRING_MAX = sizeof(uint64_t) * 2 + 1;

	constexpr utf8_t to_hex_no_caps_char(uint8_t temp_int)
	{
		switch (temp_int)
		{
		case 0x0:
			return u8'0';
		case 0x1:
			return u8'1';
		case 0x2:
			return u8'2';
		case 0x3:
			return u8'3';
		case 0x4:
			return u8'4';
		case 0x5:
			return u8'5';
		case 0x6:
			return u8'6';
		case 0x7:
			return u8'7';
		case 0x8:
			return u8'8';
		case 0x9:
			return u8'9';
		case 0xa:
			return u8'a';
		case 0xb:
			return u8'b';
		case 0xc:
			return u8'c';
		case 0xd:
			return u8'd';
		case 0xe:
			return u8'e';
		case 0xf:
			return u8'f';
		default:
			return 0;
		}
	}

	constexpr utf8_t to_hex_caps_char(uint8_t temp_int)
	{
		switch (temp_int)
		{
		case 0x0:
			return u8'0';
		case 0x1:
			return u8'1';
		case 0x2:
			return u8'2';
		case 0x3:
			return u8'3';
		case 0x4:
			return u8'4';
		case 0x5:
			return u8'5';
		case 0x6:
			return u8'6';
		case 0x7:
			return u8'7';
		case 0x8:
			return u8'8';
		case 0x9:
			return u8'9';
		case 0xA:
			return u8'A';
		case 0xB:
			return u8'B';
		case 0xC:
			return u8'C';
		case 0xD:
			return u8'D';
		case 0xE:
			return u8'E';
		case 0xF:
			return u8'F';
		default:
			return 0;
		}
	}

	constexpr uint64_t hex_char_to_uint64(utf8_t utf8char)
	{
		switch (utf8char)
		{
		case '0':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		case 'a':
			return 0xa;
		case 'b':
			return 0xb;
		case 'c':
			return 0xc;
		case 'd':
			return 0xd;
		case 'e':
			return 0xe;
		case 'f':
			return 0xf;
		case 'A':
			return 0xA;
		case 'B':
			return 0xB;
		case 'C':
			return 0xC;
		case 'D':
			return 0xD;
		case 'E':
			return 0xE;
		case 'F':
			return 0xF;
		default:
			return UINT64_MAX;
		}
	}

	namespace StringConvert
	{
		struct Hexfield
		{
			unsigned int hex1 : 4;
			unsigned int hex2 : 4;
		};

		void unsigned_integral_to_dec_string_ptr(uint64_t number, utf8_t* str, int64_t& str_size) noexcept
		{
			str_size = 0;
			while (number)
			{
				str[0] = static_cast<utf8_t>((number % 10) + '0');
				str = str - 1;
				str_size++;
				number = number / 10;
			}
		}

		void integral_to_dec_string_ptr(uint64_t abs_number, utf8_t* str, int64_t& str_size, bool negative) noexcept
		{
			str_size = 0;
			while (abs_number)
			{
				str[0] = static_cast<utf8_t>((uint8_t)(abs_number % 10) + '0');
				str = str - 1;
				str_size++;
				abs_number = abs_number / 10;
			}
			if (negative)
			{
				str[0] = '-';
				str_size++;
			}
		}

		static void to_hex_caps_string(byte_t* data, uint32_t scan_size, String& out_str) noexcept
		{
			utf8_t temp_str[UNINTEGRAL_HEX_STRING_MAX] = { 0 };
			utf8_t* cur_str = reinterpret_cast<utf8_t*>(&temp_str) + UNINTEGRAL_HEX_STRING_MAX - 2;
			uint32_t cur_scan = 0;
			uint8_t temp_int;

			while (cur_scan < scan_size)
			{
				Hexfield* hex_data = reinterpret_cast<Hexfield*>(data);
				temp_int = hex_data[0].hex1;
				cur_str[0] = to_hex_caps_char(temp_int);
				cur_str--;
				temp_int = hex_data[0].hex2;
				cur_str[0] = to_hex_caps_char(temp_int);
				cur_str--;
				cur_scan++;
				data++;
			}
			cur_str++;
			out_str.load_wr_str_include0(cur_str,
				static_cast<int64_t>(scan_size) * 2 + 1,
				static_cast<int64_t>(scan_size) * 2);
		}

		static void to_hex_no_caps_string(byte_t* data, uint32_t scan_size, String& out_str) noexcept
		{
			utf8_t temp_str[UNINTEGRAL_HEX_STRING_MAX] = { 0 };
			utf8_t* cur_str = reinterpret_cast<utf8_t*>(&temp_str) + UNINTEGRAL_HEX_STRING_MAX - 2;
			uint32_t cur_scan = 0;
			uint8_t temp_int = 0;

			while (cur_scan < scan_size)
			{
				Hexfield* hex_data = reinterpret_cast<Hexfield*>(data);
				temp_int = hex_data[0].hex1;
				cur_str[0] = to_hex_no_caps_char(temp_int);
				cur_str--;
				temp_int = hex_data[0].hex2;
				cur_str[0] = to_hex_no_caps_char(temp_int);
				cur_str--;
				cur_scan++;
				data++;
			}
			cur_str++;
			out_str.load_wr_str_include0(cur_str,
				static_cast<int64_t>(scan_size) * 2 + 1,
				static_cast<int64_t>(scan_size) * 2);
		}

		int64_t dec_str_to_int64(const String& str) noexcept
		{
			int64_t out_int = 0;
			int32_t ofs = 0;
			int32_t negative = 1;
			const utf8_t* pstr = str.data();
			if (pstr[0] == '-')
			{
				negative = -1;
				ofs = 1;
			}
			while (ofs < (INTEGRAL_STRING_MAX - 1))
			{
				if (pstr[ofs] < '0' || pstr[ofs] > '9')
					break;
				out_int = out_int * 10 + (pstr[ofs] - '0');
				ofs++;
			}
			return out_int * negative;
		}

		uint64_t hex_str_to_uint64(const String& str) noexcept
		{
			uint64_t out_int = 0;
			int32_t ofs = 0;
			const utf8_t* pstr = str.data();

			while (ofs < UNINTEGRAL_HEX_STRING_MAX)
			{
				if ((pstr[ofs] < '0' || pstr[ofs] > '9')
					&& (pstr[ofs] < 'a' || pstr[ofs] > 'f')
					&& (pstr[ofs] < 'A' || pstr[ofs] > 'F'))
					break;
				out_int = out_int * 16 + hex_char_to_uint64(pstr[ofs]);
				ofs++;
			}

			return out_int;
		}

		void to_hex_string(byte_t* data, size_t scan_size, String& out_str, bool is_caps) noexcept
		{
			if (is_caps)
				to_hex_caps_string(data, scan_size, out_str);
			else
			{
				to_hex_no_caps_string(data, scan_size, out_str);
			}
		}
	} // namespace StringConvert is end
} // namespace wr is end