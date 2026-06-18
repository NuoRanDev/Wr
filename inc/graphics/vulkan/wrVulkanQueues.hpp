#ifndef _INC_WR_VULKAN_QUEUE_HPP_
#define _INC_WR_VULKAN_QUEUE_HPP_

// core
#include <type/wrOrdinals.hpp>
#include <type/wrResult.hpp>
// std
#include <format>
// vulkan
#include <vulkan/vulkan.h>
// graphics
#include <vulkan/wrVulkanConfig.hpp>

namespace wr
{
	struct QueueInstance
	{
		float queue_priority;
		uint32_t index;
		VkQueue queue;
	};

	struct QueueList
	{
		// Graphics queue is must been created
		QueueInstance graphics_queue_inst;
		// Presentation queue that is showed in window surface is must been created
		QueueInstance presentation_queue_inst;
		// Compute queue
		QueueInstance compute_queue_inst;
		// Vedio av1 dec queue
		QueueInstance av1_video_dec_queue_inst;
	};

	struct QueueModule
	{
		uint32_t need_queues[4];
		uint32_t need_queue_count;

		QueueList queue_list;

		uint32_t queue_family_count;
		VkQueueFamilyProperties* queue_family_propertieses = nullptr;

		VkDeviceQueueCreateInfo queue_create_infos[4];
	};

	ResultInfo get_queue_info(VkPhysicalDevice phy_dev, QueueModule& dst);

	ResultInfo set_graphics_queue(QueueModule& dst, float queue_priority = graphics_queue_priority);

	ResultInfo set_presentation_queue(VkPhysicalDevice phy_dev, VkSurfaceKHR window_bit_map, QueueModule& dst, float queue_priority = presentation_queue_priority);

	ResultInfo set_compute_queue(QueueModule& dst, float queue_priority = compute_queue_priority);

	ResultInfo set_av1_video_dec_queue(QueueModule& dst, float queue_priority = av1_video_dec_queue_priority);

	void free_queue_info(QueueModule& dst);
} // namespace wr is end


#endif // _INC_WR_VULKAN_QUEUE_HPP_ IS EOF