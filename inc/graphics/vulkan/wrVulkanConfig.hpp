#ifndef _INC_WR_VULKAN_CONFIG_HPP_
#define _INC_WR_VULKAN_CONFIG_HPP_
// vulkan
#include <vulkan/vulkan.h>
// core
#include <type/wrOrdinals.hpp>

// window
#if defined(_WIN32)
const char* const	window_vulkan_enabled_extension[]		= {"VK_KHR_win32_surface", "VK_KHR_surface"};
const uint32_t		window_vulkan_enabled_extension_count	= 2;
#elif defined(WAYLAND)
#elif defined(X11)
#endif // os window platform

#endif // _INC_WR_VULKAN_CONFIG_HPP_ IS EOF