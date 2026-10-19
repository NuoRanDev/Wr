#ifndef _WR_STRING_HPP_
#define _WR_STRING_HPP_

// core
#include <core/string/wrStringRef.hpp>
#if !defined(DISABLED_UTF16_STRING) || defined(_WIN32)
#include <core/string/wrU16String.hpp>
#endif // DISABLED_UTF16_STRING

namespace wr
{
	using wr_string = u8_string_ref;
} // namespace wr is end

#endif // _WR_STRING_HPP_ IS EOF