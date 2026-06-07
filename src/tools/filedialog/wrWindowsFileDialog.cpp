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
		dynamic_array<utf16le_t> to_lpstrFilter(dynamic_array<Filter>& filter_list)
		{
			dynamic_array<utf16le_t> out;
			for (const auto& filter : filter_list)
			{
				if (!(filter.extension_info.is_empty()))
				{
					U16StringRef u16c = filter.extension_info;
					out.append_range(dynamic_array<utf16le_t>(u16c.data(), u16c.data() + (u16c.count() - 1)));
					out.push_back(u'\0');
				}
				for (const auto& extension : filter.extension_list)
				{
					U16StringRef u16c = extension;
					out.append_range(dynamic_array<utf16le_t>(u16c.data(), u16c.data() + (u16c.count() - 1)));
					out.push_back(u';');
				}
				out.push_back(u'\0');
				out.push_back(u'\0');
			}
			out.push_back(u'\0');
			return out;
		}

		Path open_file(Path& default_path, dynamic_array<Filter>& filter_list, String& title, any_type_ptr_t window_hwnd)
		{
			wchar_t out_file_name[MAX_PATH] = { 0 };
			U16StringRef u16_path = default_path.get_native_str();
			U16StringRef u16_title = U16StringRef(title);
			OPENFILENAME ofn = { 0 };
			U16StringRef ofs;
			dynamic_array<utf16le_t> filter_str;

			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = reinterpret_cast<HWND>(window_hwnd);
			ofn.nFilterIndex = 1;
			ofn.lpstrFile = out_file_name;
			ofn.nMaxFile = MAX_PATH;

			if (filter_list.empty())
			{
				ofn.lpstrFilter = TEXT("all files\0*.*\0\0");
			}
			else 
			{
				filter_str = to_lpstrFilter(filter_list);
				ofn.lpstrFilter = filter_str.data();
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
			if (GetOpenFileName(&ofn))
			{
				ofs.load_utf16_by_count(out_file_name, wcslen(out_file_name));
				return Path(ofs);
			}
			return Path(ofs);
		}
	}
} // namespace wr is end

#endif // _WIN32