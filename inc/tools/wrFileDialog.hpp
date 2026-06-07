#ifndef _INC_WR_FILE_DIALOG_HPP_
#define _INC_WR_FILE_DIALOG_HPP_

// core
#include <filesystem/wrFilesystem.hpp>

namespace wr
{
	namespace FileDialog
	{
		struct Filter
		{
			String extension_info;
			dynamic_array<String> extension_list;
		};

		Path open_file(Path& default_path, dynamic_array<Filter>& filter_list, String& title, any_type_ptr_t window_hwnd);
	} // namespace FileDialog is end
} // namespace wr is end

#endif // _INC_WR_FILE_DIALOG_HPP_ IS EOF