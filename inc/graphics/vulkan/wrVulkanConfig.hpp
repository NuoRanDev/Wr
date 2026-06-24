#ifndef _INC_WR_VULKAN_CONFIG_HPP_
#define _INC_WR_VULKAN_CONFIG_HPP_
// vulkan
#include <vulkan/vulkan.h>
// core
#include <type/wrOrdinals.hpp>

// window
#if defined(_WIN32)
const char* const	vulkan_enabled_extensions[] =
{
	"VK_KHR_win32_surface", "VK_KHR_surface"
};
constexpr uint32_t		vulkan_enabled_extension_count = 2;
#elif defined(WAYLAND)
#elif defined(X11)
#endif // os window platform

#ifdef _DEBUG
const char* const instance_validation_layers_alt1[] =
{
	"VK_LAYER_KHRONOS_validation",
};

constexpr uint32_t instance_validation_layers_alt1_count = 1;

const char* const instance_validation_layers_alt2[] =
{
	"VK_LAYER_GOOGLE_threading",       "VK_LAYER_LUNARG_parameter_validation",
	"VK_LAYER_LUNARG_object_tracker",  "VK_LAYER_LUNARG_image",
	"VK_LAYER_LUNARG_core_validation", "VK_LAYER_LUNARG_swapchain",
	"VK_LAYER_GOOGLE_unique_objects"
};

constexpr uint32_t instance_validation_layers_alt2_count = 7;
constexpr VkDeviceQueueCreateFlags queue_flags = 0;
#else
constexpr VkDeviceQueueCreateFlags queue_flags = 0;//VK_DEVICE_QUEUE_CREATE_PROTECTED_BIT;
#endif // _DEBUG is end

const char* const device_enabled_extensions[] = { "VK_KHR_swapchain" };
constexpr uint32_t device_enabled_extension_count = 1;

constexpr float graphics_queue_priority = 1.0f;
constexpr float presentation_queue_priority = 1.0f;
constexpr float compute_queue_priority = 1.0f;
constexpr float av1_video_dec_queue_priority = 0.5f;

#endif // _INC_WR_VULKAN_CONFIG_HPP_ IS EOF