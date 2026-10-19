#ifndef _WR_U8STRING_HPP_
#define _WR_U8STRING_HPP_

// base
#include <wrType.hpp>
#include <wrCompiler.hpp>
// core
#include <core/wrDynamicArray.hpp>


namespace wr
{
	const uint16_t END_OF_STRING = 0;

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

	c_extern exportfunc u8_string u8_str_create_empty(int64_t char_data_alloc_size, int64_t init_char_num) noexcept;

	// load
	c_extern exportfunc u8_string u8_str_load_all(const void* init_char_data, int64_t char_num, int64_t char_data_alloc_size) noexcept;
	// load
	c_extern exportfunc u8_string u8_str_load_c_str(const void* init_char_data, int64_t char_data_alloc_size) noexcept;

	c_extern exportfunc u8_string u8_str_load_no_size_c_str(const void* init_char_data) noexcept;

	c_extern exportfunc bool u8_str_cmp(const u8_string str1, const u8_string str2) noexcept;

	c_extern exportfunc u8_string u8_str_recount(u8_string str);

	c_extern exportfunc u8_string u8_str_slice(const u8_string str, int64_t start, int64_t end) noexcept;

	c_extern exportfunc u8_string u8_str_append_u8_str(u8_string src, const u8_string append_str) noexcept;

	c_extern exportfunc u8_string u8_str_append_unicode_char(u8_string src, unicode_t character) noexcept;

	c_extern exportfunc unicode_t u8_str_at(const u8_string src, int64_t char_index) noexcept;

	c_extern exportfunc pod_dynamic_array u8_str_find_all_u8_str_ofs(const u8_string src, const u8_string pattern) noexcept;

	c_extern exportfunc pod_dynamic_array u8_str_find_all_unicode_ofs(const u8_string src, unicode_t pattern) noexcept;

	c_extern exportfunc pod_dynamic_array u8_str_split(const u8_string src, const u8_string pattern) noexcept;

	inline u8_string u8_str_create_null() noexcept
	{
		u8_string out = { 0 };
		return out;
	}

	c_extern exportfunc void u8_str_release(u8_string str) noexcept;
} // namespace wr is end

#endif // _WR_U8STRING_HPP_ IS EOF