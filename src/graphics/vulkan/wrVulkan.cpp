// statement
#include <vulkan/wrVulkan.hpp>
// core
#include <log/wrLogOutput.hpp>
#include <string/wrString.hpp>
#include <memory/wrAlloc.hpp>
// std
#include <format>
#include <iostream>
// graphics
#include <vulkan/wrVulkanConfig.hpp>

#ifdef _DEBUG
const char* const instance_validation_layers_alt1[] =
{
	"VK_LAYER_LUNARG_standard_validation",
	"VK_EXT_debug_utils"
};

const char* const instance_validation_layers_alt2[] =
{
	"VK_LAYER_GOOGLE_threading",       "VK_LAYER_LUNARG_parameter_validation",
	"VK_LAYER_LUNARG_object_tracker",  "VK_LAYER_LUNARG_image",
	"VK_LAYER_LUNARG_core_validation", "VK_LAYER_LUNARG_swapchain",
	"VK_LAYER_GOOGLE_unique_objects"
};
#endif // _DEBUG is end

namespace wr
{
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


	bool check_vulkan_validation_layers(uint32_t check_count, const char * const* check_names,
		uint32_t layer_count, VkLayerProperties* layers) noexcept
	{
		uint32_t i, j;
		bool found = true;

		for (i = 0; i < check_count; i++)
		{
			for (j = 0; j < layer_count; j++)
			{
				if (!strcmp(check_names[i], layers[j].layerName))
				{
					found = false;
					break;
				}
			}
			if (!found)
			{
				WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", std::format("Cannot find layer: {}", check_names[i]).c_str());
				return false;
			}
		}
		return true;
	}

	void init_vulkan_instance(VulkanContext& vk_ctx, const utf8_t* app_name) noexcept
	{
		VkInstanceCreateInfo vk_inst_info;
		VkResult state;
		uint32_t instance_layer_count = 0;
		const char* const* instance_validation_layers;

#ifdef _DEBUG
		char is_running = '\0';
#endif // _DEBUG
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
		state = vkEnumerateInstanceLayerProperties(&instance_layer_count, nullptr);
		if (state != VK_SUCCESS) WR_FATAL_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "Find validation layer fatal!");
		if (instance_layer_count > 0)
		{
			vk_ctx.instance_layers = wr_malloc<VkLayerProperties>(instance_layer_count);
			state = vkEnumerateInstanceLayerProperties(&instance_layer_count, vk_ctx.instance_layers);
			if (state != VK_SUCCESS) WR_FATAL_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "Find validation layer fatal!");
		}
		else
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "System doesn't have validation layer library");
			goto IS_RUNNING_WITHOUT_VALIDATION_LAYERS;
		}

		if (instance_layer_count > 0)
		{
			bool validation_found = check_vulkan_validation_layers(1, instance_validation_layers_alt1, instance_layer_count, vk_ctx.instance_layers);
			if (validation_found)
			{
				instance_validation_layers = instance_validation_layers_alt1;
				instance_layer_count = 1;
				goto END_OF_FIND_VALIDATION_LAYERS;
			}
			else
			{
				bool validation_found = check_vulkan_validation_layers(7, instance_validation_layers_alt2, instance_layer_count, vk_ctx.instance_layers);
				if (validation_found)
				{
					instance_validation_layers = instance_validation_layers_alt2;
					instance_layer_count = 7;
					goto END_OF_FIND_VALIDATION_LAYERS;
				}
				else
				{
					WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "the validation layer library doesn't support this");
					goto IS_RUNNING_WITHOUT_VALIDATION_LAYERS;
				}
			}
		}

	IS_RUNNING_WITHOUT_VALIDATION_LAYERS:
		WR_CLR_WRITE_LINE("Do you want run this program that doesn't run validation layer? [Y : YES N : NO]", WR_CLR_COLOR::YELLOW);
		std::cin >> is_running;
		if (is_running == 'Y')
		{
			WR_CLR_WRITE_LINE("Not run validation layer");
			instance_validation_layers = nullptr;
			instance_layer_count = 0;
			goto END_OF_FIND_VALIDATION_LAYERS;
		}
		else std::exit(EXIT_FAILURE);
	END_OF_FIND_VALIDATION_LAYERS:
#else
		instance_layer_count = 0;
		instance_validation_layers = nullptr;
#endif // _DEBUG
		if (window_vulkan_enabled_extension_count == 0)
		{
			WR_FATAL_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "GUI library can't support vulkan!");
		}
		vk_inst_info =
		{
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext = nullptr,
			.pApplicationInfo = &app,
			.enabledLayerCount = instance_layer_count,
			.ppEnabledLayerNames = instance_validation_layers,
			.enabledExtensionCount = window_vulkan_enabled_extension_count,
			.ppEnabledExtensionNames = window_vulkan_enabled_extension,
		};
		state = vkCreateInstance(&vk_inst_info, pvk_allocator, &(vk_ctx.vk_main_instance));
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
			// vkDestroyInstance(vk_ctx.vk_main_instance, pvk_allocator);
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "System doesn't have validation layer library");
			goto IS_RUNNING_WITHOUT_VALIDATION_LAYERS;
		}
