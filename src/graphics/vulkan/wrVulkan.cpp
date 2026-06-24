// statement
#include <vulkan/wrVulkan.hpp>
// core
#include <log/wrLogOutput.hpp>
#include <string/wrString.hpp>
#include <memory/wrAlloc.hpp>
// std
#include <format>
#include <algorithm>
// graphics
#include <vulkan/wrVulkanConfig.hpp>
// std
#include <functional>
// core
#include <type/wrDataStruction.hpp>

namespace wr
{
	static dynamic_array <std::pair<any_type_ptr_t, std::function<ResultInfo(any_type_ptr_t)>>> recreate_swapchain_task;

	void* vk_malloc(void* pd, size_t size, size_t alignment, VkSystemAllocationScope allocation_scope) noexcept
	{
		void* ptr = wr_malloc<byte_t>(size);
		return ptr;
	}

	void* vk_realloc(void* pd, void* re_ptr, size_t size, size_t alignment, VkSystemAllocationScope allocation_scope) noexcept
	{
		void* ptr = wr_realloc<byte_t>(re_ptr, size);
		return ptr;
	}

	void vk_free(void* pd, void* free_ptr) noexcept
	{
		free(free_ptr);
	}

	void init_vk_ctx(VulkanContext* vk_ctx)
	{
		vk_ctx->vk_main_instance = VK_NULL_HANDLE;

		vk_ctx->gpu_cout = 0;
		vk_ctx->gpu_list = nullptr;
		vk_ctx->vk_gpu_properties = VK_NULL_HANDLE;
		vk_ctx->vk_gpu_features = VK_NULL_HANDLE;

		vk_ctx->cur_used_gpu_index = -1;
		vk_ctx->cur_used_gpu = VK_NULL_HANDLE;
		vk_ctx->vk_gpu_extension_count = 0;
		vk_ctx->vk_gpu_extensions = VK_NULL_HANDLE;

		vk_ctx->vk_logic_vkdevice = VK_NULL_HANDLE;
		vk_ctx->window_bitmap_surface = VK_NULL_HANDLE;

		vk_ctx->available_surface_format_count = 0;
		vk_ctx->available_surface_formats = VK_NULL_HANDLE;

		vk_ctx->surface_present_mode_count = 0;
		vk_ctx->surface_present_modes = VK_NULL_HANDLE;

		vk_ctx->swapchain_create_info_data.oldSwapchain = nullptr;
		vk_ctx->swapchain = VK_NULL_HANDLE;

		vk_ctx->swapchain_image_count = 0;
		vk_ctx->swapchain_images = VK_NULL_HANDLE;
		vk_ctx->swapchain_image_views = VK_NULL_HANDLE;

		vk_ctx->alpha_window = true;
		vk_ctx->tty_screen = false;
		vk_ctx->limit_frame_rate = true;

		// The struct will be used when create and recreate swapchain
		vk_ctx->swapchain_create_info_data.imageFormat = VK_FORMAT_UNDEFINED;

#ifdef _DEBUG
		vk_ctx->instance_layers = nullptr;
#endif // _DEBUG is end

		recreate_swapchain_task = 
			dynamic_array<std::pair<any_type_ptr_t, std::function<ResultInfo(any_type_ptr_t)>>>();
	}

	static bool check_vulkan_validation_layers(uint32_t check_count, const char * const* check_names,
		uint32_t layer_count, VkLayerProperties* layers) noexcept
	{
		uint32_t i, j;
		uint32_t is_check_count = 0;
		for (i = 0; i < check_count; i++)
		{
			for (j = 0; j < layer_count; j++)
			{
				if (strcmp(check_names[i], layers[j].layerName) == 0)
				{
					is_check_count++;
					break;
				}
			}
		}
		if (is_check_count != check_count)
			return false;
		return true;
	}

