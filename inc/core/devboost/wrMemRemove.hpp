#ifndef _WR_MEM_REMOVE_HPP_
#define _WR_MEM_REMOVE_HPP_

#include "type/wrOrdinals.hpp"

namespace wr
{
	void* memerase(byte_t* src, uint64_t src_size, uint64_t start, uint64_t ofs) noexcept;
} // namespace wr is end

#endif // !_WR_MEM_REMOVE_HPP_ IS EOF