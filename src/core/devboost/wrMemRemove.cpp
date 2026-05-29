// statement
#include <devboost/wrMemCmp.hpp>
// avx
#include <emmintrin.h>

namespace wr
{
	void* memerase(byte_t* src, uint64_t src_size, uint64_t start, uint64_t ofs) noexcept
	{
		if (src == nullptr || src_size == 0)
			return nullptr;

		uint64_t move_start = start + ofs;
		uint64_t copy_size = src_size - start - ofs;
		byte_t* copy_from = src + move_start;
		byte_t* copy_to = src + start;


		uint64_t temp_8byte;
		__m128i temp_16_byte;

		if (copy_size < 8) goto _1_BYTE;
		if (copy_size < 16) goto _8_BYTE;

	// _16_BYTE:
		do
		{
			temp_16_byte = _mm_loadu_si128(reinterpret_cast<__m128i*>(copy_from));
			_mm_storeu_si128(reinterpret_cast<__m128i*>(copy_to), temp_16_byte);
			copy_size -= 16;
			copy_from += 16;
			copy_to += 16;
		} while (copy_size > 16);
		if (copy_size < 8) goto _1_BYTE;
	_8_BYTE:
		temp_8byte = *(reinterpret_cast<uint64_t*>(copy_from));
		*(reinterpret_cast<uint64_t*>(copy_to)) = temp_8byte;
		copy_size -= 8;
		copy_from += 8;
		copy_to += 8;
	_1_BYTE:
		for (uint64_t i = 0; i < copy_size; i++)
		{
			*copy_to = *copy_from;
			++copy_from;
			++copy_to;
		}
		return src;
	}
} // namespace wr is end