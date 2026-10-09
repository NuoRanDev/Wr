#ifndef _INC_WR_SEGMENT_HPP_
#define _INC_WR_SEGMENT_HPP_

// base
#include <wrType.hpp>

namespace wr
{
	constexpr size_t MEMORY_ALIGNED_BLOCK_SIZE = 8;
	constexpr size_t SEGMRNT_HEAD_SIZE = sizeof(size_t) * 2;

	struct segment
	{
		struct
		{
			size_t num;
			size_t item_size;
		} head;
		byte_t size[0];
	};
} // namespace wr is end

#endif // _INC_WR_SEGMENT_HPP_ IS EOF