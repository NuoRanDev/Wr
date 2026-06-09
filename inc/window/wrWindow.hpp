#ifndef _INC_WR_WINDOW_HPP_
#define _INC_WR_WINDOW_HPP_
// core
#include <string/wrString.hpp>
#include <math/wrMathVector.hpp>
// std
#include <any>
// window
#include <wrWindowProperty.hpp>

namespace wr
{
	bool init_wr_window_ctx();

	class Window
	{
	public:

		Window() = default;

		bool create_window(String& window_name, int32_t w, int32_t h, uint32_t style = WindowStyle::Default) noexcept;

		recti get_window_size();

		bool event();

		~Window();

	private:

		any_type_ptr_t vulkan_ctx;
		any_type_ptr_t window_hwnd;
	};
} // namespace wr is end

#endif // _INC_WR_WINDOW_HPP_ IS EOF