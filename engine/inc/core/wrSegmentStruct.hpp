#ifndef _WR_SEGMENT_STRUCT_HPP_
#define _WR_SEGMENT_STRUCT_HPP_

// base
#include <wrType.hpp>

namespace wr
{
	constexpr size_t MEMORY_ALIGNED_BLOCK_SIZE = 8;
	constexpr size_t SEGMRNT_HEAD_SIZE = sizeof(size_t) * 2;

	struct segment
	{
		size_t ofs;
		byte_t data[1];
	};

	using segment_view = segment*;

	//constexpr segment* to_segment_view(void* ptr) { return reinterpret_cast<segment*>(ptr); }
} // namespace wr is end

#endif // _WR_SEGMENT_STRUCT_HPP_ IS EOF