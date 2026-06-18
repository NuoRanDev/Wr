#ifndef _INC_WR_VULKAN_HPP_
#define _INC_WR_VULKAN_HPP_

// vulkan
#include <vulkan/vulkan.h>
// core
#include <type/wrOrdinals.hpp>
#include <math/wrMathVector.hpp>
// graphics
#include <vulkan/wrVulkanQueues.hpp>

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
		uint32_t						vk_gpu_extension_count			= 0;
		VkExtensionProperties*			vk_gpu_extensions				= VK_NULL_HANDLE;

		VkDevice						vk_logic_vkdevice				= VK_NULL_HANDLE;
		VkSurfaceKHR					window_bitmap_surface			= VK_NULL_HANDLE;

		QueueModule						queue_mod;

		VkSwapchainCreateInfoKHR		swapchain_create_info_data;
		VkSwapchainKHR					swapchain;

		uint32_t						available_surface_format_count	= 0;
		VkSurfaceFormatKHR*				available_surface_formats		= VK_NULL_HANDLE;

		uint32_t						surface_present_mode_count		= 0;
		VkPresentModeKHR*				surface_present_modes			= VK_NULL_HANDLE;

		uint32_t						swapchain_image_count			= 0;
		VkImage*						swapchain_images				= VK_NULL_HANDLE;
		VkImageView*					swapchain_image_views			= VK_NULL_HANDLE;

		bool							alpha_window					= true;
		bool							tty_screen						= false;
		bool							limit_frame_rate				= true;

#ifdef _DEBUG
		VkLayerProperties*				instance_layers					= nullptr;
#endif // _DEBUG is end
	};

	void init_vk_ctx(VulkanContext* vk_ctx);

	void init_vulkan_instance(VulkanContext* vk_ctx, const utf8_t* app_name) noexcept;

	void find_gpu(VulkanContext* vk_ctx) noexcept;

	ResultInfo create_logic_device(VulkanContext* vk_ctx, VkDeviceCreateFlags flags = 0, bool use_compute_queue = true) noexcept;

	/*=======================================================
	* \param window_size if draw on screen ,that is screen size
	* \param cache_surface_count this cache bitmap number
	* \param alpha_window no window border set alpha
	* \param tty_screen is direct window
	* \param limit_frame_rate lock frame rate
	========================================================*/
	ResultInfo create_swapchain(VulkanContext* ck_ctx,
		vec2u window_size,
		uint32_t cache_surface_count,
		bool alpha_window,
		bool tty_screen,
		bool limit_frame_rate = true);

	VkResult recreate_swapchain(VulkanContext* vk_ctx) noexcept;

	ResultInfo create_image_view(VulkanContext* vk_ctx);

	void release_vulkan_ctx(VulkanContext* vk_ctx) noexcept;
} // namespace wr is end

#endif // _INC_WR_VULKAN_HPP_ IS EOF