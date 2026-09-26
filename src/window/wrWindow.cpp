// statement
#include <wrWindow.hpp>
// graphis
#include <wrGraphicsInstance.hpp>
#include <vulkan/wrVulkan.hpp>
// core
#include <log/wrLogOutput.hpp>
#include <memory/wrAlloc.hpp>
// std
#include <cstring>
#include <format>

// window
#if defined(_WIN32)
#include <platform/wrWindowsPlatformWindow.hpp>
#elif defined(WAYLAND)
#elif defined(X11)
#endif // os window platform

namespace wr
{
	ResultInfo init_wr_window_ctx()
	{
		ResultInfo state = init_windows_env();
		if(state)
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrWindow", "Init model window env error");
		WR_INFO_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrWindow", "Init model window env")
		return state;
	}

	ResultInfo Window::create_window(String& window_name, vec2u size, uint32_t style) noexcept
	{
		VkInstance pvk_inst = reinterpret_cast<VkInstance>(get_vk_inst());
		VkSurfaceKHR pvk_surface;
#if defined(_WIN32)
		// windows string is utf16 format
		U16StringRef win_str_window_name = window_name;
		window_hwnd = create_windows_window(win_str_window_name, size.x, size.y, style);
		if(!window_hwnd)
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrWindow", "Create window failed!");
			return ResultInfo::WR_ERROR;
		}
#else
#endif // window platform

		if (get_vulkan_surface(pvk_inst, window_hwnd, pvk_allocator, &pvk_surface))
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrWindow", "Create window surface failed!");
			return ResultInfo::WR_ERROR;
		}

		if (create_swapchain(vk_ctx, size, 3, true, false, true))
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrWindow", "Create vulkan logic device failed!");
			return ResultInfo::WR_ERROR;
		}
		if(create_image_view(vk_ctx))
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrWindow", "Create vulkan image view failed!");
			return ResultInfo::WR_ERROR;
		}
		return ResultInfo::WR_OK;
	}

	rectu Window::get_window_size() const
	{
		rectu size;
#if defined(_WIN32)
		size = get_windows_window_rect(window_hwnd);
#else
#endif // window platform
		return size;
	}

	ResultInfo Window::event()
	{
#if defined(_WIN32)
		return switch_event();
#elif defined(WAYLAND)
#elif defined(X11)
#endif // window platform
	}

	Window::~Window()
	{
		VulkanContext* vk_ctx = reinterpret_cast<VulkanContext*>(vulkan_ctx);
		release_vulkan_ctx(vk_ctx);
		wr_free(vk_ctx);
	}
} // namespace wr is end