// statement
#include <devboost/wrMemFind.hpp>
// core
#include <devboost/wrMemCmp.hpp>

namespace wr
{
	int64_t memfind_start(const byte_t* child, const byte_t* main, const int64_t child_byte_size, const int64_t main_area)
	{
		for (int64_t i = 0; i < main_area - child_byte_size; i++)
		{
			if (long_memory_cmp(main + i, child, child_byte_size))
			{
				return i;
			}
		}
		return -1;
	}
	int64_t memfind_last(const byte_t* child, const byte_t* main, const int64_t child_byte_size, const int64_t main_area)
	{
		for (int64_t i = main_area - child_byte_size; i >= 0; i--)
		{
			if (long_memory_cmp(main + i, child, child_byte_size))
			{
				return i;
			}
		}
		return -1;
	}
} // namespace wr is end