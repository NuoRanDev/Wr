#ifndef _WR_FILE_MMAP_VIEWER_HPP_
#define _WR_FILE_MMAP_VIEWER_HPP_

// base
#include <wrType.hpp>
#include <wrResult.hpp>
// os
#include <os/wrPath.hpp>
#if defined(_WIN32)
#include <os/windows/wrWindowsMmap.hpp>
#endif // OS
#include <os/wrOsInfo.hpp>


namespace wr
{
	class mmap_file_viewer
	{
	public:
		mmap_file_viewer() = default;

		WR_RESULT open_file(const path& mmap_file_path)
		{
			return os_namespace::os_mmap_open_file(&file_info, mmap_file_path.get_os_str());
		}
		
		void close_file()
		{
			os_namespace::os_mmap_close_file(&file_info);
		}

		~mmap_file_viewer()
		{
			close_file();
		}

	//private:
		os_namespace::mmap_file_info file_info;
	};
} // namespace wr is end

#endif // _WR_FILE_MMAP_VIEWER_HPP_ IS EOF