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
		int INT32_TRUE = 0b1111'1111'1111'1111'1111'1111'1111'1111;

		int32_t wr_memcmp(const byte_t* buf1, const byte_t* buf2, size_t cmp_size) noexcept
		{
			uint64_t i = 0;
			__m256i cmp1_32_byte, cmp2_32_byte, cmp_result;
			int mask = 0;

			if (cmp_size < 32)
				goto END_STR_CMP;
			do {
				cmp1_32_byte = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(buf1));
				cmp2_32_byte = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(buf2));

				cmp_result = _mm256_cmpeq_epi8(cmp1_32_byte, cmp2_32_byte);
				// if all 32 byte are equal, then cmp_result will be 0xFF
				// if not equal, then cmp_result will be 0x00
				auto cmp_result_mask = _mm256_movemask_epi8(cmp_result);
				if (cmp_result_mask != INT32_TRUE)
				{
					// if not all 32 byte are equal, then return false
					return false;
				}
				buf1 = buf1 + 32;
				buf2 = buf2 + 32;
				i += 32;
			} while ((i + 32) < cmp_size);
		END_STR_CMP:
			if (i == cmp_size)
			{
				return true; // all bytes are equal
			}
			// if cur_cmp_size < 32, then use short memory compare
			mask = static_cast<int32_t>(cmp_size - i);
			mask = ~(INT32_TRUE << cmp_size);
			cmp1_32_byte = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(buf1));
			cmp2_32_byte = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(buf2));

			cmp_result = _mm256_cmpeq_epi8(cmp1_32_byte, cmp2_32_byte);

			auto cmp_result_mask = _mm256_movemask_epi8(cmp_result);

			cmp_result_mask = cmp_result_mask & mask;

			if (cmp_result_mask != mask)
			{
				return false;
			}
			return true; // all bytes are equal
		}
	} // namespace avx is end
} // namespace wr is end
#endif // AVX CPU platform