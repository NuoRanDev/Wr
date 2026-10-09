// statement
#include <dev/avx/wrMemoryAVX.hpp>
#if defined(USE_AVX)
// avx
#include <emmintrin.h>
#include <mmintrin.h>
#include <immintrin.h>

namespace wr
{
	namespace avx
	{
		void* wr_memcpy(byte_t* dst, const byte_t* src, size_t cpy_size) noexcept
		{
			if (src == nullptr || cpy_size == 0)
				return nullptr;

			const byte_t* copy_from = reinterpret_cast<const byte_t*>(src);
			byte_t* copy_to = reinterpret_cast<byte_t*>(dst);
			size_t need_copy_size = cpy_size;


			uint64_t temp_8byte;
			__m128i temp_16_byte;
			__m256i temp_32_byte;

		_OFS_JMP:
			if (need_copy_size < 8) goto _1_BYTE;
			if (need_copy_size < 16) goto _8_BYTE;
			if (need_copy_size < 32) goto _16_BYTE;

			do
			{
				temp_32_byte = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(copy_from));
				_mm256_storeu_si256(reinterpret_cast<__m256i*>(copy_to), temp_32_byte);
				need_copy_size -= 32;
				copy_from += 32;
				copy_to += 32;
			} while (need_copy_size > 16);
			if (need_copy_size < 16) goto _OFS_JMP;

		_16_BYTE:
			temp_16_byte = _mm_loadu_si128(reinterpret_cast<const __m128i*>(copy_from));
			_mm_storeu_si128(reinterpret_cast<__m128i*>(copy_to), temp_16_byte);
			need_copy_size -= 16;
			copy_from += 16;
			copy_to += 16;
			if (need_copy_size < 8) goto _1_BYTE;

		_8_BYTE:
			temp_8byte = *(reinterpret_cast<const uint64_t*>(copy_from));
			*(reinterpret_cast<uint64_t*>(copy_to)) = temp_8byte;
			need_copy_size -= 8;
			copy_from += 8;
			copy_to += 8;

		_1_BYTE:
			for (uint64_t i = 0; i < need_copy_size; i++)
			{
				*copy_to = *copy_from;
				++copy_from;
				++copy_to;
			}
			return dst;
		}
	} // namespace avx is end
} // namespace wr is end
#endif // AVX CPU platform