	void init_vulkan_instance(VulkanContext* vk_ctx, const utf8_t* app_name) noexcept
	{
		VkInstanceCreateInfo vk_inst_info;
		VkResult state;
		uint32_t instance_layer_count = 0;
		const char* const* instance_validation_layers = nullptr;

		const VkApplicationInfo app =
		{
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pNext = nullptr,
			.pApplicationName = reinterpret_cast<const char*>(app_name),
			.applicationVersion = 0,
			.pEngineName = engine_name,
			.engineVersion = 0,
			.apiVersion = VK_API_VERSION_1_0,
		};

#ifdef _DEBUG
		char is_running = '\0';
		bool validation_found = true;

		state = vkEnumerateInstanceLayerProperties(&instance_layer_count, nullptr);
		if (state != VK_SUCCESS) WR_FATAL_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "Find validation layer fatal!");
		if (instance_layer_count > 0)
		{
			vk_ctx->instance_layers = wr_malloc<VkLayerProperties>(instance_layer_count);
			state = vkEnumerateInstanceLayerProperties(&instance_layer_count, vk_ctx->instance_layers);
			if (state != VK_SUCCESS) WR_FATAL_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "Find validation layer fatal!");
		}
		else
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "System doesn't have validation layer library");
			goto IS_RUNNING_WITHOUT_VULKAN_VALIDATION_LAYERS;
		}


		validation_found = check_vulkan_validation_layers(instance_validation_layers_alt1_count, instance_validation_layers_alt1, instance_layer_count, vk_ctx->instance_layers);
		if (validation_found)
		{
			instance_validation_layers = instance_validation_layers_alt1;
			instance_layer_count = instance_validation_layers_alt1_count;
			goto END_OF_FIND_VULKAN_VALIDATION_LAYERS;
		}
		else
		{
			bool validation_found = check_vulkan_validation_layers(instance_validation_layers_alt2_count, instance_validation_layers_alt2, instance_layer_count, vk_ctx->instance_layers);
			if (validation_found)
			{
				instance_validation_layers = instance_validation_layers_alt2;
				instance_layer_count = instance_validation_layers_alt2_count;
				goto END_OF_FIND_VULKAN_VALIDATION_LAYERS;
			}
			else
			{
				WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "the validation layer library doesn't support this");
				goto IS_RUNNING_WITHOUT_VULKAN_VALIDATION_LAYERS;
			}
		}

	IS_RUNNING_WITHOUT_VULKAN_VALIDATION_LAYERS:
		WR_CLR_WRITE_LINE("Do you want run this program that doesn't run validation layer? [Y : YES N : NO]", WR_CLR_COLOR::YELLOW);
		std::cin >> is_running;
		if (is_running == 'Y')
		{
			WR_CLR_WRITE_LINE("Not run validation layer");
			instance_validation_layers = nullptr;
			instance_layer_count = 0;
			goto END_OF_FIND_VULKAN_VALIDATION_LAYERS;
		}
		else std::exit(EXIT_FAILURE);
	END_OF_FIND_VULKAN_VALIDATION_LAYERS:
#else
		instance_layer_count = 0;
		instance_validation_layers = nullptr;
#endif // _DEBUG
		vk_inst_info =
		{
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext = nullptr,
			.pApplicationInfo = &app,
			.enabledLayerCount = instance_layer_count,
			.ppEnabledLayerNames = instance_validation_layers,
			.enabledExtensionCount = vulkan_enabled_extension_count,
			.ppEnabledExtensionNames = vulkan_enabled_extensions,
		};

		state = vkCreateInstance(&vk_inst_info, pvk_allocator, &(vk_ctx->vk_main_instance));
		if (state == VK_SUCCESS) return;
		else if (state == VK_ERROR_INCOMPATIBLE_DRIVER)
		{
			WR_FATAL_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "Cannot find a compatible Vulkan installable client driver!");
		}
		else if (state == VK_ERROR_EXTENSION_NOT_PRESENT) {
			WR_FATAL_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "Cannot find a specified extension library!");
		}
#ifdef _DEBUG
		else if (state == VK_ERROR_LAYER_NOT_PRESENT)
		{
			// vkDestroyInstance(vk_ctx->vk_main_instance, pvk_allocator);
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "System doesn't have validation layer library");
			goto IS_RUNNING_WITHOUT_VULKAN_VALIDATION_LAYERS;
		}
