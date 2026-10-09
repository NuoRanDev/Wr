#ifndef _WR_U8STRING_HPP_
#define _WR_U8STRING_HPP_

// base
#include <wrType.hpp>
//#include <wrResult.hpp>
#include <wrCompiler.hpp>
// core
#include <core/wrDynamicArray.hpp>


namespace wr
{
	struct u8_string
	{
		// string size
		int64_t char_num;
		// alloc size
		// struction [],[],[]...,[],0
		int64_t char_data_size;
		// <12 is true
		bool is_short_string;
		// data ptr
		utf8_t* char_data;
	};

	// load
	c_extern exportfunc u8_string load_all(const void* char_data, int64_t char_num, int64_t char_data_alloc_size) noexcept;
	// load
	c_extern exportfunc u8_string load_c_str(const void* char_data, int64_t char_data_alloc_size) noexcept;

	c_extern exportfunc bool str_cmp(const u8_string str1, const u8_string str2) noexcept;

	c_extern exportfunc u8_string str_slice(u8_string str, int64_t start, int64_t end) noexcept;

	c_extern exportfunc u8_string append_str(u8_string src, const u8_string append_str) noexcept;

	c_extern u8_string append_char(u8_string src, unicode_t character) noexcept;

	c_extern exportfunc unicode_t at(const u8_string, int64_t char_index) noexcept;

	c_extern exportfunc void release_u8_string(u8_string str) noexcept;

	// c_extern dynamic_array_data<int64_t> find_all_ofs(const unicode_t pattern) noexcept;

	// c_extern dynamic_array_data<u8_string> split(const u8_string separator)noexcept;

	forceinline u8_string create_empty_u8str() noexcept
	{
		return load_all("", 0, 1);
	}
} // namespace wr is end

#endif // _WR_U8STRING_HPP_ IS EOF