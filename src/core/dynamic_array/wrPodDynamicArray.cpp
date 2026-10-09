// statement
#include <core/dynamic_array/wrPodDynamicArray.hpp>
// os
#include <os/wrAlloc.hpp>
// dev
#include <dev/wrMemory.hpp>

namespace wr
{
	pod_dynamic_array pod_create(size_t item_size, size_t init_size) noexcept
	{
		pod_dynamic_array format_data =
		{
			.item_num = 0,
			.item_size = item_size,
			.capacity_num = init_size,
			.pitems = wr_malloc<byte_t>(init_size * item_size)
		};
		return format_data;
	}

	pod_dynamic_array pod_form(pod_dynamic_array format_data, const void* ptr, size_t num, size_t item_size) noexcept
	{
		pod_release(format_data);
		format_data =
		{
			.item_num = num,
			.item_size = item_size,
			.capacity_num = num,
			.pitems = wr_malloc<byte_t>(num & item_size)
		};
		wr_memcpy(format_data.pitems, ptr, num * format_data.item_size);
		return format_data;
	}

	pod_dynamic_array pod_pop_back(pod_dynamic_array format_data) noexcept
	{
		format_data.item_num--;
		return format_data;
	}

	pod_dynamic_array pod_resize(pod_dynamic_array format_data, size_t num) noexcept
	{
		if (num == format_data.item_num) [[unlikely]]
			return format_data;
		else
		{
			if (num < 0) [[unlikely]]
				return format_data;
			else [[likely]]
			{
				if (num == 0) [[unlikely]]
				{
					pod_release(format_data);
					return format_data;
				}
				else [[likely]]
				{
					format_data.item_num = num;
					if (num > format_data.capacity_num) [[likely]]
					{
						pod_capacity(format_data, num);
						return format_data;
					}
					else [[unlikely]]
					{
						return format_data;
					}
				}
			}
		}
	}

	pod_dynamic_array pod_capacity(pod_dynamic_array format_data, size_t num) noexcept
	{
		if (num < 0) [[unlikely]]
		{
			return format_data;
		}
		else [[likely]]
		{
			if (num == 0) [[unlikely]]
			{
				pod_release(format_data);
				return format_data;
			}
			else [[likely]]
			{
				if (format_data.capacity_num != 0) [[likely]]
					format_data.pitems = wr_realloc<byte_t>(format_data.pitems, format_data.item_size * num);
				else [[unlikely]]
				{
					format_data.pitems = wr_malloc<byte_t>(format_data.item_size * num);
				}
			}
			if (num < format_data.item_num)
			{
				format_data.item_num = num;
			}
			format_data.capacity_num = num;
			return format_data;
		}
	}

	pod_dynamic_array pod_push_back(pod_dynamic_array format_data, const void* ptr, size_t num) noexcept
	{
		if (num == 0 || ptr == nullptr) [[unlikely]]
			return format_data;
		else [[likely]]
		{
			size_t new_item_num = num + format_data.item_num;
			if (new_item_num > format_data.capacity_num) [[likely]]
			{
				pod_capacity(format_data, (new_item_num / MEMORY_ALIGNED_BLOCK_SIZE + 1) * MEMORY_ALIGNED_BLOCK_SIZE);
			}
			auto cpy_to = format_data.pitems
				+ format_data.item_num * format_data.item_size;
			wr_memcpy(cpy_to, ptr, num * format_data.item_size);
			format_data.item_num = new_item_num;
			return format_data;
		}
	}

	pod_dynamic_array pod_release(pod_dynamic_array format_data) noexcept
	{
		format_data =
		{
			.item_num = 0,
			.capacity_num = 0,
			.pitems = reinterpret_cast<byte_t*>(wr_free(format_data.pitems))
		};
		return format_data;
	}
} // namespace wr register end