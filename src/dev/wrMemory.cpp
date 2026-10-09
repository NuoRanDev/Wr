// statement
#include <dev/wrMemory.hpp>
// base
#include <wrType.hpp>
#include <wrCompiler.hpp>

#if defined(USE_AVX)
#include <dev/avx/wrMemoryAVX.hpp>
#elif defined(USE_NEON)
#else // DEFAULT
#include <cstring>
#endif // CPU platform

namespace wr
{
	void* wr_memcpy(void* dst, const void* src, size_t cpy_size) noexcept
	{
#if defined(USE_AVX)
		return avx::wr_memcpy(
			reinterpret_cast<byte_t*>(dst),
			reinterpret_cast<const byte_t*>(src),
			cpy_size);
#elif defined(USE_NEON)
#else // DEFAULT
		return std::memcpy(dst, src, cpy_size);
#endif // CPU platform
	}

	int32_t wr_memcmp(const void* buf1, const void* buf2, size_t cmp_size) noexcept
	{
#if defined(USE_AVX)
		return avx::wr_memcmp(
			reinterpret_cast<const byte_t*>(buf1),
			reinterpret_cast<const byte_t*>(buf2),
			cmp_size);
#elif defined(USE_NEON)
#else // DEFAULT
		return std::memcmp(buf1, buf2, cmp_size);
#endif // CPU platform
	}
} // namespace wr is end