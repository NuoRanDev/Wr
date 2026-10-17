// statement
#include <core/wrSegmentOperate.hpp>
// dev
#include <dev/wrMemory.hpp>

namespace wr
{
	bool segment_struct_equals(const segment_view seg1, const segment_view seg2) noexcept
	{
		if (seg1->ofs != seg2->ofs) [[unlikely]]
			return false;
		return segment_byte_equals(seg1->data, seg2->data, seg1->ofs);
	}

	bool segment_byte_equals(const byte_t* pseg1, const byte_t* pseg2, size_t cmp_size) noexcept
	{
		return wr_memcmp(pseg1, pseg2, cmp_size);
	}

	pod_dynamic_array segment_struct_find_all(const segment_view main_seg, const segment_view child_seg) noexcept
	{
		return segment_byte_find_all(main_seg->data, main_seg->ofs, child_seg->data, child_seg->ofs);
	}

	pod_dynamic_array segment_byte_find_all(const byte_t* main_seg, size_t main_seg_size,
		const byte_t* child_seg, size_t child_seg_size) noexcept
	{
		pod_dynamic_array out = pod_dynamic_void_array_create(sizeof(size_t));

		if (main_seg_size < child_seg_size) [[unlikely]]
			return out;

		const byte_t* cur_ptr = main_seg;
		size_t cur_ofs = 0;
		const size_t end_ofs = main_seg_size - child_seg_size + 1;


		while (cur_ofs < end_ofs)
		{
			// auto c1 = reinterpret_cast<const char8_t*>(cur_ptr);
			// auto c2 = reinterpret_cast<const char8_t*>(child_seg);
			if (wr_memcmp(cur_ptr, child_seg, child_seg_size)) [[unlikely]]
			{
				out = pod_dynamic_array_push_back(out, &cur_ofs, 1llu);
			}
			cur_ofs++;
			cur_ptr++;
		}
		return out;
	}


} // namespace wr is end