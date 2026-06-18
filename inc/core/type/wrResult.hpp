#ifndef _INC_WR_RESULT_HPP_
#define _INC_WR_RESULT_HPP_

#include <type/wrOrdinals.hpp>

namespace wr
{
	// Result
	enum ResultInfo :uint32_t
	{
		WR_OK = 0,
		WR_WARNING,
		WR_ERROR = UINT32_MAX,
		WR_EXIT = INT32_MAX,
	};
} // namespace wr is end

#endif // _INC_WR_RESULT_HPP_ IS EOF