#ifndef _WR_PATH_HPP_
#define _WR_PATH_HPP_

// core
#include <string/wrString.hpp>

namespace wr
{

#if defined(_WIN32)
	using OS_STRING = U16StringRef;
	using OS_CHAR = utf16le_t;
#else
	using OS_STRING = U8StringRef;
	using OS_CHAR = char;
#endif // defined(_WIN32) IS END

	class Path
	{
	public:
		Path() = default;

		Path(const String &path) noexcept;

		Path(Path&& other) noexcept;

#if defined(_WIN32)

		Path(const U16StringRef& path) noexcept { n_str.load_utf16_string(path); }

#endif // defined(_WIN32) is end

		String abs_path() const noexcept;

		String base_name() noexcept;

		bool exists() const noexcept;

		std::time_t get_create_time() const noexcept;

		uint64_t get_size() const noexcept;

		bool is_abs() const noexcept;

		bool is_dir() const noexcept;

		bool is_file() const noexcept;

		std::pair<String, String> split() const noexcept;

		const String to_string() const noexcept
		{
			String out_str;
#if defined(_WIN32)
			if (n_str.to_utf8(out_str))
				return out_str;
			out_str.load_default_str();
			return out_str;
#else
			return out_str.load_u8stringref(n_str);
#endif // defined(_WIN32) is end
		}

		bool is_empty() const noexcept
		{
			return n_str.data() == nullptr;
		}

		const OS_CHAR* get_native_str_data() const { return reinterpret_cast<const OS_CHAR*>(n_str.data()); }

		OS_STRING get_native_str() { return OS_STRING(n_str); }

	private:

		friend class Path;

		OS_STRING n_str;
	}; // namespace Path is end

} // namespace wr is end

#endif // !_WR_PATH_HPP_