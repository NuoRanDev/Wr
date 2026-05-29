#ifndef _INC_WR_DATA_STRUCTION_HPP_
#define _INC_WR_DATA_STRUCTION_HPP_

// statement
#include <type/wrOrdinals.hpp>
// std
#include <vector>
#include <map>
// 3rd lib mimalloc
#if defined(USE_MIMALLOC)
#include <mimalloc-override.h>
#endif // USE_MIMALLOC

namespace wr
{
#if defined(USE_MIMALLOC)
	template<class struction1> using dynamic_array = std::vector<struction1, mi_stl_allocator<struction1>>;

	template<class struction1, class struction2> using fast_map = std::map<struction1, struction2, mi_stl_allocator<std::pair<struction1, struction2>>>;
#else
	template<class struction1> using dynamic_array = std::vector<struction1>;

	template<class struction1, class struction2> using fast_map = std::map<struction1, struction2>;
#endif // USE_MIMALLOC

} // namespace wr is end

#endif // _INC_WR_DATA_STRUCTION_HPP_ IS EOF