#endif // _DEBUG
		else if (state != VK_SUCCESS) {
			WR_FATAL_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "vkCreateInstance failed.\n\nDo you have a compatible Vulkan!");
		}
	}

	// Copy gpu information to "VulkanContext"
	void find_gpu(VulkanContext& vk_ctx) noexcept
	{
		VkResult state;
		state = vkEnumeratePhysicalDevices(vk_ctx.vk_main_instance, &(vk_ctx.gpu_cout), nullptr);
		if (state != VK_SUCCESS || vk_ctx.gpu_cout == 0)
			WR_FATAL_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "No GPU!");
		vk_ctx.gpu_list = wr::wr_malloc<VkPhysicalDevice>(vk_ctx.gpu_cout);
		state = vkEnumeratePhysicalDevices(vk_ctx.vk_main_instance, &(vk_ctx.gpu_cout), vk_ctx.gpu_list);
		if (state != VK_SUCCESS || vk_ctx.gpu_cout == 0)
			WR_FATAL_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "Enumerate gpu failed!");

		vk_ctx.vk_gpu_properties = wr_malloc<VkPhysicalDeviceProperties>(vk_ctx.gpu_cout);
		vk_ctx.vk_gpu_features = wr_malloc<VkPhysicalDeviceFeatures>(vk_ctx.gpu_cout);

		for (uint32_t i = 0; i < vk_ctx.gpu_cout; i++)
		{
			vkGetPhysicalDeviceProperties(vk_ctx.gpu_list[i], vk_ctx.vk_gpu_properties + i);
			std::cout << "Find Gpu[" << i << "]: " << vk_ctx.vk_gpu_properties[i].deviceName << std::endl;
			vkGetPhysicalDeviceFeatures(vk_ctx.gpu_list[i], vk_ctx.vk_gpu_features + i);
		}

	}

	// create queue
	bool get_queue_family_indices(VulkanContext* vk_ctx, bool enable_graphics_queue, bool enable_compute_queue) noexcept
	{
		uint32_t ig, ip, ic;
		VkResult result;

		vkGetPhysicalDeviceQueueFamilyProperties(vk_ctx->cur_used_gpu, &(vk_ctx->queue_family_count), nullptr);
		if (!vk_ctx->queue_family_count)
		{
			WR_FATAL_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "No queue family!");
			WR_CLR_WRITE_LINE("Update your gpu device", WR_CLR_COLOR::YELLOW);
			std::exit(EXIT_FAILURE);
		}
		vk_ctx->queue_family_propertieses = wr_malloc<VkQueueFamilyProperties>(vk_ctx->queue_family_count);
		vkGetPhysicalDeviceQueueFamilyProperties(vk_ctx->cur_used_gpu, &(vk_ctx->queue_family_count), vk_ctx->queue_family_propertieses);

		ig = ip = ic = VK_QUEUE_FAMILY_IGNORED;
		for (uint32_t i = 0; i < vk_ctx->queue_family_count; i++) 
		{
			VkBool32 must_support_graphics = enable_graphics_queue && static_cast<bool>(vk_ctx->queue_family_propertieses[i].queueFlags & VK_QUEUE_GRAPHICS_BIT);
			VkBool32 is_support_presentation = false;
			VkBool32 is_support_compute = enable_compute_queue && static_cast<bool>(vk_ctx->queue_family_propertieses[i].queueFlags & VK_QUEUE_COMPUTE_BIT);
			// is support current window
			if (vk_ctx->window_bitmap_surface)
			{
				result = vkGetPhysicalDeviceSurfaceSupportKHR(vk_ctx->cur_used_gpu, i, vk_ctx->window_bitmap_surface, &is_support_presentation);
				if (result)
				{
					WR_FATAL_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", 
						std::format("Failed to determine if the queue family supports presentation!\tError code: {0}", static_cast<uint64_t>(result)).c_str());
					return false;
				}
			}
			if (must_support_graphics && is_support_compute) 
			{
				if (is_support_presentation) 
				{
					ig = ip = ic = i;
					break;
				}
				if (ig != ic ||
					ig == VK_QUEUE_FAMILY_IGNORED)
					ig = ic = i;
				// if not show, break
				if (!vk_ctx->window_bitmap_surface)
					break;
			}
			if (must_support_graphics && ig == VK_QUEUE_FAMILY_IGNORED)
				ig = i;
			if (is_support_presentation && ip == VK_QUEUE_FAMILY_IGNORED)
				ip = i;
			if (is_support_compute && ic == VK_QUEUE_FAMILY_IGNORED)
				ic = i;
		}
		if (ig == VK_QUEUE_FAMILY_IGNORED && static_cast<bool>(enable_graphics_queue) ||
			ip == VK_QUEUE_FAMILY_IGNORED && static_cast<bool>(vk_ctx->window_bitmap_surface) ||
			ic == VK_QUEUE_FAMILY_IGNORED && static_cast<bool>(enable_compute_queue))
		{
			WR_FATAL_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "No supported queue family!");
			WR_CLR_WRITE_LINE("Update your gpu device", WR_CLR_COLOR::YELLOW);
			std::exit(EXIT_FAILURE);
		}
		vk_ctx->graphics_queue_index = ig;
		vk_ctx->presentation_queue_index = ip;
		vk_ctx->compute_queue_index = ic;
		return true;
	}

	void release_vulkan_ctx(VulkanContext& vk_ctx) noexcept
	{
#ifdef _DEBUG
		wr_free(vk_ctx.instance_layers);
		vk_ctx.instance_layers = nullptr;
#endif // _DEBUG
		
		wr_free(vk_ctx.vk_gpu_properties);
		vk_ctx.vk_gpu_properties = nullptr;

		wr_free(vk_ctx.vk_gpu_features);
		vk_ctx.vk_gpu_features = nullptr;

		wr_free(vk_ctx.queue_family_propertieses);
		vk_ctx.queue_family_propertieses = nullptr;

		vkDestroySurfaceKHR(vk_ctx.vk_main_instance, vk_ctx.window_bitmap_surface, pvk_allocator);
		vkDestroyInstance(vk_ctx.vk_main_instance, pvk_allocator);
	}
} // namespace wr is end