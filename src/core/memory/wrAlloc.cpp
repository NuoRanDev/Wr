#include "memory/wrAlloc.hpp"

#include "mimalloc-override.h"

namespace wr
{
	any_type_ptr_t wr_only_malloc(size_t byte_size) noexcept
	{
		return mi_malloc(byte_size);
	}

	any_type_ptr_t wr_only_realloc(any_type_ptr_t src, size_t byte_size) noexcept
	{
		return mi_realloc(src, byte_size);
	}

	any_type_ptr_t wr_free(any_type_ptr_t src) noexcept
	{
		if (src != nullptr)mi_free(src);
		return nullptr;
	}

	any_type_ptr_t wr_olny_aligned_alloc(size_t alignment,size_t byte_size) noexcept
	{
		return mi_aligned_alloc(alignment, byte_size);
	}

	any_type_ptr_t wr_olny_aligned_realloc(any_type_ptr_t src, size_t alignment, size_t byte_size) noexcept
	{
		return mi_realloc_aligned(src, alignment, byte_size);
	}
}