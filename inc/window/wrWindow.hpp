#ifndef _INC_WR_WINDOW_HPP_
#define _INC_WR_WINDOW_HPP_
// core
#include <string/wrString.hpp>

namespace wr
{
	namespace WindowStyle
	{
#if defined(_WIN32)
		unsigned long ACCEPTFILES = 0x00000010L;
		unsigned long TOOLWINDOW = 0x00000080L;
		unsigned long CLIENTEDGE = 0x00000200L;
#elif defined(wayland)

#endif // defined(_WIN32)
	}

	class Window
	{
	public:

		enum class Style
		{

		};

		Window() = default;

		bool create_window(String& window_name)
		{}

		~Window();

	private:

	};
} // namespace wr is end

#endif // _INC_WR_WINDOW_HPP_ IS EOF