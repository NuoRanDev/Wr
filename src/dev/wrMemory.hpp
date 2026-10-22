#ifndef _WR_MEMORY_HPP_
#define _WR_MEMORY_HPP_

// base
#include <wrType.hpp>
#include <wrCompiler.hpp>

namespace wr
{
	c_extern exportfunc void* wr_memcpy(void* dst, const void* src, size_t cpy_size) noexcept;

	c_extern exportfunc int32_t wr_memcmp(const void* buf1, const void* buf2, size_t cmp_size) noexcept;

	c_extern exportfunc void* wr_memset(int8_t val, size_t bytes, void* where) noexcept;
} // namespace wr is end

#endif // _WR_MEMORY_HPP_ IS EOF