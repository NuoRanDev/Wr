#ifndef _INC_WR_WINDOWS_PLATFORM_WINDOW_HPP_
#define _INC_WR_WINDOWS_PLATFORM_WINDOW_HPP_

#if defined(_WIN32)
// os api
#include <Windows.h>
// vulkan
#include <vulkan/vulkan.h>
// core
#include <string/wrString.hpp>
// window
#include <wrWindowProperty.hpp>

namespace wr
{
	bool init_windows_env() noexcept;

	any_type_ptr_t create_window(U16StringRef& window_name, int32_t w, int32_t h, uint32_t style) noexcept;

	bool switch_enevt() noexcept;

	bool get_vulkan_surface(VkInstance vk_inst, any_type_ptr_t window_ptr, const VkAllocationCallbacks* cb_allocer, VkSurfaceKHR& out_surface) noexcept;

	constexpr unsigned long get_window_style(const uint32_t style)
	{
		unsigned long win32_style = WS_VISIBLE;
		if (style == WindowStyle::None)
		{
			win32_style |= WS_POPUP;
		}
		else
		{
			if (style & WindowStyle::Titlebar)
				win32_style |= WS_CAPTION | WS_MINIMIZEBOX;
			if (style & WindowStyle::Resize)
				win32_style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
			if (style & WindowStyle::Close)
				win32_style |= WS_SYSMENU;
		}
		return win32_style;
	}
} // namespace wr is end

#endif // defined(_WIN32) is end

#endif // _INC_WR_WINDOWS_PLATFORM_WINDOW_HPP_ IS EOF