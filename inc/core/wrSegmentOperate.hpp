#ifndef _INC_SEGMENT_OPERATE_
#define _INC_SEGMENT_OPERATE_

// base
#include <wrType.hpp>
#include <wrCompiler.hpp>
// core
#include <core/wrSegmentStruct.hpp>
#include <core/dynamic_array/wrPodDynamicArray.hpp>

namespace wr
{
	bool segment_struct_equals(const segment_view seg1, const segment_view seg2) noexcept;

	bool segment_byte_equals(const byte_t* pseg1, const byte_t* pseg2, size_t cmp_size) noexcept;

	pod_dynamic_array segment_struct_find_all(const segment_view main_seg, const segment_view child_seg) noexcept;

	pod_dynamic_array segment_byte_find_all
	(const byte_t* main_seg, size_t main_seg_size, const byte_t* child_seg, size_t child_seg_size) noexcept;
} // namespace wr is end

#endif // !_INC_SEGMENT_OPERATE_