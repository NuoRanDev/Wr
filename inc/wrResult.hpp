#ifndef _INC_WR_RESULT_HPP_
#define _INC_WR_RESULT_HPP_

#include <wrType.hpp>

namespace wr
{
	// Result
	enum class WR_RESULT :uint32_t
	{
		WR_OK = 0,
		WR_WARNING = 1,
		WR_SYSTEM_CALL_ERROR = 3,
		WR_APP_CALL_ERROR = 4,
		WR_ERROR = UINT32_MAX,
		WR_EXIT = INT32_MAX,
	};
} // namespace wr is end

#endif // _INC_WR_RESULT_HPP_ IS EOF