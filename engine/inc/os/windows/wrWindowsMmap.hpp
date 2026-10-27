#ifndef _WR_WINDOWS_MMAP_HPP_
#define _WR_WINDOWS_MMAP_HPP_

// base
#include <wrResult.hpp>
#include <wrCompiler.hpp>
// core
#include <core/wrPath.hpp>
// base
#include <wrType.hpp>

namespace wr
{
	namespace windows
	{
		struct mmap_file_info
		{
			// is start
			any_type_ptr_t pfile_start;

			void* hfile_mapping;
			void* c_dump_file_descriptor;
			size_t file_size;
		};

		c_extern exportfunc WR_RESULT os_mmap_open_file(mmap_file_info* mf, const os_string path) noexcept;

		c_extern exportfunc WR_RESULT os_mmap_close_file(mmap_file_info* mf) noexcept;
	} // namespace windows is end
} // namespace wr is end

#endif // _WR_WINDOWS_MMAP_HPP_ IS EOF