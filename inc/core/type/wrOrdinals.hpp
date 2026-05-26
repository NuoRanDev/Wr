#ifndef _WR_ORDINALA_HPP_
#define _WR_ORDINALA_HPP_

#include <cstdint>
#include <type_traits>

namespace wr
{
	using unicode_t = char32_t;
	using byte_t = uint8_t;

	using utf8_t = char8_t;
	using utf16le_t = wchar_t;

	using any_type_ptr_t = void*;

	template<typename T>
	concept IntegralType = requires(T)
	{
		std::is_integral<T>::value;
	};
} // namespace wr is end

#endif // _WR_ORDINALA_HPP_ IS END