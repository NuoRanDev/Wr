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
	ResultInfo init_wr_window_ctx();

	class Window
	{
	public:

		Window() = default;

		ResultInfo create_window(String& window_name, vec2u size, uint32_t style = WindowStyle::Default) noexcept;

		rectu get_window_size();

		ResultInfo event();

		~Window();

	private:

		any_type_ptr_t vulkan_ctx;
		any_type_ptr_t window_hwnd;
	};
} // namespace wr is end

#endif // _INC_WR_WINDOW_HPP_ IS EOF