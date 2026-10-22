// statement
#include <dev/avx/wrMemoryAVX.hpp>
#if defined(USE_AVX)
// avx
#include <mmintrin.h>
#include <immintrin.h>

namespace wr
{
	namespace avx
	{
		void* wr_memset(int8_t val, size_t bytes, void* where) noexcept
		{
			__m256i vec32_byte;
			__m128i vec16_byte;
			int64_t vec8_byte;
			byte_t* where_load = reinterpret_cast<byte_t*>(where);

			vec16_byte = _mm_set1_epi8(val);
		_OFS_JMP:
			if (bytes < 8) goto _1_BYTE;
			if (bytes < 16) goto _8_BYTE;
			if (bytes < 32) goto _16_BYTE;
			vec32_byte = _mm256_set1_epi8(val);
			do
			{
				_mm256_storeu_si256(reinterpret_cast<__m256i*>(where_load), vec32_byte);
				bytes = bytes - 32;
				where_load = where_load + 32;
			} while (bytes > 32);
			if (bytes < 16) goto _OFS_JMP;
		_16_BYTE:
			_mm_storeu_si128(reinterpret_cast<__m128i*>(where_load), vec16_byte);
			bytes = bytes - 16;
			where_load = where_load + 16;
			if (bytes < 8) goto _1_BYTE;
		_8_BYTE:
			vec8_byte = _mm_cvtss_i64(_mm_castsi128_ps(vec16_byte));
			*(reinterpret_cast<int64_t*>(where_load)) = vec8_byte;
			bytes = bytes - 8;
			where_load = where_load + 8;
		_1_BYTE:
			for (uint64_t i = 0; i < bytes; i++)
			{
				*where_load = val;
				where_load++;
			}
			return where;
		}
	} // namespace avx is end
} // namespace wr is end
#endif // AVX CPU platform