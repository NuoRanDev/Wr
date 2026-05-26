#include "vulkan/wrVulkan.hpp"

#include "log/wrLogOutput.hpp"
#include "memory/wrAlloc.hpp"

#include <format>
#include <iostream>

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
				XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", std::format("Cannot find layer: {}", check_names[i]).c_str());
				return false;
			}
		}
		return true;
	}

	void init_vulkan_instance(VulkanContext& vk_ctx, const utf8_t* app_name,
		const char* const* vulkan_enabled_extensions, uint32_t vulkan_enabled_extension_count) noexcept
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
		if (state != VK_SUCCESS) XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "Find validation layer fatal!");
		if (instance_layer_count > 0)
		{
			vk_ctx.instance_layers = wr_malloc<VkLayerProperties>(instance_layer_count);
			state = vkEnumerateInstanceLayerProperties(&instance_layer_count, vk_ctx.instance_layers);
			if (state != VK_SUCCESS) XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "Find validation layer fatal!");
		}
		else
		{
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "System doesn't have validation layer library");
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
					XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "the validation layer library doesn't support this");
					goto IS_RUNNING_WITHOUT_VALIDATION_LAYERS;
				}
			}
		}

	IS_RUNNING_WITHOUT_VALIDATION_LAYERS:
		XE_CLR_WRITE_LINE("Do you want run this program that doesn't run validation layer? [Y : YES N : NO]", XE_CLR_COLOR::YELLOW);
		std::cin >> is_running;
		if (is_running == 'Y')
		{
			XE_CLR_WRITE_LINE("Not run validation layer");
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
		if (vulkan_enabled_extension_count == 0)
		{
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "GUI library can't support vulkan!");
		}
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
		state = vkCreateInstance(&vk_inst_info, pvk_allocator, &(vk_ctx.vk_main_instance));
		if (state == VK_SUCCESS) return;
		else if (state == VK_ERROR_INCOMPATIBLE_DRIVER)
		{
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "Cannot find a compatible Vulkan installable client driver!");
		}
		else if (state == VK_ERROR_EXTENSION_NOT_PRESENT) {
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "Cannot find a specified extension library!");
		}
#ifdef _DEBUG
		else if (state == VK_ERROR_LAYER_NOT_PRESENT)
		{
			vkDestroyInstance(vk_ctx.vk_main_instance, pvk_allocator);
			XE_ERROR_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "System doesn't have validation layer library");
			goto IS_RUNNING_WITHOUT_VALIDATION_LAYERS;
		}
#endif // _DEBUG
		else if (state != VK_SUCCESS) {
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "vkCreateInstance failed.\n\nDo you have a compatible Vulkan!");
		}
	}

	void find_gpu(VulkanContext& vk_ctx) noexcept
	{
		VkResult state;
		state = vkEnumeratePhysicalDevices(vk_ctx.vk_main_instance, &(vk_ctx.gpu_cout), nullptr);
		if (state != VK_SUCCESS || vk_ctx.gpu_cout == 0)
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "No GPU!");
		vk_ctx.gpu_list = wr::wr_malloc<VkPhysicalDevice>(vk_ctx.gpu_cout);
		state = vkEnumeratePhysicalDevices(vk_ctx.vk_main_instance, &(vk_ctx.gpu_cout), vk_ctx.gpu_list);
		if (state != VK_SUCCESS || vk_ctx.gpu_cout == 0)
			XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::LIB, "wrGraphics::vulkan", "Enumerate gpu failed!");

		vk_ctx.vk_gpu_properties = wr_malloc<VkPhysicalDeviceProperties>(vk_ctx.gpu_cout);
		vk_ctx.vk_gpu_features = wr_malloc<VkPhysicalDeviceFeatures>(vk_ctx.gpu_cout);

		for (uint32_t i = 0; i < vk_ctx.gpu_cout; i++)
		{
			vkGetPhysicalDeviceProperties(vk_ctx.gpu_list[i], vk_ctx.vk_gpu_properties + i);
			std::cout << "Find Gpu[" << i << "]: " << vk_ctx.vk_gpu_properties[i].deviceName << std::endl;
			vkGetPhysicalDeviceFeatures(vk_ctx.gpu_list[i], vk_ctx.vk_gpu_features + i);
		}

	}

	void release_vulkan_ctx(VulkanContext& vk_ctx) noexcept
	{
#ifdef _DEBUG
		wr_free(vk_ctx.instance_layers);
#endif // _DEBUG
		wr_free(vk_ctx.vk_gpu_properties);
		wr_free(vk_ctx.vk_gpu_features);

		vkDestroyInstance(vk_ctx.vk_main_instance, pvk_allocator);
	}
} // namespace wr is end