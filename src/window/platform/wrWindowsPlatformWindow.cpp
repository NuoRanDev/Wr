// statement
#include <platform/wrWindowsPlatformWindow.hpp>

#if defined(_WIN32)
// os api
#include <dwmapi.h>
// vulkan
#include <vulkan/vulkan_win32.h>
// std
#include <cstdlib>
#include <format>
#include <iostream>
// core
#include <log/wrLogOutput.hpp>
#include <memory/wrAlloc.hpp>
#include <type/wrDataStruction.hpp>
#include <filesystem/wrPath.hpp>

namespace wr
{
	using WindowsWindowHandle = HWND;

	static HINSTANCE cur_hinst = nullptr;
	static WNDCLASSEX window_class = { 0 };
	static MSG msg = { 0 };
	static ATOM atom_state;

	static const wchar_t sub_key[]		= L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
	static const wchar_t value_name[]	= L"AppsUseLightTheme";

	static void set_window_border_sytle(HWND hWnd) noexcept
	{
		HKEY key;
		DWORD key_value = -1;
		DWORD key_value_size = sizeof(DWORD);
		DWORD attribute;
		BOOL value;

		if (RegOpenKeyEx(HKEY_CURRENT_USER, sub_key, 0, KEY_READ, &key) == ERROR_SUCCESS)
		{
			HRESULT hr = RegGetValue(key, nullptr, value_name, RRF_RT_REG_DWORD, nullptr, &key_value, &key_value_size);
			if (hr != S_OK)
			{
				key_value_size = -1;
			}
			RegCloseKey(key);
		}

		switch(key_value)
		{
		case 0:
			attribute = DWMWA_USE_IMMERSIVE_DARK_MODE;
			value = TRUE;
			DwmSetWindowAttribute(hWnd, attribute, &value, sizeof(value));
			return;
		case 1:
			attribute = DWMWA_USE_IMMERSIVE_DARK_MODE;
			value = FALSE;
			DwmSetWindowAttribute(hWnd, attribute, &value, sizeof(value));
			return;
		default:
			return;
		}
	}

	static dynamic_array<Path> drop_files(HWND hwnd, HDROP drop_info)
	 {
		 UINT  file_count = DragQueryFileW(drop_info, (UINT)-1, NULL, 0);
		 TCHAR file_name[_MAX_PATH];
		 dynamic_array<Path> out_data;
		 uint64_t char_count;

		// 获取拖拽进来文件和目录
		for (UINT i = 0; i < file_count; i++)
		{
			char_count = DragQueryFileW(drop_info, i, file_name, sizeof(file_name));
			U16StringRef u16_path;
			u16_path.load_utf16_by_count(file_name, char_count);
			out_data.push_back(Path(u16_path));
		}
		DragFinish(drop_info);
		return out_data;
	}

	static LRESULT CALLBACK window_proc_event(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept
	{
		switch (msg)
		{
		case WM_DISPLAYCHANGE:
			break;

		case WM_DEVICECHANGE:
			break;
		case WM_THEMECHANGED:
			std::cout << "Theme changed" << std::endl;
			set_window_border_sytle(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_DROPFILES:
			auto fs = drop_files(hwnd, reinterpret_cast<HDROP>(wparam));
			for (const auto& f : fs)
			{
				auto si = f.split();
				std::cout << si.first << " " << si.second << "\n";
				Path d = si.second;
				auto c = d.abs_path();
				std::cout << "abs " << c << "\n";
			}
			break;
		}
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}

	bool init_windows_env() noexcept
	{
		wchar_t out_proc_name[256];
		wchar_t* class_name;
		size_t i, out_str_number;

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

	any_type_ptr_t create_window(U16StringRef& window_name, int32_t w, int32_t h, uint32_t style) noexcept
	{
		WindowsWindowHandle window_inst;
		unsigned long win32_style = get_window_style(style);
		window_inst = CreateWindowExW(WS_EX_ACCEPTFILES, MAKEINTATOM(atom_state), window_name.data(),
			win32_style, CW_USEDEFAULT, CW_USEDEFAULT,
			w, h,
			NULL, NULL, cur_hinst, NULL);
		if (window_inst == NULL)
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrWindow::window API", std::format("Create windows fataled , ERROR CODE: {0}", GetLastError()).c_str());
			return nullptr;
		}
		set_window_border_sytle(window_inst);
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
	{}
} // namespace wr is end

#endif // defined(_WIN32) is end