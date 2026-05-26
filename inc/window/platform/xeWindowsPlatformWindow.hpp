#ifndef _INC_WR_WINDOWS_PLATFORM_WINDOW_HPP_
#define _INC_WR_WINDOWS_PLATFORM_WINDOW_HPP_

#include "string/wrString.hpp"

#include <vulkan/vulkan.h>

#if defined(_WIN32)

namespace wr
{
	bool init_windows_env() noexcept;

	any_type_ptr_t create_window(U16StringRef& window_name, int32_t w, int32_t h) noexcept;

	bool switch_enevt() noexcept;

	bool get_vulkan_surface(VkInstance vk_inst, any_type_ptr_t window_ptr, const VkAllocationCallbacks* cb_allocer, VkSurfaceKHR& out_surface) noexcept;
}

#endif // defined(_WIN32) is end

#endif // _INC_WR_WINDOWS_PLATFORM_WINDOW_HPP_ IS EOF