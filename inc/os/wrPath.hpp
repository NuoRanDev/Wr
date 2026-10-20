#ifndef _WR_PATH_HPP_
#define _WR_PATH_HPP_

// core
#include <core/wrString.hpp>
// os
#if defined(_WIN32)
#include <os/winows/wrWinowsPath.hpp>
#endif // OS


namespace wr
{
	class path
	{
	public:
		path(wr_string& init_path);

		wr_string to_str() const noexcept;

		bool exists() const noexcept;

		int64_t get_create_time() const noexcept;

		u8_string abs_path() const noexcept;

		size_t get_size() const noexcept;

		bool is_abs() const noexcept;

		bool empty() const noexcept;

		~path();

	private:

		OS_STRING native_str_path;
	};
} // namespace wr is end

#endif // _WR_PATH_HPP_ IS EOF