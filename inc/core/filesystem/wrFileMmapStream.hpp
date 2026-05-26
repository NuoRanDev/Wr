#ifndef _INC_WR_FILE_MMAP_STREAM_HPP_
#define _INC_WR_FILE_MMAP_STREAM_HPP_

#include "type/wrOrdinals.hpp"
#include "string/wrString.hpp"
#include "memory/wrAlloc.hpp"
#include "filesystem/wrPath.hpp"

#include <format>
#if defined(__linux__)
#include <sys/stat.h>
#endif

namespace wr
{
	class Mmapfstream final
	{
	public:

		Mmapfstream() noexcept = default;

		Mmapfstream(const Mmapfstream&) noexcept = default;

		Mmapfstream& operator=(const Mmapfstream&) noexcept = default;

		Mmapfstream& operator=(Mmapfstream&&) noexcept = default;

		// dircetly get file data pointer

		const byte_t* get_mmap_ptr() const noexcept { return (const byte_t*)pfile_start; }

		const byte_t* get_mmap_offset_ptr(size_t offset_byte) const noexcept;

		// read file data to dst (copy)

		bool read(size_t offset_byte, size_t data_size, any_type_ptr_t dst) const noexcept;

		template<typename T> bool read(T* dst, size_t number) const noexcept { return read(0, dst, number); }

		template<typename T> bool read(size_t offset_byte, T* dst, size_t number) const noexcept
		{
			return read(offset_byte, number * sizeof(T), dst);
		}

		bool open_file(const Path& path) noexcept;

		void release() noexcept;

		// not safe function

		byte_t* data(uint64_t &size) noexcept 
		{
			size = file_size;
			return (byte_t*)pfile_start; 
		}

		const uint64_t get_file_size() const noexcept { return file_size; }

	protected:
		bool get_file_size(const Path& path) noexcept;

		uint64_t file_size;

		void* pfile_start;

#if defined(_WIN32)
		void* hfile_mapping;
		void* c_dump_file_descriptor;
#elif defined(__linux__)
		struct stat st;
		// file description
		int32_t fd = -1;
#else
#error "SUPPORTED ERROR: NOT SUPPORT THIS SYSTEM!"
#endif // SYSTEM CHOOSE
	};
} // namespace wr is end

#endif // _INC_WR_FILE_MMAP_STREAM_HPP_ IS EOF