#endif // _DEBUG
		else if (state != VK_SUCCESS) {
			WR_FATAL_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "vkCreateInstance failed.\n\nDo you have a compatible Vulkan!");
		}
	}

	// Copy gpu information to "VulkanContext"
	void find_gpu(VulkanContext* vk_ctx) noexcept
	{
		VkResult state;
		state = vkEnumeratePhysicalDevices(vk_ctx->vk_main_instance, &(vk_ctx->gpu_cout), nullptr);
		if (state != VK_SUCCESS || vk_ctx->gpu_cout == 0)
			WR_FATAL_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "No GPU!");
		vk_ctx->gpu_list = wr::wr_malloc<VkPhysicalDevice>(vk_ctx->gpu_cout);
		state = vkEnumeratePhysicalDevices(vk_ctx->vk_main_instance, &(vk_ctx->gpu_cout), vk_ctx->gpu_list);
		if (state != VK_SUCCESS || vk_ctx->gpu_cout == 0)
			WR_FATAL_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "Enumerate gpu failed!");

		vk_ctx->vk_gpu_properties = wr_malloc<VkPhysicalDeviceProperties>(vk_ctx->gpu_cout);
		vk_ctx->vk_gpu_features = wr_malloc<VkPhysicalDeviceFeatures>(vk_ctx->gpu_cout);

		for (uint32_t i = 0; i < vk_ctx->gpu_cout; i++)
		{
			vkGetPhysicalDeviceProperties(vk_ctx->gpu_list[i], vk_ctx->vk_gpu_properties + i);
			WR_CLR_WRITE_LINE(std::format("Find Gpu[{0}] : {1} ", i, vk_ctx->vk_gpu_properties[i].deviceName).c_str());
			vkGetPhysicalDeviceFeatures(vk_ctx->gpu_list[i], vk_ctx->vk_gpu_features + i);
		}
		return;
	}

	ResultInfo create_logic_device(VulkanContext* vk_ctx, VkDeviceCreateFlags flags, bool use_compute_queue) noexcept
	{
		VkResult result;
		uint32_t is_device_enabled_extension_count = 0;

		result = vkEnumerateDeviceExtensionProperties(vk_ctx->cur_used_gpu, nullptr, &vk_ctx->vk_gpu_extension_count, nullptr);
		if (result != VK_SUCCESS || vk_ctx->vk_gpu_extension_count == 0)
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan",
				"This gpu not support graphics output");
			vk_ctx->vk_gpu_extension_count = 0;
			return ResultInfo::WR_ERROR;
		}
		vk_ctx->vk_gpu_extensions = wr_malloc<VkExtensionProperties>(vk_ctx->vk_gpu_extension_count);
		vkEnumerateDeviceExtensionProperties(vk_ctx->cur_used_gpu, nullptr, &vk_ctx->vk_gpu_extension_count, vk_ctx->vk_gpu_extensions);

		// some GPU not support swapchain, such as V100 ,jsoth.
		for (uint32_t j = 0; j < vk_ctx->vk_gpu_extension_count; j++)
		{
			for (uint32_t i = 0; i < device_enabled_extension_count; i++)
			{
				if (strcmp(device_enabled_extensions[i],vk_ctx->vk_gpu_extensions[j].extensionName) == 0)
					is_device_enabled_extension_count++;
			}
		}

		if (is_device_enabled_extension_count != device_enabled_extension_count)
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan",
				"This gpu not support graphics output");
			vk_ctx->vk_gpu_extension_count = 0;
			wr_free(vk_ctx->vk_gpu_extensions);
			vk_ctx->vk_gpu_extensions = nullptr;
			return ResultInfo::WR_ERROR;
		}

		if (get_queue_info(vk_ctx->cur_used_gpu, vk_ctx->queue_mod))
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "Get queue failed!");
			return ResultInfo::WR_ERROR;
		}
		if (set_graphics_queue(vk_ctx->queue_mod))
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "Get graphics queue failed!");
			return ResultInfo::WR_ERROR;
		}
		if (set_presentation_queue(vk_ctx->cur_used_gpu, vk_ctx->window_bitmap_surface, vk_ctx->queue_mod))
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "Get presentation queue failed!");
			return ResultInfo::WR_ERROR;
		}
		if (use_compute_queue)
		{
			if(set_compute_queue(vk_ctx->queue_mod))
			{
				WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "Get presentation queue failed!");
				return ResultInfo::WR_WARNING;
			}
		}

		VkDeviceCreateInfo deviceCreateInfo =
		{
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.pNext = nullptr,
			.flags = flags,
			.queueCreateInfoCount = vk_ctx->queue_mod.need_queue_count,
			.pQueueCreateInfos = reinterpret_cast<VkDeviceQueueCreateInfo*>(&(vk_ctx->queue_mod.queue_create_infos)),
			.enabledLayerCount = 0,
			.ppEnabledLayerNames = nullptr,
			.enabledExtensionCount = device_enabled_extension_count,
			.ppEnabledExtensionNames = device_enabled_extensions,
			.pEnabledFeatures = &(vk_ctx->vk_gpu_features[vk_ctx->cur_used_gpu_index])
			//.pEnabledFeatures = nullptr
		};

		result = vkCreateDevice(vk_ctx->cur_used_gpu, &deviceCreateInfo, pvk_allocator, &(vk_ctx->vk_logic_vkdevice));

		if (result)
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", 
				std::format("Failed to create a vulkan logical device!\t Error code: {0}", static_cast<uint64_t>(result)).c_str());
			return ResultInfo::WR_ERROR;
		}

		if (vk_ctx->queue_mod.queue_list.graphics_queue_inst.index != UINT32_MAX)
			vkGetDeviceQueue(vk_ctx->vk_logic_vkdevice, 
				vk_ctx->queue_mod.queue_list.graphics_queue_inst.index,
				0,
				&(vk_ctx->queue_mod.queue_list.graphics_queue_inst.queue));
		if (vk_ctx->queue_mod.queue_list.presentation_queue_inst.index != UINT32_MAX)
			vkGetDeviceQueue(vk_ctx->vk_logic_vkdevice,
				vk_ctx->queue_mod.queue_list.presentation_queue_inst.index,
				0,
				&(vk_ctx->queue_mod.queue_list.presentation_queue_inst.queue));
		if (vk_ctx->queue_mod.queue_list.compute_queue_inst.index != UINT32_MAX)
			vkGetDeviceQueue(vk_ctx->vk_logic_vkdevice,
				vk_ctx->queue_mod.queue_list.compute_queue_inst.index,
				0,
				&(vk_ctx->queue_mod.queue_list.compute_queue_inst.queue));
		return ResultInfo::WR_OK;
	}

	static bool set_surface_format(VulkanContext* vk_ctx, VkSurfaceFormatKHR surface_format)
	{
		if (!surface_format.format)
		{
			// If not define format, switch color space
			for (size_t i = 0; i < vk_ctx->available_surface_format_count; i++)
			{
				if (vk_ctx->available_surface_formats[i].colorSpace == surface_format.colorSpace)
				{
					vk_ctx->swapchain_create_info_data.imageFormat = vk_ctx->available_surface_formats[i].format;
					vk_ctx->swapchain_create_info_data.imageColorSpace = vk_ctx->available_surface_formats[i].colorSpace;
					return true;
				}
			}
		}
		else
		{
			// all switch
			for (size_t i = 0; i < vk_ctx->available_surface_format_count; i++)
			{
				if (vk_ctx->available_surface_formats[i].format == surface_format.format &&
					vk_ctx->available_surface_formats[i].colorSpace == surface_format.colorSpace)
				{
					vk_ctx->swapchain_create_info_data.imageFormat = vk_ctx->available_surface_formats[i].format;
					vk_ctx->swapchain_create_info_data.imageColorSpace = vk_ctx->available_surface_formats[i].colorSpace;
					return true;
				}
			}
		}
		WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan",
			std::format("Failed to find surface formats!\tError code: {0}",
				static_cast<int64_t>(VK_ERROR_FORMAT_NOT_SUPPORTED)).c_str());
		return false;		
	}

	VkResult recreate_swapchain(VulkanContext* vk_ctx, vec2u window_size)
	{
		VkSurfaceCapabilitiesKHR surface_capabilities = {};
		VkResult result;
		vk_ctx->swapchain_create_info_data.oldSwapchain = vk_ctx->swapchain;
		result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vk_ctx->cur_used_gpu, vk_ctx->window_bitmap_surface, &surface_capabilities);
		if (result)
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", 
				std::format("Failed to get physical device surface capabilities!\tError code : {0}",
				static_cast<int64_t>(result)).c_str());
			return result;
		}
		// is window min
		if (surface_capabilities.currentExtent.width == 0 || surface_capabilities.currentExtent.height == 0)
			return VK_SUBOPTIMAL_KHR;
		vk_ctx->swapchain_create_info_data.imageExtent = surface_capabilities.currentExtent;

		// wait queue is end
		if (vk_ctx->queue_mod.queue_list.graphics_queue_inst.index ==
			vk_ctx->queue_mod.queue_list.presentation_queue_inst.index)
		{
			// graphics_queue is must been created
			result = vkQueueWaitIdle(vk_ctx->queue_mod.queue_list.graphics_queue_inst.queue);
			if (result) goto QUEUE_WAIT_ERROR;
		}
		else
		{
			result = vkQueueWaitIdle(vk_ctx->queue_mod.queue_list.graphics_queue_inst.queue);
			if (result) goto QUEUE_WAIT_ERROR;

			result = vkQueueWaitIdle(vk_ctx->queue_mod.queue_list.presentation_queue_inst.queue);
			if (result) goto QUEUE_WAIT_ERROR;
		}
		for (uint32_t i = 0; i < vk_ctx->swapchain_image_count; i++)
		{
			vkDestroyImageView(vk_ctx->vk_logic_vkdevice, vk_ctx->swapchain_image_views[i], pvk_allocator);
		}
		if (create_swapchain(vk_ctx, window_size, vk_ctx->swapchain_create_info_data.minImageCount,
			vk_ctx->alpha_window, vk_ctx->tty_screen, vk_ctx->limit_frame_rate))
			return VK_ERROR_INITIALIZATION_FAILED;

		return VK_SUCCESS;
	QUEUE_WAIT_ERROR:
		WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan",
			std::format("Failed to wait for the queue to be idle!\tError code: {0}",
			static_cast<int64_t>(result)).c_str());
		return result;
	}

	static ResultInfo get_gpu_suface_support(VulkanContext* vk_ctx)
	{
		VkResult result;
		result = vkGetPhysicalDeviceSurfaceFormatsKHR(vk_ctx->cur_used_gpu, vk_ctx->window_bitmap_surface,
			&vk_ctx->available_surface_format_count, nullptr);
		if (result)
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan",
				std::format("Failed to get the count of surface formats!\tError code: {0}",
					static_cast<int64_t>(result)).c_str());
			return ResultInfo::WR_ERROR;
		}
		if (!vk_ctx->available_surface_format_count)
		{
			WR_FATAL_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan",
				"Failed to find any supported surface format!");
		}
		vk_ctx->available_surface_formats = wr_malloc<VkSurfaceFormatKHR>(vk_ctx->available_surface_format_count);

		result = vkGetPhysicalDeviceSurfaceFormatsKHR(vk_ctx->cur_used_gpu, vk_ctx->window_bitmap_surface,
			&(vk_ctx->available_surface_format_count), vk_ctx->available_surface_formats);
		if (result)
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan",
				std::format("Failed to get surface formats!\tError code: {}\n", static_cast<int64_t>(result)).c_str());
			return ResultInfo::WR_ERROR;
		}
		return ResultInfo::WR_OK;
	}

	static ResultInfo get_surface_present_support(VulkanContext* vk_ctx)
	{
		VkResult result;
		uint32_t old_surface_present_mode_count = vk_ctx->surface_present_mode_count;
		result = vkGetPhysicalDeviceSurfacePresentModesKHR(vk_ctx->cur_used_gpu, vk_ctx->window_bitmap_surface,
			&vk_ctx->surface_present_mode_count, nullptr);
		if (result)
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan",
				std::format("Failed to get the count of surface present modes!\t Error code: {0}",
					static_cast<int64_t>(result)).c_str());
			return ResultInfo::WR_ERROR;
		}
		if (!vk_ctx->surface_present_mode_count)
			WR_FATAL_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan",
				"Failed to find any surface present mode!");
		if (vk_ctx->surface_present_modes != nullptr)
		{
			if (old_surface_present_mode_count != vk_ctx->surface_present_mode_count)
				vk_ctx->surface_present_modes = wr_realloc<VkPresentModeKHR>(vk_ctx->surface_present_modes,
					vk_ctx->surface_present_mode_count);
		}
		else
		{
			vk_ctx->surface_present_modes = wr_malloc<VkPresentModeKHR>(vk_ctx->surface_present_mode_count);
		}
		
		result = vkGetPhysicalDeviceSurfacePresentModesKHR(vk_ctx->cur_used_gpu, vk_ctx->window_bitmap_surface,
			&vk_ctx->surface_present_mode_count, vk_ctx->surface_present_modes);
		if (result)
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan",
				std::format("Failed to get surface present modes!\tError code: {0}", static_cast<int64_t>(result)).c_str()
			);
			return ResultInfo::WR_ERROR;
		}
		return ResultInfo::WR_OK;
	}

	ResultInfo create_swapchain(VulkanContext* vk_ctx,
		vec2u window_size,
		uint32_t cache_surface_count,
		bool alpha_window,
		bool tty_screen,
		bool limit_frame_rate)
	{
		VkSwapchainCreateFlagsKHR flags = 0;
		VkSurfaceCapabilitiesKHR surface_capabilities = {};
		VkResult result;
		char is_running = 'Y';

		vk_ctx->alpha_window			= alpha_window;
		vk_ctx->tty_screen				= tty_screen;
		vk_ctx->limit_frame_rate		= limit_frame_rate;

		result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vk_ctx->cur_used_gpu, vk_ctx->window_bitmap_surface, &surface_capabilities);
		if (result)
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan",
				std::format("Failed to get physical device surface capabilities!\tError code: {0}",
					static_cast<int64_t>(result)).c_str());
			return ResultInfo::WR_ERROR;
		}
		vk_ctx->swapchain_create_info_data.minImageCount =
			std::clamp(cache_surface_count, surface_capabilities.minImageCount + 1, surface_capabilities.maxImageCount);
		surface_capabilities.currentExtent =
		{
			std::clamp(window_size.x, surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width),
			std::clamp(window_size.y, surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height)
		};
		vk_ctx->swapchain_create_info_data.imageExtent = surface_capabilities.currentExtent;
		vk_ctx->swapchain_create_info_data.imageArrayLayers = 1;
		vk_ctx->swapchain_create_info_data.preTransform = surface_capabilities.currentTransform;

		// the desktop that not support alpha, such as niri and so on
		if (surface_capabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR //windowed
			&& (!tty_screen) && (!alpha_window))
			vk_ctx->swapchain_create_info_data.compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
		else
		{
			vk_ctx->swapchain_create_info_data.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		}
		vk_ctx->swapchain_create_info_data.flags = flags;

		vk_ctx->swapchain_create_info_data.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		// The tty not support print screen <VK_IMAGE_USAGE_TRANSFER_SRC_BIT>
		if (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT ||
			tty_screen)
			vk_ctx->swapchain_create_info_data.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		if (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
			vk_ctx->swapchain_create_info_data.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		else
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan",
				std::format("VK_IMAGE_USAGE_TRANSFER_DST_BIT isn't supported!").c_str());
			return ResultInfo::WR_ERROR;
		}
		//////////////////////////////////////////////////////////////////////////
		if (get_gpu_suface_support(vk_ctx))
		{
			return ResultInfo::WR_ERROR;
		}

		if (!vk_ctx->swapchain_create_info_data.imageFormat)
		{
			if (tty_screen || (!alpha_window))
			{
				if (set_surface_format(vk_ctx, { VK_FORMAT_R8G8B8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR }))
					goto END_OF_SET_IMAGE_FORMAT;
				else { goto FAILD_OF_SET_IMAGE_FORMAT; }
			}
			else
			{
				if (set_surface_format(vk_ctx, { VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR }))
					goto END_OF_SET_IMAGE_FORMAT;
				else
				{
					WR_CLR_WRITE_LINE("Do you want to run this program that doesn't run without alpha window? [Y : YES N : NO]", WR_CLR_COLOR::YELLOW);
					std::cin >> is_running;
					if (is_running == 'Y')
					if (set_surface_format(vk_ctx, { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR }))
						goto END_OF_SET_IMAGE_FORMAT;
					return ResultInfo::WR_ERROR;
				}
			}
		FAILD_OF_SET_IMAGE_FORMAT:
			vk_ctx->swapchain_create_info_data.imageFormat = vk_ctx->available_surface_formats[0].format;
			vk_ctx->swapchain_create_info_data.imageColorSpace = vk_ctx->available_surface_formats[0].colorSpace;
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan",
				"Failed to select a four-component UNORM surface format!");
			return ResultInfo::WR_ERROR;
		}
	END_OF_SET_IMAGE_FORMAT:
		//////////////////////////////////////////////////////////////////////////////
		if (get_surface_present_support(vk_ctx))
		{
			return ResultInfo::WR_ERROR;
		}

		vk_ctx->swapchain_create_info_data.presentMode = VK_PRESENT_MODE_FIFO_KHR;

		// This environment not has window synth which is linux in tty
		// The VK_PRESENT_MODE_MAILBOX_KHR is fucking carzy joke
		// Fuck windows window synth
		if (tty_screen) limit_frame_rate = false;
		if (!limit_frame_rate)
		{
			for (size_t i = 0; i < vk_ctx->surface_present_mode_count; i++)
				if (vk_ctx->surface_present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
				{
					vk_ctx->swapchain_create_info_data.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
					break;
				}
		}

		// the swap chain is used in graphics queue and presentation queue
		if (vk_ctx->queue_mod.need_queue_count > 1)
		{
			if (vk_ctx->queue_mod.queue_list.graphics_queue_inst.index !=
				vk_ctx->queue_mod.queue_list.presentation_queue_inst.index)
			{
				vk_ctx->swapchain_create_info_data.queueFamilyIndexCount = 2;
				vk_ctx->swapchain_create_info_data.pQueueFamilyIndices =
					reinterpret_cast<uint32_t*>(&(vk_ctx->queue_mod.need_queues));
			}
			else
			{
				vk_ctx->swapchain_create_info_data.queueFamilyIndexCount = 1;
				vk_ctx->swapchain_create_info_data.pQueueFamilyIndices =
					reinterpret_cast<uint32_t*>(&(vk_ctx->queue_mod.need_queues));
			}
		}
		else
		{
			vk_ctx->swapchain_create_info_data.queueFamilyIndexCount = 0;
			vk_ctx->swapchain_create_info_data.pQueueFamilyIndices = nullptr;
		}


		vk_ctx->swapchain_create_info_data.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		vk_ctx->swapchain_create_info_data.flags = flags;
		vk_ctx->swapchain_create_info_data.surface = vk_ctx->window_bitmap_surface;
		vk_ctx->swapchain_create_info_data.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		vk_ctx->swapchain_create_info_data.clipped = VK_TRUE;
		vk_ctx->swapchain_create_info_data.pNext = nullptr;

		result = vkCreateSwapchainKHR(vk_ctx->vk_logic_vkdevice, &vk_ctx->swapchain_create_info_data, pvk_allocator, &(vk_ctx->swapchain));
		if (result) 
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", 
				std::format("Failed to create a swapchain!\nError code: {}\n", static_cast<int64_t>(result)).c_str());
			return ResultInfo::WR_ERROR;
		}
		vk_ctx->swapchain_create_info_data = vk_ctx->swapchain_create_info_data;
		return ResultInfo::WR_OK;
	}

	ResultInfo create_image_view(VulkanContext* vk_ctx)
	{
		VkResult result;
		result = vkGetSwapchainImagesKHR(vk_ctx->vk_logic_vkdevice,
			vk_ctx->swapchain, &(vk_ctx->swapchain_image_count), nullptr);
		if (result)
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", 
				std::format("Failed to get the count of swapchain images!\tError code: {0}", 
					static_cast<int64_t>(result)).c_str());
			return ResultInfo::WR_ERROR;
		}
		vk_ctx->swapchain_images = wr_malloc<VkImage>(vk_ctx->swapchain_image_count);
		result = vkGetSwapchainImagesKHR(vk_ctx->vk_logic_vkdevice,
			vk_ctx->swapchain, &(vk_ctx->swapchain_image_count), vk_ctx->swapchain_images);
		if (result)
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", 
				std::format("Failed to get swapchain images!\tError code: {0}", 
					static_cast<int64_t>(result)).c_str());
			return ResultInfo::WR_ERROR;
		}

		VkImageViewCreateInfo imageViewCreateInfo = {
			.sType				= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.viewType			= VK_IMAGE_VIEW_TYPE_2D,
			.format				= vk_ctx->swapchain_create_info_data.imageFormat,
			//.components		= {}, //VK_COMPONENT_SWIZZLE_IDENTITY
			.subresourceRange	= { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
		};

		vk_ctx->swapchain_image_views = wr_malloc<VkImageView>(vk_ctx->swapchain_image_count);
		
		for (size_t i = 0; i < vk_ctx->swapchain_image_count; i++) 
		{
			imageViewCreateInfo.image = vk_ctx->swapchain_images[i];
			result = vkCreateImageView(vk_ctx->vk_logic_vkdevice,
				&imageViewCreateInfo, nullptr, &(vk_ctx->swapchain_image_views[i]));
			if (result)
			{
				WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", 
					std::format("Failed to create a swapchain image view!\tError code: {0}", 
						static_cast<int64_t>(result)).c_str());
				return ResultInfo::WR_ERROR;
			}
		}
		return ResultInfo::WR_OK;
	}

	void release_vulkan_ctx(VulkanContext* vk_ctx) noexcept
	{
		if (vk_ctx->swapchain_image_views != nullptr)
		{
			for (uint32_t i = 0; i < vk_ctx->swapchain_image_count; i++)
			{
				if ((vk_ctx->swapchain_image_views)[i] != nullptr)
					vkDestroyImageView(vk_ctx->vk_logic_vkdevice, (vk_ctx->swapchain_image_views)[i], pvk_allocator);
			}
		}
		wr_free(vk_ctx->swapchain_image_views);
		vk_ctx->swapchain_image_views = nullptr;
		
		wr_free(vk_ctx->swapchain_images);
		vk_ctx->swapchain_images = nullptr;

		wr_free(vk_ctx->surface_present_modes);
		vk_ctx->surface_present_modes = nullptr;

		wr_free(vk_ctx->available_surface_formats);
		vk_ctx->available_surface_formats = nullptr;

		free_queue_info(vk_ctx->queue_mod);

		wr_free(vk_ctx->vk_gpu_extensions);
		vk_ctx->vk_gpu_extensions = nullptr;

		wr_free(vk_ctx->vk_gpu_properties);
		vk_ctx->vk_gpu_properties = nullptr;

		wr_free(vk_ctx->vk_gpu_features);
		vk_ctx->vk_gpu_features = nullptr;

#ifdef _DEBUG
		wr_free(vk_ctx->instance_layers);
		vk_ctx->instance_layers = nullptr;
#endif // _DEBUG
		vkDestroySwapchainKHR(vk_ctx->vk_logic_vkdevice, vk_ctx->swapchain, pvk_allocator);
		vkDestroyDevice(vk_ctx->vk_logic_vkdevice, pvk_allocator);
		vkDestroySurfaceKHR(vk_ctx->vk_main_instance, vk_ctx->window_bitmap_surface, pvk_allocator);
		vkDestroyInstance(vk_ctx->vk_main_instance, pvk_allocator);
	}
} // namespace wr is end