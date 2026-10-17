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
		// string size
		int64_t char_num;
		// alloc size
		// struction [],[],[]...,[],0
		int64_t char_data_size;
		// data ptr
		utf16le_t* char_data;
	};

	[[nodiscard]] c_extern exportfunc u16_string u16_string_load_utf8_string(const u8_string u8_str);
} // namespace wr is end

#endif // _WR_U16STRING_HPP_ IS EOF