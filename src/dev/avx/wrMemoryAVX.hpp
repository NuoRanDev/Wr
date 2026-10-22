#ifndef _WR_MEMORY_AVX_HPP_
#define _WR_MEMORY_AVX_HPP_

/*****************************************************************
* use avx(128) and avx2(256)
* important the cpu must be i*4** later(intel), zen1000 later(AMD)
* or intel CPU is producted later than 2011 ,AMD later than 2017
******************************************************************/

// base
#include <wrType.hpp>
#include <wrCompiler.hpp>

#if defined(USE_AVX)

namespace wr
{
	namespace avx
	{
		void* wr_memcpy(byte_t* dst, const byte_t* src, size_t cpy_size) noexcept;

		int32_t wr_memcmp(const byte_t* buf1, const byte_t* buf2, size_t cmp_size) noexcept;

		void* wr_memset(int8_t val, size_t bytes, void* where) noexcept;
	} // namespace avx is end
} // namespace wr is end
#endif // AVX CPU platform

#endif // _WR_MEMORY_AVX_HPP_ IS EOF