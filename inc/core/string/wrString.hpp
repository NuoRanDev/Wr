#ifndef _WR_STRING_HPP_
#define _WR_STRING_HPP_

// std
#include <iostream>
#include <cstring>
#include <ostream>
// core
#include <type/wrOrdinals.hpp>
#include <type/wrDataStruction.hpp>

namespace wr
{
	constexpr int64_t SHORT_STRING_SIZE = 12;

	class U16StringRef;
	class U8StringRef;

	// default string format
	using String = U8StringRef;

	class U8StringRef
	{
		friend class U8StringRef;

	public:

		// Init
		U8StringRef(std::nullptr_t null_type) { this->load_default_str(); }

		U8StringRef() { load_default_str(); }

		U8StringRef(const char* _c_str) noexcept { load_c_str_add0(_c_str); }

		U8StringRef(const utf8_t* str) noexcept { load_cpp_u8_str_add0(str); }

		U8StringRef(const utf8_t* str, int64_t str_size) noexcept { load_seq_no_0_str_add0(str, str_size); }

		U8StringRef(const utf8_t* str, int64_t str_size, int64_t input_character_number) noexcept
		{
			load_wr_str_include0(str, str_size, input_character_number);
		}

		U8StringRef(const U8StringRef& temp_string)
		{
			load_u8stringref(temp_string);
		}

		U8StringRef(U8StringRef&& src) noexcept;

		U8StringRef& operator=(const U8StringRef& temp_string) noexcept 
		{
			this->load_u8stringref(temp_string);
			return *this;
		}

		U8StringRef operator=(U8StringRef&& src) noexcept
		{
			characters_number = src.characters_number;
			characters_data_size = src.characters_data_size;
			characters_data = std::move(src.characters_data);
			src.clear_moved_str();
			return *this;
		}

		U8StringRef& operator=(const char* _c_str) noexcept
		{
			if (_c_str == nullptr)
			{
				this->load_default_str();
				return *this;
			}
			this->load_cpp_u8_str_add0(reinterpret_cast<const utf8_t*>(_c_str));
			return *this;
		}

		U8StringRef& operator=(std::nullptr_t null_type) noexcept
		{
			this->load_default_str();
			return *this;
		}

		U8StringRef& operator=(const utf8_t* c_utf8_str) noexcept
		{
			if (c_utf8_str == nullptr)
			{
				this->load_default_str();
				return *this;
			}
			this->load_cpp_u8_str_add0(c_utf8_str);
			return *this;
		}

		// range: include start exclude end
		[[nodiscard]] U8StringRef slice(int64_t start, int64_t end) const noexcept;

		bool is_empty() const noexcept;

		[[nodiscard]] int64_t get_characters_number() const noexcept { return characters_number; }

		[[nodiscard]] int64_t get_characters_data_size() const noexcept { return characters_data_size; }

		bool erase(int64_t index, int64_t count);

		void append(unicode_t character) noexcept;

		void append(const U8StringRef& append_string) noexcept
		{
			append(append_string.data(), append_string.get_characters_data_size(), append_string.get_characters_number());
		}

		void append(const utf8_t* append_string, int64_t append_size, int64_t append_chacharacters_number) noexcept;

		// Find all

		[[nodiscard]] dynamic_array<int64_t> find_all(const unicode_t pattern) const noexcept;

		[[nodiscard]] dynamic_array<int64_t> find_all(const U8StringRef& pattern) const noexcept;

		[[nodiscard]] dynamic_array<int64_t> find_all(const utf8_t pattern) const noexcept;

		[[nodiscard]] dynamic_array<int64_t> find_all(const utf8_t* pattern, const int64_t pattern_size) const noexcept;

		[[nodiscard]] dynamic_array<int64_t> find_all_ofs(const utf8_t* pattern, const int64_t pattern_size) const noexcept;

		// Find
		int64_t find_start(const U8StringRef& pattern_str) const noexcept { return find_start(pattern_str.data(), pattern_str.get_characters_data_size()); }

		int64_t find_start(const utf8_t* pattern, int64_t pattern_size) const noexcept;

		int64_t find_last(const utf8_t* pattern, int64_t pattern_size) const noexcept;

		// Split string by separator

		[[nodiscard]] dynamic_array<U8StringRef> split(unicode_t separator) noexcept;

		[[nodiscard]] dynamic_array<U8StringRef> split(U8StringRef& separator) noexcept;

		[[nodiscard]] dynamic_array<U8StringRef> 
			split(const utf8_t* separator, int64_t separator_characters_data_size)noexcept;

		[[nodiscard]] dynamic_array<U8StringRef>
			split(dynamic_array<int64_t>& separator_list, int64_t separator_characters_data_size) noexcept;

		[[nodiscard]] unicode_t at(int64_t offset) noexcept;

		// U8StringRef Comparison Operators
		[[nodiscard]] unicode_t operator[](int64_t offset) noexcept { return this->at(offset); }

		// const c type string
		[[nodiscard]] const char* c_str() const noexcept { return (const char*)characters_data; }

		// exclude end of '\0'
		[[nodiscard]] const int64_t length() const noexcept { return characters_data_size - 1; }

		[[nodiscard]] const utf8_t* data()const noexcept { return characters_data; }

		[[nodiscard]] bool stringcmp(const U8StringRef& cmp_str) const noexcept
		{
			return stringcmp(cmp_str.data(), cmp_str.length());
		}

		[[nodiscard]] bool stringcmp(const char* cmp_str) const noexcept
		{
			return stringcmp(reinterpret_cast<const utf8_t*>(cmp_str), std::strlen(cmp_str));
		}

