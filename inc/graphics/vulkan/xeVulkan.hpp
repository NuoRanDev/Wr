#include <vulkan/vulkan.hpp>

#include "type/wrOrdinals.hpp"

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
	constexpr auto pvk_allocator = nullptr;

	constexpr auto engine_name = "wr engine main";
#ifdef _DEBUG
	const char* const instance_validation_layers_alt1[] =
	{
		"VK_LAYER_LUNARG_standard_validation",
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
	};

	const char* const instance_validation_layers_alt2[] = 
	{
		"VK_LAYER_GOOGLE_threading",       "VK_LAYER_LUNARG_parameter_validation",
		"VK_LAYER_LUNARG_object_tracker",  "VK_LAYER_LUNARG_image",
		"VK_LAYER_LUNARG_core_validation", "VK_LAYER_LUNARG_swapchain",
		"VK_LAYER_GOOGLE_unique_objects"
	};
#endif // _DEBUG is end

	struct VulkanContext
	{
		VkInstance							vk_main_instance;

		uint32_t							gpu_cout;
		VkPhysicalDevice*					gpu_list;
		VkPhysicalDeviceProperties*			vk_gpu_properties;
		VkPhysicalDeviceFeatures*			vk_gpu_features;
		VkPhysicalDevice					cur_used_gpu;

		VkDevice							vk_logic_vkdevice;
		VkSurfaceKHR						window_bitmap_surface;

		VkQueue								graphics_queue;
		uint32_t							graphics_queue_index;
#ifdef _DEBUG
		VkLayerProperties*					instance_layers;
#endif // _DEBUG

	};

	void init_vulkan_instance(VulkanContext& vk_ctx, const utf8_t* app_name,
		const char* const* vulkan_enabled_extensions, uint32_t vulkan_enabled_extension_count) noexcept;

	void find_gpu(VulkanContext& vk_ctx) noexcept;

	void release_vulkan_ctx(VulkanContext& vk_ctx) noexcept;
} // namespace wr is end