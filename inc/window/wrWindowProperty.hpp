#ifndef _INC_WR_WINDOW_PROPERTY_HPP_
#define _INC_WR_WINDOW_PROPERTY_HPP_

// core
#include <type/wrOrdinals.hpp>

namespace wr
{
	namespace WindowStyle
	{
		constexpr uint32_t None = 0;			//!< No border / title bar (this flag and all others are mutually exclusive)
		constexpr uint32_t Titlebar = 1 << 0;	//!< Title bar + fixed border
		constexpr uint32_t Resize = 1 << 1;		//!< Title bar + resizable border + maximize button
		constexpr uint32_t Close = 1 << 2;		//!< Title bar + close button

		constexpr uint32_t Default = Titlebar | Resize | Close; //!< Default window style
	}

	enum class State
	{
		Windowed,  //!< Floating window
		Fullscreen //!< Fullscreen window
	};

} // namespace wr is end

#endif // _INC_WR_WINDOW_PROPERTY_HPP_ IS EOF