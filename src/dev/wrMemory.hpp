#ifndef _WR_MEMORY_HPP_
#define _WR_MEMORY_HPP_

// base
#include <wrType.hpp>
#include <wrCompiler.hpp>

namespace wr
{
	exportfunc void* wr_memcpy(void* dst, const void* src, size_t cpy_size) noexcept;

	exportfunc int32_t wr_memcmp(const void* buf1, const void* buf2, size_t cmp_size) noexcept;
} // namespace wr is end

#endif // _WR_MEMORY_HPP_ IS EOF