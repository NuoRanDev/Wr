#ifndef _WR_U16STRING_HPP_
#define _WR_U16STRING_HPP_

// base
#include <wrType.hpp>
#include <wrCompiler.hpp>
// core
#include <core/string/wrU8String.hpp>

namespace wr
{
	struct u16_string
	{
		// alloc size
		// struction [],[],[]...,[],0
		int64_t char_number;
		// data ptr
		utf16le_t* char_data;
	};

	c_extern exportfunc u16_string u16le_str_load_utf8_str(const u8_string u8_str) noexcept;

	c_extern exportfunc u8_string u16le_str_to_utf8_str(const u16_string u16_str) noexcept;

	c_extern exportfunc u16_string u16le_str_release(u16_string u16_str) noexcept;

	constexpr bool u16le_str_is_empty(u16_string u16_str) { return u16_str.char_number == 0; }
} // namespace wr is end

#endif // _WR_U16STRING_HPP_ IS EOF