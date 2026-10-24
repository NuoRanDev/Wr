#ifndef _WR_PATH_HPP_
#define _WR_PATH_HPP_

// base
#include <wrCompiler.hpp>
// core
#include <core/wrString.hpp>
#include <core/wrPair.hpp>
// os
#if defined(_WIN32)
#include <os/windows/wrWinowsPath.hpp>
#endif // OS
#include <os/wrOsInfo.hpp>

namespace wr
{
	class path
	{
	public:
		path(wr_string& init_path)
		{
			native_str_path = os_namespace::os_to_path_native_str(init_path.get_pod());
		}

		wr_string to_str() const noexcept
		{
			return wr_string(os_namespace::os_path_str_to_wr_string(native_str_path));
		}

		bool exists() const noexcept
		{
			return os_namespace::os_path_exists(native_str_path);
		}

		int64_t get_create_time() const noexcept
		{
			return os_namespace::os_path_get_file_create_time(native_str_path);
		}

		wr_string abs_path() const noexcept
		{
			return wr_string(os_namespace::os_path_abs(native_str_path));
		}

		wr_string base_name() const noexcept
		{
			return wr_string(os_namespace::os_path_base_name(native_str_path));
		}

		wr_string dir_name() const noexcept
		{
			return wr_string(os_namespace::os_path_dir_name(native_str_path));
		}

		pair<wr_string, wr_string> split() const noexcept
		{
			pair<wr_string, wr_string> out;
			pair<u8_string, u8_string> os_out = os_namespace::os_path_split(native_str_path);
			out.key.load_pod(os_out.key);
			out.vale.load_pod(os_out.vale);
			return out;
		}

		size_t get_size() const noexcept
		{
			return os_namespace::os_get_file_size(native_str_path);
		}

		bool is_abs() const noexcept
		{
			return os_namespace::os_is_path_is_abs(native_str_path);
		}

		bool empty() const noexcept
		{
			return os_namespace::os_path_str_is_empty(native_str_path);
		}

		const os_string get_os_str() const noexcept { return native_str_path; }

		~path()
		{
			os_namespace::os_path_release_str(native_str_path);
		}

	private:

		os_string native_str_path;
	};
} // namespace wr is end

#endif // _WR_PATH_HPP_ IS EOF