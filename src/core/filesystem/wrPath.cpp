// statement
#include <filesystem/wrPath.hpp>
// core
#include <memory/wrAlloc.hpp>
#include <log/wrLogOutput.hpp>
// os api
#if defined(_WIN32)
#include <Windows.h>
#include <shlwapi.h>
#include <wchar.h>
#elif defined(__linux__)
#include <unistd.h>
#include <sys/stat.h> 
#endif
// std
#include <format>

#if defined(_WIN32)
#else
#endif // OS

namespace wr
{
	Path::Path(const String& path) noexcept
	{
#if defined(_WIN32)
		n_str.load_utf8(path);
#elif defined(__linux__)
		n_str = String(path.data(), path.get_characters_data_size(), path.get_characters_number());
#endif // init windows native str
	}

	Path::Path(Path&& other) noexcept : n_str(std::move(other.n_str)) {}

	String Path::abs_path() const noexcept
	{
#if defined(_WIN32)
		U16StringRef u16str;
		utf16le_t file_path[MAX_PATH];
		int64_t str_count = GetFullPathNameW(n_str.data(), MAX_PATH, file_path, nullptr);
		u16str.load_utf16_by_count(file_path, str_count);
		String u8str{};
		u16str.to_utf8(u8str);
		puts(u8str.c_str());
		return u8str;
#elif defined(__linux__)
		return String(realpath(n_str.c_str(), nullptr));
#endif // OS
	}

	String Path::base_name() noexcept
	{
#if defined(_WIN32)
		String out{};
		PathStripPathW(n_str.get_win32_str_pdata());
		n_str.ptr_resize(std::wcslen(n_str.data()) + 1);
		n_str.to_utf8(out);
		return out;
#elif defined(__linux__)
		return String(basename(n_str.c_str()));
#endif // OS
	}

	bool Path::exists() const noexcept
	{
#if defined(_WIN32)
		return static_cast<bool>(PathFileExistsW(n_str.data()));
#elif defined(__linux__)
		return access(n_str.c_str(), F_OK) == 0;
#endif // OS
	}

	std::time_t Path::get_create_time() const noexcept
	{
#if defined(_WIN32)
		FILETIME create_time;
		ULARGE_INTEGER ui;
		WIN32_FILE_ATTRIBUTE_DATA wfad;

		if (!exists())
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrFilesystem",
				"File/Dir is not existsed");
			return -1;
		}
		if (GetFileAttributesExW(n_str.data(), GetFileExInfoStandard, &wfad))
		{
			create_time = wfad.ftCreationTime;
			ui.LowPart = create_time.dwLowDateTime;
			ui.HighPart = create_time.dwHighDateTime;
			return ((std::time_t)(ui.QuadPart - 116444736000000000) / 10000000);
		}
		WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrFilesystem",
			std::format("Get file attributses failed , SYSTEM ERROR CODE:{0}", GetLastError()).c_str());
		return -1;
#elif defined(__linux__)
		struct stat file_stat;
		if (stat(n_str.c_str(), &file_stat) != 0)
		{
			return 0;
		}
		return file_stat.st_ctime;
#endif // OS
	}

	uint64_t Path::get_size() const noexcept
	{
#if defined(_WIN32)
		LARGE_INTEGER info;
		WIN32_FILE_ATTRIBUTE_DATA wfad;

		if (!exists())
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrFilesystem",
				"File/Dir is not existsed");
			return 0;
		}
		if (GetFileAttributesExW(n_str.data(), GetFileExInfoStandard, &wfad))
		{
			info.HighPart = wfad.nFileSizeHigh;
			info.LowPart = wfad.nFileSizeLow;
			return static_cast<uint64_t>(info.QuadPart);
		}
		WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrFilesystem",
			std::format("Get file attributses failed , SYSTEM ERROR CODE:{0}", GetLastError()).c_str());
		return 0;
#elif defined(__linux__)
		struct stat file_stat;
		if (stat(n_str.c_str(), &file_stat) != 0)
		{
			return 0;
		}
		return static_cast<uint64_t>(file_stat.st_size);
#endif // OS
	}

	bool Path::is_abs() const noexcept
	{
#if defined(_WIN32)
		return ('A' <= n_str.data()[0] && n_str.data()[0] <= 'Z');
#elif defined(__linux__)
		return n_str.data()[0] == '/';
#endif // OS
	}

	bool Path::is_dir() const noexcept
	{
#if defined(_WIN32)
		DWORD dwat = GetFileAttributesW(n_str.data());
		if (dwat == INVALID_FILE_ATTRIBUTES)
		{
			WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrFilesystem", "Path is not exist");
		}
		return dwat & FILE_ATTRIBUTE_DIRECTORY;
#elif defined(__linux__)
		struct stat file_stat;
		if (stat(n_str.c_str(), &file_stat) != 0)
		{
			return false;
		}
		return S_ISDIR(file_stat.st_mode);
#endif // OS
	}

	bool Path::is_file() const noexcept
	{
#if defined(_WIN32)
		return !is_dir();
#elif defined(__linux__)
		struct stat file_stat;
		if (stat(n_str.c_str(), &file_stat) != 0)
		{
			return false;
		}
		return S_ISREG(file_stat.st_mode);
#endif // OS
	}

	std::pair<String, String> Path::split() const noexcept
	{
		String base_name_str{};
		String dir_name_str{};
#if defined(_WIN32)
		if (!exists()) return { dir_name_str, base_name_str };
		if (is_dir()) 
		{
			n_str.to_utf8(dir_name_str);
			return { dir_name_str, base_name_str };
		}
		else
		{
			int64_t i;
			U16StringRef temp_str;
			for (i = n_str.count() - 1; i >= 0; i--)
			{
				if (((n_str.data())[i] == L'\\') ||
					((n_str.data())[i] == L'/'))
				{
					break;
				}
			}
			temp_str.load_utf16_by_count(n_str.data(), i);
			temp_str.to_utf8(dir_name_str);
			temp_str.load_utf16_by_count(n_str.data() + i + 1, n_str.count() - i - 1);
			temp_str.to_utf8(base_name_str);
			return { dir_name_str, base_name_str };
		}
#elif defined(__linux__)

		if (!exists()) return { dir_name_str, base_name_str };
		if (is_dir()) return { dir_name_str(n_str), base_name_str };
		else
		{
			String base_name_str = base_name();
			String dir_name_str = n_str.slice(0, n_str.get_characters_number() - base_name_str.get_characters_number());
			return { dir_name_str, base_name_str };
		}

#endif // OS
	}
} // namespace wr is end