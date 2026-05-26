#ifndef _WR_ALLOC_HPP_
#define _WR_ALLOC_HPP_

#define USE_MIMALLOC

#include "type/wrOrdinals.hpp"

//#include "mimalloc-new-delete.h"

namespace wr
{
	any_type_ptr_t wr_only_malloc(size_t byte_size) noexcept;

	any_type_ptr_t wr_only_realloc(any_type_ptr_t src, size_t byte_size) noexcept;

	any_type_ptr_t wr_olny_aligned_alloc(size_t alignment, size_t byte_size) noexcept;

	any_type_ptr_t wr_olny_aligned_realloc(any_type_ptr_t src, size_t alignment, size_t byte_size) noexcept;

	// malloc
	template<typename T> T* wr_malloc(size_t size) noexcept
	{
		return (T*)wr_only_malloc(sizeof(T) * size);
	}

	template<typename T> T* wr_realloc(any_type_ptr_t src, size_t size) noexcept
	{
		return (T*)wr_only_realloc(src, sizeof(T) * size);
	}

	any_type_ptr_t wr_free(any_type_ptr_t src) noexcept;

	template<typename T> void wr_delete_array(T* src) noexcept
	{
		if (src != nullptr)
		{
			delete[] src;
			src = nullptr;
		}
	}

	template<typename T> void wr_delete(T* src) noexcept
	{
		if (src != nullptr)
		{
			delete src;
			src = nullptr;
		}
	}
} // namespace wr is end

#endif // _WR_ALLOC_HPP_ IS EOF