		[[nodiscard]] bool stringcmp(const utf8_t* cmp_str) const noexcept
		{
			return stringcmp(reinterpret_cast<const utf8_t*>(cmp_str), std::strlen(reinterpret_cast<const char*>(cmp_str)));
		}

		[[nodiscard]] bool stringcmp(const utf8_t* cmp_str, const int64_t cmp_str_size) const noexcept;

		bool operator<(const U8StringRef& src) const noexcept
		{
			return this->characters_data_size < src.characters_data_size;
		}

		bool operator==(const U8StringRef& src) const noexcept
		{
			if ((this->is_empty()) ^ (src.is_empty()))
				return false;
			return this->stringcmp(src.data(), src.characters_data_size);
		}

		template<IntegralType T> void integral_to_string(T number) noexcept
		{
			utf8_t characters[sizeof(T)] = { 0 };
			int64_t str_size = 0;

			integral_to_string_ptr(static_cast<int64_t>(number), characters, str_size);
			load_data(characters, str_size, str_size);
		}

		inline U8StringRef operator+(const U8StringRef& string) noexcept
		{
			this->append(string);
			return *this;
		}

		U8StringRef operator+(const utf8_t* string) noexcept;

		void release() noexcept;

		~U8StringRef() { release(); }

		// default load data
		void load_default_str() noexcept;
		// load other wr string
		void load_wr_str_include0(const utf8_t* wr_utf8_str, int64_t wr_str_size, int64_t input_character_number) noexcept;
		// load cpp char8_t style utf8 string
		void load_cpp_u8_str_add0(const utf8_t* c_utf8_str) noexcept;
		// load seq_no_0 string
		void load_seq_no_0_str_add0(const utf8_t* wr_seq_no_0_str, int64_t seq_no_0_str_size);
		// load c style string
		void load_c_str_add0(const char* _c_str) noexcept
		{
			load_cpp_u8_str_add0(reinterpret_cast<const utf8_t*>(_c_str));
		}
		// laod this class
		void load_u8stringref(const U8StringRef& temp_string)
		{
			if (temp_string.is_empty())
			{
				load_default_str();
				return;
			}
			load_wr_str_include0(temp_string.data(), temp_string.get_characters_data_size(), temp_string.get_characters_number());
		}

	private:

		// data ptr
		utf8_t* characters_data;
		// string size
		int64_t characters_number;
		// alloc size
		int64_t characters_data_size;

		void clear_moved_str() noexcept;

		bool string_long_cmp(const U8StringRef& cmp_str) const noexcept
		{
			return string_long_cmp(cmp_str.data(), cmp_str.length());
		}

		bool string_long_cmp(const char* cmp_str) const noexcept
		{
			return string_long_cmp(reinterpret_cast<const utf8_t*>(cmp_str), std::strlen(cmp_str));
		}

		bool string_long_cmp(const utf8_t* cmp_str) const noexcept
		{
			return string_long_cmp(reinterpret_cast<const utf8_t*>(cmp_str), std::strlen(reinterpret_cast<const char*>(cmp_str)));
		}

		bool string_long_cmp(const utf8_t* cmp_str, int64_t cmp_str_size) const noexcept;

		bool string_short_cmp(const U8StringRef& cmp_str) noexcept
		{
			return string_short_cmp(cmp_str.data(), cmp_str.length());
		}

		bool string_short_cmp(const char* cmp_str) noexcept
		{
			return string_short_cmp(reinterpret_cast<const utf8_t*>(cmp_str), std::strlen(cmp_str));
		}

		bool string_short_cmp(const utf8_t* cmp_str) noexcept
		{
			return string_short_cmp(reinterpret_cast<const utf8_t*>(cmp_str), std::strlen(reinterpret_cast<const char*>(cmp_str)));
		}

		bool string_short_cmp(const utf8_t* cmp_str, int64_t cmp_str_size) const noexcept;

		void integral_to_string_ptr(int64_t number, utf8_t* str, int64_t& str_size) noexcept;

		// input utf-8 index out data offset
		int64_t get_u8_data_ofs(int64_t index, utf8_t* cur_str_ptr);
		// 
		bool is_short_string;
	};

	// std::cout << <U8StringRef>
	[[nodiscard]] inline std::ostream& operator<<(std::ostream& out, const U8StringRef& string)
	{
		auto str = string.c_str();
		if (str == nullptr)
			out << "";
		else
		{
			out << str;
		}
		return out;
	}

	class U16StringRef
	{
		friend class U16StringRef;

	public:

		U16StringRef() 
		{
			characters_data = nullptr;
			characters_number = 0;
		}

		U16StringRef(const U16StringRef& other) noexcept;

		U16StringRef(U16StringRef&& other) noexcept;

		U16StringRef operator=(const U16StringRef& other) { return U16StringRef(other); }

		U16StringRef(const U8StringRef& u8_str) noexcept { load_utf8(u8_str); }

		bool load_utf8(const U8StringRef& u8_str) noexcept;

		bool to_utf8(U8StringRef& u8_dst) const noexcept;

		void load_utf16_by_count(const utf16le_t* data, int64_t conut) noexcept;

		void load_utf16_string(const U16StringRef& other) noexcept
		{
			load_utf16_by_count(other.data(), other.characters_number);
		}

		const utf16le_t* data() const noexcept { return characters_data; }

		// String's _size is include 0
		void ptr_resize(int64_t _size) noexcept;

		int64_t count() const { return characters_number; }

#ifdef _WIN32
		utf16le_t* get_win32_str_pdata() { return characters_data; }
#endif // _WIN32

		void release() noexcept;

		void clear_moved_str() noexcept;

		~U16StringRef() { release(); }
	
	private:

		utf16le_t* characters_data;
		// include 0
		int64_t characters_number;
	};
} // namespace wr is end

#endif // _WR_STRING_HPP_ IS EOF