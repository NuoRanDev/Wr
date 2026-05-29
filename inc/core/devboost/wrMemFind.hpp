#ifndef _INC_WR_MEM_FIND_HPP_
#define _INC_WR_MEM_FIND_HPP_

// core
#include <type/wrOrdinals.hpp>

namespace wr
{
	int64_t memfind_start(const byte_t* child, const byte_t* main, const int64_t child_byte_size, const int64_t main_area);

	int64_t memfind_last(const byte_t* child, const byte_t* main, const int64_t child_byte_size, const int64_t main_area);
} // namespace wr is end

#endif // _INC_WR_MEM_FIND_HPP_ IS EOF