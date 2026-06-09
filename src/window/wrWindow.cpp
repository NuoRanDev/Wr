// statement
#include <wrWindow.hpp>
// graphis
#include <vulkan/wrVulkan.hpp>
// core
#include <log/wrLogOutput.hpp>
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
	bool init_wr_window_ctx()
	{
		bool state = init_windows_env();
		if(!state)
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrWindow", "Init model window env error");
		return state;
	}

	bool Window::create_window(String& window_name, int32_t w, int32_t h, uint32_t style) noexcept
	{
		uint32_t score = -1;
		VulkanContext* vk_ctx = new VulkanContext[1];
		
		// init vulkan instance
		// not show window
		// if failed , will exit this program
		init_vulkan_instance(vk_ctx, window_name.data());
		
		// select gpu
		// if not , will exit this program
		find_gpu(vk_ctx);

		// usually the nvidia's GPU is better than other
		// what is more, we select AMD GPU and Intel GPU
		// in the end NO:0 GPU will be selected
		for (size_t i = 0; i < vk_ctx->gpu_cout; i++)
		{
			if (strstr(vk_ctx->vk_gpu_properties[i].deviceName, "NVIDIA"))
			{
				vk_ctx->cur_used_gpu_index = i;
				break;
			}
			if (strstr(vk_ctx->vk_gpu_properties[i].deviceName, "AMD"))
			{
				vk_ctx->cur_used_gpu_index = i;
				score = 2;
			}
			if ((strstr(vk_ctx->vk_gpu_properties[i].deviceName, "INTEL")) && (score > 1))
			{
				vk_ctx->cur_used_gpu_index = i;
				score = 1;
			}
		}
		if (vk_ctx->cur_used_gpu_index == -1)
			vk_ctx->cur_used_gpu_index = 0;
		vk_ctx->cur_used_gpu = vk_ctx->gpu_list[vk_ctx->cur_used_gpu_index];

		WR_CLR_WRITE_LINE(std::format("Use the {0}", vk_ctx->vk_gpu_properties[vk_ctx->cur_used_gpu_index].deviceName).c_str());

#if defined(_WIN32)
		// windows string is utf16 format
		U16StringRef win_str_window_name = window_name;
		window_hwnd = create_windows_window(win_str_window_name, w, h, style);
		if(!window_hwnd)
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrWindow", "Create window failed!");
			return false;
		}
#else
#endif // window platform
		if(!get_vulkan_surface(vk_ctx->vk_main_instance, window_hwnd, pvk_allocator, vk_ctx->window_bitmap_surface))
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrWindow", "Create window surface failed!");
			return false;
		}
		if (!get_queue_family_indices(vk_ctx))
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrWindow", "Create queue failed!");
			return false;
		}
		if (!create_logic_device(vk_ctx))
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrWindow", "Create vulkan logic device failed!");
			return false;
		}
		vulkan_ctx = vk_ctx;
		return true;
	}

	recti Window::get_window_size()
	{
		recti size;
#if defined(_WIN32)
		size = get_windows_window_rect(window_hwnd);
#else
#endif // window platform
		return size;
	}

	bool Window::event()
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
		delete vk_ctx;
	}
} // namespace wr is end