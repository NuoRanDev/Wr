#if defined(_WIN32)

// statement
#include <os/windows/wrWindowsMmap.hpp>
// os api
#include <Windows.h>
// os
#include <os/wrLogOutput.hpp>
#include <os/windows/wrWinowsPath.hpp>
// std
#include <format>

namespace wr
{
	namespace windows
	{
		WR_RESULT os_mmap_open_file(mmap_file_info* mf, const os_string path)
		{
			mf->file_size = os_get_file_size(path);
			mf->c_dump_file_descriptor = CreateFileW(
				path.char_data,
				GENERIC_READ,
				0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (mf->c_dump_file_descriptor == nullptr)
			{
				WR_WARNING_OUTPUT(
					WR_TYPE_NAME_OUTPUT::APP,
					"wrCore",
					std::format("Create file mapping failed , SYSTEM ERROR CODE:{0}", GetLastError()).c_str());
				return WR_RESULT::WR_SYSTEM_CALL_ERROR;
			}
			mf->hfile_mapping = CreateFileMapping(mf->c_dump_file_descriptor, NULL, PAGE_READONLY, 0, 0, NULL);
			if (mf->hfile_mapping == nullptr)
			{
				WR_WARNING_OUTPUT(
					WR_TYPE_NAME_OUTPUT::APP,
					"wrCore",
					std::format("Open file mapping failed , SYSTEM ERROR CODE:{0}", GetLastError()).c_str());
				return WR_RESULT::WR_SYSTEM_CALL_ERROR;
			}
			mf->pfile_start = MapViewOfFile(mf->hfile_mapping, FILE_MAP_READ, 0, 0, 0);
			if (mf->pfile_start == nullptr)
			{
				WR_WARNING_OUTPUT(
					WR_TYPE_NAME_OUTPUT::APP,
					"wrCore",
					std::format("Map file mapping failed ,SYSTEM ERROR CODE:{0}", GetLastError()).c_str());
				CloseHandle(mf->hfile_mapping);
				return WR_RESULT::WR_SYSTEM_CALL_ERROR;
			}
			return WR_RESULT::WR_OK;
		}

		WR_RESULT os_mmap_close_file(mmap_file_info* mf)
		{
			if (mf->pfile_start != nullptr)
				UnmapViewOfFile(mf->pfile_start);
			if (mf->hfile_mapping != nullptr)
				CloseHandle(mf->hfile_mapping);
			if (mf->c_dump_file_descriptor != nullptr)
				CloseHandle(mf->c_dump_file_descriptor);
			mf->pfile_start = nullptr;
			mf->hfile_mapping = nullptr;
			mf->c_dump_file_descriptor = nullptr;
			mf->file_size = 0;
			return WR_RESULT::WR_OK;
		}
	} // namespace windows is end
} // namespace wr is end

#endif // os