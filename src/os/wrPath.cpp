// statement
#include <os/wrPath.hpp>

// os
#if defined(_WIN32)
#include <os/windows/wrWinowsPath.hpp>
using namespace wr::windows;
#endif // OS

namespace wr
{
	path::path(wr_string& init_path)
	{
		native_str_path = os_to_path_native_str(init_path.get_pod());
	}

	wr_string path::to_str() const noexcept
	{
		return wr_string(os_path_str_to_wr_string(native_str_path));
	}

	bool path::exists() const noexcept
	{
		return os_path_exists(native_str_path);
	}

	int64_t path::get_create_time() const noexcept
	{
		return os_path_get_file_create_time(native_str_path);
	}

	wr_string path::abs_path() const noexcept
	{
		return wr_string(os_path_abs(native_str_path));
	}

	wr_string path::base_name() const noexcept
	{
		return wr_string(os_path_base_name(native_str_path));
	}

	wr_string path::dir_name() const noexcept
	{
		return wr_string(os_path_dir_name(native_str_path));
	}

	pair<wr_string, wr_string> path::split() const noexcept
	{
		pair<wr_string, wr_string> out;
		pair<u8_string, u8_string> os_out = os_path_split(native_str_path);
		out.key.load_pod(os_out.key);
		out.vale.load_pod(os_out.vale);
		return out;
	}

	size_t path::get_size() const noexcept
	{
		return os_get_file_size(native_str_path);
	}

	bool path::is_abs() const noexcept
	{
		return os_is_path_is_abs(native_str_path);
	}

	bool path::empty() const noexcept
	{
		return os_path_str_is_empty(native_str_path);
	}

	path::~path()
	{
		os_path_release_str(native_str_path);
	}

} // namespace wr is end