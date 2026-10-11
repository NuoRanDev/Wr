#ifndef _WR_U8STRING_REF_HPP_
#define _WR_U8STRING_REF_HPP_

// base
#include <wrType.hpp>
#include <wrCompiler.hpp>
// core
#include <core/string/wrU8String.hpp>

namespace wr
{
	class u8_string_ref
	{
	public:
		u8_string_ref();
	private:
		u8_string str_data;
	};

} // namespace wr is end

#endif // _WR_U8STRING_REF_HPP_ IS EOF