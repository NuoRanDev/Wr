#ifndef _WR_MEM_CMP_HPP_
#define _WR_MEM_CMP_HPP_
// std
#include <cstdint>
// core
#include <type/wrOrdinals.hpp>
#include <type/wrDataStruction.hpp>

namespace wr
{
	bool short_memory_cmp(const byte_t* cmp1, const byte_t* cmp2, const uint64_t cmp_size) noexcept;

	bool long_memory_cmp(const byte_t* cmp1, const byte_t* cmp2, const uint64_t cmp_size) noexcept;

	dynamic_array<int64_t> find_byte_all_memory_int64size(const byte_t* str, const uint64_t size, const byte_t c) noexcept;
} // namespace wr is end

#endif // _WR_MEM_CMP_HPP_ IS EOF