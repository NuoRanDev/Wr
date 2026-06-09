#ifndef _INC_WR_VULKAN_HPP_
#define _INC_WR_VULKAN_HPP_

// vulkan
#include <vulkan/vulkan.h>
// core
#include <type/wrOrdinals.hpp>

namespace wr
{
	void* vk_malloc(void* pd, size_t size, size_t alignment, VkSystemAllocationScope allocation_scope) noexcept;

	void* vk_realloc(void* pd, void* re_ptr, size_t size, size_t alignment, VkSystemAllocationScope allocation_scope) noexcept;

	void vk_free(void* pd, void* free_ptr) noexcept;

	const VkAllocationCallbacks vk_allocator =
	{
		.pUserData = nullptr,
		.pfnAllocation = vk_malloc,
		.pfnReallocation = vk_realloc,
		.pfnFree = vk_free,
		.pfnInternalAllocation = nullptr,
		.pfnInternalFree = nullptr,
	};
	constexpr VkAllocationCallbacks* pvk_allocator = nullptr;

	constexpr char engine_name[] = "wr engine main";

	struct VulkanContext
	{
		VkInstance						vk_main_instance				= VK_NULL_HANDLE;

		uint32_t						gpu_cout						= 0;
		VkPhysicalDevice*				gpu_list						= nullptr;
		VkPhysicalDeviceProperties*		vk_gpu_properties				= VK_NULL_HANDLE;
		VkPhysicalDeviceFeatures*		vk_gpu_features					= VK_NULL_HANDLE;

		uint32_t						cur_used_gpu_index				= -1;
		VkPhysicalDevice				cur_used_gpu					= VK_NULL_HANDLE;

		VkDevice						vk_logic_vkdevice				= VK_NULL_HANDLE;
		VkSurfaceKHR					window_bitmap_surface			= VK_NULL_HANDLE;

		uint32_t						queue_family_count				= 0;
		VkQueueFamilyProperties*		queue_family_propertieses		= nullptr;
		
		VkQueue							graphics_queue					= VK_NULL_HANDLE;
		uint32_t						graphics_queue_index			= 0;

		VkQueue							compute_queue					= VK_NULL_HANDLE;
		uint32_t						compute_queue_index				= 0;

		VkQueue							presentation_queue				= VK_NULL_HANDLE;
		uint32_t						presentation_queue_index		= 0;
#ifdef _DEBUG
		VkLayerProperties*				instance_layers					= nullptr;
#endif // _DEBUG is end
	};

	void init_vulkan_instance(VulkanContext* vk_ctx, const utf8_t* app_name) noexcept;

	void find_gpu(VulkanContext* vk_ctx) noexcept;

	bool get_queue_family_indices(VulkanContext* vk_ctx, bool enable_graphics_queue = true, bool enable_compute_queue = true) noexcept;

	bool create_logic_device(VulkanContext* vk_ctx, float queue_priority = 1.f, VkDeviceCreateFlags flags = VK_DEVICE_QUEUE_CREATE_PROTECTED_BIT) noexcept;

	void release_vulkan_ctx(VulkanContext* vk_ctx) noexcept;
} // namespace wr is end

#endif // _INC_WR_VULKAN_HPP_ IS EOF