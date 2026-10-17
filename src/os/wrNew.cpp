// statement
#include <os/wrNew.hpp>
// os
#include <os/wrAlloc.hpp>
/*
void* operator new(size_t size)
{
	return wr::wr_malloc<wr::byte_t>(size);
}

void* operator new[](size_t size)
{
	return wr::wr_malloc<wr::byte_t>(size);
}

void operator delete[](void* ptr) noexcept
{
	wr::wr_check_null_free(ptr);
}
*/