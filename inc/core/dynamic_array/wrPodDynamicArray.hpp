#ifndef _WR_POD_DYNAMIC_ARRAY_HPP_
#define _WR_POD_DYNAMIC_ARRAY_HPP_

// base
#include <wrType.hpp>
#include <wrCompiler.hpp>
// core
#include <core/wrSegmentStruct.hpp>

namespace wr
{
	struct pod_dynamic_array
	{
		size_t item_num;
		size_t item_size;
		size_t capacity_num;
		byte_t* pitems;
	};

	c_extern exportfunc pod_dynamic_array pod_dynamic_void_array_create(size_t init_item_size, size_t init_capacity_num = MEMORY_ALIGNED_BLOCK_SIZE) noexcept;

	c_extern exportfunc pod_dynamic_array pod_dynamic_empty_array_create(size_t init_item_size, size_t init_item_num) noexcept;

	c_extern exportfunc pod_dynamic_array pod_dynamic_array_form(pod_dynamic_array format_data, const void* ptr, size_t num, size_t item_size) noexcept;

	c_extern exportfunc pod_dynamic_array pod_dynamic_array_pop_back(pod_dynamic_array format_data) noexcept;

	c_extern exportfunc pod_dynamic_array pod_dynamic_array_resize(pod_dynamic_array format_data, size_t num) noexcept;

	c_extern exportfunc pod_dynamic_array pod_dynamic_array_capacity(pod_dynamic_array format_data, size_t num) noexcept;

	c_extern exportfunc pod_dynamic_array pod_dynamic_array_push_back(pod_dynamic_array format_data, const void* ptr, size_t num) noexcept;

	c_extern exportfunc pod_dynamic_array pod_dynamic_array_release(pod_dynamic_array format_data) noexcept;

} // namespace wr is end

#endif // _WR_POD_DYNAMIC_ARRAY_HPP_ IS END