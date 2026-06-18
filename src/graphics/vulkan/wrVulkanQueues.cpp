// statement
#include <vulkan/wrVulkanQueues.hpp>
// core
#include <log/wrLogOutput.hpp>
#include <memory/wrAlloc.hpp>

namespace wr
{
	ResultInfo get_queue_info(VkPhysicalDevice phy_dev, QueueModule& dst)
	{
		vkGetPhysicalDeviceQueueFamilyProperties(phy_dev, &(dst.queue_family_count), nullptr);

		if (dst.queue_family_count == 0)
		{
			WR_FATAL_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "No queue family!");
			WR_CLR_WRITE_LINE("Update your gpu device", WR_CLR_COLOR::YELLOW);
			std::exit(EXIT_FAILURE);
		}

		dst.queue_family_propertieses = wr_malloc<VkQueueFamilyProperties>(dst.queue_family_count);

		vkGetPhysicalDeviceQueueFamilyProperties(phy_dev, &(dst.queue_family_count), dst.queue_family_propertieses);

		// init default var
		dst.queue_list.graphics_queue_inst.index = UINT32_MAX;
		dst.queue_list.graphics_queue_inst.queue = VK_NULL_HANDLE;

		dst.queue_list.presentation_queue_inst.index = UINT32_MAX;
		dst.queue_list.presentation_queue_inst.queue = VK_NULL_HANDLE;

		dst.queue_list.compute_queue_inst.index = UINT32_MAX;
		dst.queue_list.compute_queue_inst.queue = VK_NULL_HANDLE;

		dst.queue_list.av1_video_dec_queue_inst.index = UINT32_MAX;
		dst.queue_list.av1_video_dec_queue_inst.queue = VK_NULL_HANDLE;

		dst.need_queue_count = 0;

		return ResultInfo::WR_OK;
	}

	ResultInfo set_graphics_queue(QueueModule& dst, float queue_priority)
	{
		for (uint32_t i = 0; i < dst.queue_family_count; i++)
		{
			if (dst.queue_family_propertieses[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				dst.queue_list.graphics_queue_inst.index = i;
				dst.queue_list.graphics_queue_inst.queue_priority = queue_priority;

				dst.need_queues[dst.need_queue_count] = i;
				
				dst.queue_create_infos[dst.need_queue_count].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				dst.queue_create_infos[dst.need_queue_count].queueCount = 1;
				dst.queue_create_infos[dst.need_queue_count].pQueuePriorities = &(dst.queue_list.graphics_queue_inst.queue_priority);
				dst.queue_create_infos[dst.need_queue_count].queueFamilyIndex = i;
				dst.queue_create_infos[dst.need_queue_count].flags = queue_flags;
				dst.queue_create_infos[dst.need_queue_count].pNext = nullptr;

				dst.need_queue_count++;
				
				return ResultInfo::WR_OK;
			}
		}
		WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "Not support graphics queue!");
		return ResultInfo::WR_ERROR;
	}

	ResultInfo set_presentation_queue(VkPhysicalDevice phy_dev, VkSurfaceKHR window_bit_map, QueueModule& dst, float queue_priority)
	{
		VkResult result;
		VkBool32 is_support_presentation;
		for (uint32_t i = 0; i < dst.queue_family_count; i++)
		{
			result = vkGetPhysicalDeviceSurfaceSupportKHR(phy_dev, i, window_bit_map, &is_support_presentation);
			if (result)
			{
				WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan",
					std::format("Get physical device surface support failed\tError Code: {0}",
						static_cast<int64_t>(result)).c_str());
				return ResultInfo::WR_ERROR;
			}
			if(is_support_presentation)
			{
				dst.queue_list.presentation_queue_inst.index = i;
				dst.queue_list.presentation_queue_inst.queue_priority = queue_priority;
				if (i != dst.queue_list.graphics_queue_inst.index)
				{
					dst.need_queues[dst.need_queue_count] = i;
					dst.queue_create_infos[dst.need_queue_count].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
					dst.queue_create_infos[dst.need_queue_count].queueCount = 1;
					dst.queue_create_infos[dst.need_queue_count].pQueuePriorities = &(dst.queue_list.graphics_queue_inst.queue_priority);
					dst.queue_create_infos[dst.need_queue_count].queueFamilyIndex = i;
					dst.queue_create_infos[dst.need_queue_count].flags = queue_flags;
					dst.queue_create_infos[dst.need_queue_count].pNext = nullptr;
					dst.need_queue_count++;
				}
				return ResultInfo::WR_OK;
			}
		}
		WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "Not support presentation queue!");
		return ResultInfo::WR_ERROR;
	}

	ResultInfo set_compute_queue(QueueModule& dst, float queue_priority)
	{
		if (dst.queue_family_count < 2)
		{
			WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "No enogh queue family!");
			return ResultInfo::WR_WARNING;
		}

		for (uint32_t i = 0; i < dst.queue_family_count; i++)
		{
			if (dst.queue_family_propertieses[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				// we can not wait queue when swap-chain recreate
				if (i != dst.queue_list.graphics_queue_inst.index && i != dst.queue_list.presentation_queue_inst.index)
				{
					dst.queue_list.compute_queue_inst.queue_priority = queue_priority;
					dst.queue_list.compute_queue_inst.index = i;
					dst.need_queues[dst.need_queue_count] = i;
					dst.queue_create_infos[dst.need_queue_count].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
					dst.queue_create_infos[dst.need_queue_count].queueCount = 1;
					dst.queue_create_infos[dst.need_queue_count].pQueuePriorities = &(dst.queue_list.graphics_queue_inst.queue_priority);
					dst.queue_create_infos[dst.need_queue_count].queueFamilyIndex = i;
					dst.queue_create_infos[dst.need_queue_count].flags = queue_flags;
					dst.queue_create_infos[dst.need_queue_count].pNext = nullptr;

					dst.need_queue_count++;
					return ResultInfo::WR_OK;
				}
			}
		}
		WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "Not support compute queue!");
		return ResultInfo::WR_WARNING;
	}

	// TODO: VULKAN VEDIO SUPPORT
	ResultInfo set_av1_video_dec_queue(QueueModule& dst, float queue_priority)
	{
		// VK_QUEUE_VIDEO_DECODE_BIT_KHR;
		return ResultInfo::WR_WARNING;
	}

	void free_queue_info(QueueModule& dst)
	{
		wr_free(dst.queue_family_propertieses);
		dst.queue_family_propertieses = nullptr;
	}
} // namespace wr is end