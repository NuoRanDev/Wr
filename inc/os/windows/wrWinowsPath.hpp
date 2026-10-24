#ifndef _WR_WINDOWS_PATH_HPP_
#define _WR_WINDOWS_PATH_HPP_

#if defined(_WIN32)

// base
#include <wrType.hpp>
#include <wrCompiler.hpp>
// core
#include <core/wrString.hpp>
#include <core/wrPair.hpp>

namespace wr
{
	using os_string = u16_string;

	namespace windows
	{

		c_extern exportfunc u16_string os_to_path_native_str(const u8_string path) noexcept;

		c_extern exportfunc u8_string os_path_str_to_wr_string(const u16_string path) noexcept;

		c_extern exportfunc bool os_path_exists(const u16_string path) noexcept;

		c_extern exportfunc int64_t os_path_get_file_create_time(const u16_string path) noexcept;

		c_extern exportfunc u8_string os_path_abs(const u16_string path) noexcept;

		c_extern exportfunc u8_string os_path_base_name(const u16_string path) noexcept;

		c_extern exportfunc u8_string os_path_dir_name(const u16_string path) noexcept;

		c_extern exportfunc size_t os_get_file_size(const u16_string path) noexcept;

		c_extern exportfunc bool os_is_path_is_abs(const u16_string path) noexcept;

		c_extern exportfunc bool os_is_path_is_dir(const u16_string path) noexcept;

		c_extern exportfunc bool os_is_path_is_file(const u16_string path) noexcept;

		c_extern exportfunc pair<u8_string, u8_string> os_path_split(const u16_string path) noexcept;

		c_extern exportfunc void os_path_release_str(const u16_string path) noexcept;

		constexpr bool os_path_str_is_empty(const u16_string path) { return u16le_str_is_empty(path); }
	} // namespace windows is end
} // namespace wr is end
#endif // OS

#endif // _WR_WINDOWS_PATH_HPP_ IS EOF