#ifdef _WIN32

// statement
#include <wrFileDialog.hpp>
// os api
#include <Windows.h>
#include <commdlg.h>
#include <ShlObj.h>
// std
#include <wchar.h>

namespace wr
{
	namespace FileDialog
	{
		Path open_file(Path& default_path, String& filter, String& title, any_type_ptr_t window_hwnd)
		{
			wchar_t out_file_name[MAX_PATH] = { 0 };
			U16StringRef u16_filter = U16StringRef(filter);
			U16StringRef u16_path = default_path.get_native_str();
			U16StringRef u16_title = U16StringRef(title);
			OPENFILENAME ofn = { 0 };
			U16StringRef ofs;

			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = reinterpret_cast<HWND>(window_hwnd);
			ofn.nFilterIndex = 1;
			ofn.lpstrFile = out_file_name;
			ofn.nMaxFile = MAX_PATH;

			if (filter.is_empty())
			{
				ofn.lpstrFilter = L"all files\0*.*\0\0";
			}
			else 
			{
				ofn.lpstrFilter = u16_filter.data();
			}

			if (default_path.is_empty())
				ofn.lpstrInitialDir = u16_path.data();
			else
			{
				ofn.lpstrInitialDir = L"";
			}
			if (!title.is_empty())
			{
				ofn.lpstrTitle = u16_title.data();
			}

			ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
			if (GetOpenFileNameW(&ofn))
			{
				ofs.load_utf16_by_count(out_file_name, wcslen(out_file_name));
				return Path(ofs);
			}
			return Path(ofs);
		}
	}
} // namespace wr is end

#endif // _WIN32