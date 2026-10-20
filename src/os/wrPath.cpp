// statement
#include <os/wrPath.hpp>

// os
#if defined(_WIN32)
#include <os/winows/wrWinowsPath.hpp>
using namespace wr::windows;
#endif // OS

namespace wr
{
	path::path(wr_string& init_path)
	{
		native_str_path = to_native_str(init_path.get_pod());
	}

	wr_string path::to_str() const noexcept
	{
		return wr_string(to_wr_string(native_str_path));
	}

	bool path::exists() const noexcept
	{
		return os_exists(native_str_path);
	}

	int64_t path::get_create_time() const noexcept
	{
		return os_get_create_time(native_str_path);
	}

	u8_string path::abs_path() const noexcept
	{
		return u8_string(os_abs_path(native_str_path));
	}

	size_t path::get_size() const noexcept
	{
		return os_get_size(native_str_path);
	}

	bool path::is_abs() const noexcept
	{
		return os_is_abs(native_str_path);
	}

} // namespace wr is end