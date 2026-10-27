#if defined(_WIN32)
// statement
#include <os/windows/wrWinowsPath.hpp>
// os
#include <os/wrLogOutput.hpp>
#include <os/wrAlloc.hpp>
// os api
#include <Windows.h>
#include <shlwapi.h>
#include <wchar.h>
#include <PathCch.h>
// core
#include <core/wrString.hpp>
// dev
#include <dev/wrMemory.hpp>
// base
#include <wrCompiler.hpp>
// std
#include <format>

namespace wr
{
	namespace windows
	{
		u16_string os_to_path_native_str(const u8_string path) noexcept
		{
			return u16le_str_load_utf8_str(path);
		}

		u8_string os_path_str_to_wr_string(const u16_string path) noexcept
		{
			return u16le_str_to_utf8_str(path);
		}
		
		bool os_path_exists(const u16_string path) noexcept
		{
			return static_cast<bool>(PathFileExistsW(path.char_data));
		}

		int64_t os_path_get_file_create_time(const u16_string path) noexcept
		{
			FILETIME create_time;
			ULARGE_INTEGER ui;
			WIN32_FILE_ATTRIBUTE_DATA wfad;

			if (!os_path_exists(path))
			{
				WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrFilesystem",
					"File/Dir is not existsed");
				return -1;
			}
			if (GetFileAttributesExW(path.char_data, GetFileExInfoStandard, &wfad))
			{
				create_time = wfad.ftCreationTime;
				ui.LowPart = create_time.dwLowDateTime;
				ui.HighPart = create_time.dwHighDateTime;
				return ((std::time_t)(ui.QuadPart - 116444736000000000) / 10000000);
			}
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrFilesystem",
				std::format("Get file attributses failed , SYSTEM ERROR CODE:{0}", GetLastError()).c_str());
			return -1;
		}

		u8_string os_path_abs(const u16_string path) noexcept
		{
			utf16le_t file_path[MAX_PATH];
			int64_t str_count = GetFullPathNameW(path.char_data, MAX_PATH, file_path, nullptr);
			u16_string u16_tmp =
			{
				.char_number = str_count,
				.char_data = file_path
			};
			return u16le_str_to_utf8_str(u16_tmp);
		}

		u8_string os_path_base_name(const u16_string path) noexcept
		{
			utf16le_t* pu16temp = wr_malloc<utf16le_t>(path.char_number);
			//wr_memset(0, path.char_number * 2, pu16temp);
			wr_memcpy(pu16temp, path.char_data, path.char_number * 2);
			PathStripPathW(pu16temp);
			int64_t utf16_size = static_cast<int64_t>(lstrlenW(pu16temp) + 1);
			u16_string temp_str =
			{
				.char_number = utf16_size,
				.char_data = pu16temp
			};
			u8_string out = u16le_str_to_utf8_str(temp_str);
			temp_str = u16le_str_release(temp_str);
			return out;
		}

		u8_string os_path_dir_name(const u16_string path) noexcept
		{
			utf16le_t* pu16temp = wr_malloc<utf16le_t>(path.char_number);
			//wr_memset(0, path.char_number * 2, pu16temp);
			wr_memcpy(pu16temp, path.char_data, path.char_number * 2);
			PathCchRemoveFileSpec(pu16temp, path.char_number - 1);
			int64_t utf16_size = static_cast<int64_t>(lstrlenW(pu16temp) + 1);
			u16_string temp_str =
			{
				.char_number = utf16_size,
				.char_data = pu16temp
			};
			u8_string out = u16le_str_to_utf8_str(temp_str);
			temp_str = u16le_str_release(temp_str);
			return out;
		}

		size_t os_get_file_size(const u16_string path) noexcept
		{
			LARGE_INTEGER info;
			WIN32_FILE_ATTRIBUTE_DATA wfad;

			if (!os_path_exists(path))
			{
				WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrFilesystem",
					"File/Dir is not existsed");
				return 0;
			}
			if (GetFileAttributesExW(path.char_data, GetFileExInfoStandard, &wfad))
			{
				info.HighPart = wfad.nFileSizeHigh;
				info.LowPart = wfad.nFileSizeLow;
				return static_cast<uint64_t>(info.QuadPart);
			}
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrFilesystem",
				std::format("Get file attributses failed , SYSTEM ERROR CODE:{0}", GetLastError()).c_str());
			return 0;
		}

		bool os_is_path_is_abs(const u16_string path) noexcept
		{
			return (L'A' <= path.char_data[0] && path.char_data[0] <= L'Z');;
		}

		bool os_is_path_is_dir(const u16_string path) noexcept
		{
			DWORD dwat = GetFileAttributesW(path.char_data);
			if (dwat == INVALID_FILE_ATTRIBUTES)
			{
				WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrFilesystem", "Path is not exist");
			}
			return dwat & FILE_ATTRIBUTE_DIRECTORY;
		}

		bool os_is_path_is_file(const u16_string path) noexcept
		{
			return !os_is_path_is_dir(path);
		}

		pair<u8_string, u8_string> os_path_split(const u16_string path) noexcept
		{
			pair<u8_string, u8_string> out = { };
			if (!os_path_exists(path)) [[unlikely]]
				return out;
			else [[likely]]
			{
				if (os_is_path_is_dir(path)) [[unlikely]]
				{
					out.key = os_path_dir_name(path);
					return out;
				}
				else [[likely]]
				{
					out.key = os_path_dir_name(path);
					out.vale = os_path_base_name(path);
					return out;
				}
			}
		}

		void os_path_release_str(const u16_string path) noexcept
		{
			u16le_str_release(path);
		}
	} // namespace windows is end
} // namespace wr is end
#endif