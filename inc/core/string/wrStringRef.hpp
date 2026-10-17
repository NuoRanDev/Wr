#ifndef _WR_U8STRING_REF_HPP_
#define _WR_U8STRING_REF_HPP_

// base
#include <wrType.hpp>
#include <wrCompiler.hpp>
#include <wrTemplate.hpp>
// core
#include <core/string/wrU8String.hpp>
#include <core/wrDynamicArray.hpp>

namespace wr
{
	class u8_string_ref
	{
	private:
		u8_string str_data;

	public:
		// 
		u8_string_ref() noexcept
		{
			str_data = u8_str_create_null();
		}
		// laod c and c++ utf8 string
		template<char_type ch_t> u8_string_ref(const ch_t* str) noexcept
		{
			const char* ch_in = reinterpret_cast<const char*>(str);
			str_data = u8_str_load_c_str(ch_in, strlen(ch_in));
		}
		// load
		u8_string_ref(const utf8_t* str, size_t init_char_num,size_t mem_size) noexcept
		{
			str_data = u8_str_load_all(str, init_char_num, mem_size);
		}
		// load pod
		u8_string_ref(u8_string pod) noexcept
		{
			str_data = pod;
		}

		// 
		const u8_string get_pod() const noexcept
		{
			return str_data;
		}

		// 
		u8_string_ref slice(int64_t start, int64_t end) const noexcept
		{
			// NRVO
			return u8_string_ref(u8_str_slice(str_data, start, end));
		}

		void append(unicode_t ch) noexcept
		{
			str_data = u8_str_append_unicode_char(str_data, ch);
		}

		void append(u8_string_ref str) noexcept
		{
			str_data = u8_str_append_u8_str(str_data, str.get_pod());
		}

		unicode_t at(int64_t index) const noexcept { return u8_str_at(str_data, index); }

		dynamic_array<u8_string_ref> split(const u8_string_ref pattern) const noexcept
		{
			dynamic_array<u8_string> str_list = dynamic_array<u8_string>(u8_str_split(str_data, pattern.get_pod()));
			// NRVO
			dynamic_array<u8_string_ref> out = dynamic_array<u8_string_ref>(str_list.num());
			for (size_t i = 0; i < str_list.num(); i++)
			{
				out.at(i) = str_list.at(i);
			}
			return out;
		}

		void release() noexcept
		{
			u8_str_release(str_data);
		}

		~u8_string_ref() { release(); }
	};

} // namespace wr is end

#endif // _WR_U8STRING_REF_HPP_ IS EOF