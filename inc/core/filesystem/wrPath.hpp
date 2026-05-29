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

		const OS_CHAR* get_native_str() const { return reinterpret_cast<const OS_CHAR*>(n_str.data()); }

	private:

		friend class Path;

		OS_STRING n_str;
	}; // namespace Path is end

} // namespace wr is end

#endif // !_WR_PATH_HPP_