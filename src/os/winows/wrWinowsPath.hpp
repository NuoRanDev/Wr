#ifndef _WR_WINDOWS_PATH_HPP_
#define _WR_WINDOWS_PATH_HPP_

#if defined(_WIN32)

// base
#include <wrType.hpp>
// core
#include <core/wrString.hpp>

namespace wr
{
	using OS_STRING = u16_string;

	namespace windows
	{

		u16_string to_native_str(const u8_string path)
		{
			return u16le_str_load_utf8_str(path);
		}

		u8_string to_wr_string(u16_string path)
		{
			return u16le_str_to_utf8_str(path);
		}

		bool os_exists(const u16_string path) noexcept;

		int64_t os_get_create_time(const u16_string path) noexcept;

		u8_string os_abs_path(const u16_string path) noexcept;

		u8_string os_base_name(const u16_string path) noexcept;

		size_t os_get_size(const u16_string path) noexcept;

		bool os_is_abs(const u16_string path) noexcept;

		bool os_is_dir(const u16_string path) noexcept;

		bool os_is_file(const u16_string path) noexcept;
	} // namespace windows is end
} // namespace wr is end
#endif // OS

#endif // _WR_WINDOWS_PATH_HPP_ IS EOF