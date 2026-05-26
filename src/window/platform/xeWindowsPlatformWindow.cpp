#include "platform/xeWindowsPlatformWindow.hpp"

#if defined(_WIN32)
// OS
#include <Windows.h>
//
#include <vulkan/vulkan_win32.h>
//
#include <cstdlib>
#include <format>
//
#include "log/wrLogOutput.hpp"
#include "memory/wrAlloc.hpp"

namespace wr
{
	using WindowsWindowHandle = HWND;

	static HINSTANCE cur_hinst = nullptr;
	static WNDCLASSEX window_class = { 0 };
	static MSG msg = { 0 };
	static ATOM atom_state;

	static LRESULT CALLBACK window_proc_event(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
	{
		switch (uMsg)
		{
		case WM_DISPLAYCHANGE:
			break;

		case WM_DEVICECHANGE:
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}

	bool init_windows_env() noexcept
	{
		wchar_t out_proc_name[256];
		wchar_t* class_name;
		size_t i, copy_number, out_str_number;
		
		out_str_number = GetModuleFileNameW(NULL, out_proc_name, 256);
		
		for (i = out_str_number - 1; i >= 0; i--)
		{
			if (out_proc_name[i] == L'\\')
			{
				i++;
				break;
			}
		}
		class_name = (reinterpret_cast<wchar_t*>(&out_proc_name)) + i;

		cur_hinst = GetModuleHandleW(NULL);
		window_class.cbSize = sizeof(WNDCLASSEX);
		window_class.style = CS_HREDRAW | CS_VREDRAW,
		window_class.lpfnWndProc = &window_proc_event;
		window_class.hInstance = cur_hinst;
		window_class.lpszClassName = class_name;
		
		atom_state = RegisterClassExW(&window_class);
		if (atom_state == 0)
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrWindow", std::format("Create windows class fataled , ERROR CODE: {0}", GetLastError()).c_str());
			return false;
		}

		return true;
	}

	any_type_ptr_t create_window(U16StringRef& window_name, int32_t w, int32_t h) noexcept
	{
		WindowsWindowHandle window_inst;
		window_inst = CreateWindowExW(0, MAKEINTATOM(atom_state), window_name.data(),
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			w, h,
			NULL, NULL, cur_hinst, NULL);
		if (window_inst == NULL)
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrWindow::window API", std::format("Create windows fataled , ERROR CODE: {0}", GetLastError()).c_str());
			return nullptr;
		}
		if (ShowWindow(window_inst, SW_SHOWNORMAL))
		{
			WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrWindow:: window API", std::format("Show windows fataled , ERROR CODE: {0}", GetLastError()).c_str());
		}
		return window_inst;
	}

	bool switch_enevt() noexcept
	{
		BOOL state = GetMessageW(&msg, NULL, 0, 0);
		if (state == FALSE)
			return false;
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
		return true;
	}

	bool get_vulkan_surface(VkInstance vk_inst, any_type_ptr_t window_ptr, const VkAllocationCallbacks* cb_allocer, VkSurfaceKHR& out_surface) noexcept
	{
		HWND window_inst = reinterpret_cast<HWND>(window_ptr);
		VkWin32SurfaceCreateInfoKHR win32surface_create_info = {};
		win32surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		win32surface_create_info.hwnd = window_inst;
		win32surface_create_info.hinstance = cur_hinst;

		if (vkCreateWin32SurfaceKHR(vk_inst, &win32surface_create_info, cb_allocer, &out_surface) != VK_SUCCESS)
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrWindow::(window API , VULKAN)", "Create win32 vulkan surface fataled");
			return false;
		}
		return true;
	}

	void free()
	{
	}
}

#endif // defined(_WIN32) is end