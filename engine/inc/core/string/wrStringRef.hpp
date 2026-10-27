#ifndef _WR_STRING_REF_HPP_
#define _WR_STRING_REF_HPP_

// base
#include <wrType.hpp>
#include <wrCompiler.hpp>
#include <wrTemplate.hpp>
// core
#include <core/string/wrU8String.hpp>

namespace wr
{
	class u8_string_ref
	{
	private:
		u8_string str_data;

	public:
		// create empty string
		u8_string_ref() noexcept
		{
			str_data = u8_str_create_null();
		}

		// laod c and c++ utf8 string
		template<char_type ch_t> u8_string_ref(const ch_t* str) noexcept
		{
			const char* ch_in = reinterpret_cast<const char*>(str);
			str_data = u8_str_load_c_str(ch_in, strlen(ch_in) + 1);
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

		// copy
		u8_string_ref(const u8_string_ref& other)
		{
			auto temp_pod = other.get_pod();
			str_data = u8_str_load_all(temp_pod.char_data, temp_pod.char_num, temp_pod.char_data_size);
		}

		// pod in
		void load_pod(u8_string pod_in)
		{
			str_data = pod_in;
		}

		// return c api struct to other progream code c abi function
		const u8_string get_pod() const noexcept
		{
			return str_data;
		}

		// move c api struct to other progream code c style struction
		const u8_string mov_pod() noexcept
		{
			auto temp = str_data;
			release();
			return str_data;
		}

		// cut string include start and exclude end
		u8_string_ref slice(int64_t start, int64_t end) const noexcept
		{
			// NRVO
			return u8_string_ref(u8_str_slice(str_data, start, end));
		}

		// push a utf-8 'char' in string end
		void append(unicode_t ch) noexcept
		{
			str_data = u8_str_append_unicode_char(str_data, ch);
		}

		// push a utf-8 string in string end
		void append(u8_string_ref str) noexcept
		{
			str_data = u8_str_append_u8_str(str_data, str.get_pod());
		}

		// get unicode_t(char32_t) 'char' in string
		unicode_t at(int64_t index) const noexcept { return u8_str_at(str_data, index); }

		// Input sequence is split into tokens, separated by separators
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

		// free memory in this string
		void release() noexcept 
		{ 
			u8_str_release(str_data);
			str_data = { 0 };
		}

		// empty dynamic_array return true
		bool empty() const { return str_data.char_data_size == 0; }

		// get unsafe item data
		utf8_t* mut_data() noexcept { return str_data.char_data; }

		// get unsafe item data
		const utf8_t* const_data() const noexcept { return str_data.char_data; }

		// return c style string
		const char* c_str() { return reinterpret_cast<const char*>(str_data.char_data); }

		~u8_string_ref() { release(); }
	};

} // namespace wr is end

#endif // _WR_STRING_REF_HPP_ IS EOF