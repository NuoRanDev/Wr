#ifndef _WR_DYNAMIC_ARRAY_HPP_
#define _WR_DYNAMIC_ARRAY_HPP_

// base
#include <wrType.hpp>
// core
#include <core/dynamic_array/wrPodDynamicArray.hpp>
// std
#include <memory>

namespace wr
{
	constexpr size_t bast_num(size_t item_size)
	{
		if (item_size > 8)
			return 2;
		if (item_size > 4)
			return 4;
		if (item_size > 2)
			return 8;
		else
		{
			return 16;
		}
	}

	template<class T> 
	class dynamic_array
	{
	public:
		// create empty 
		dynamic_array() noexcept 
		{
			format_data = pod_dynamic_void_array_create(sizeof(T), bast_num(sizeof(T)));
		}

		// create
		dynamic_array(size_t _num) noexcept 
		{
			format_data = pod_dynamic_void_array_create(sizeof(T), _num);
		}

		// get
		dynamic_array(pod_dynamic_array data) noexcept { format_data = data; }

		// move
		dynamic_array(dynamic_array<T>&& temp) noexcept
		{
			format_data = temp.format_data;
			temp.format_data.pitems = nullptr;
		}

		// by form(*)
		dynamic_array(const T* ptr, size_t number) noexcept { form(ptr, number); }

		// clear all data
		// load data form external ptr
		void form(T* ptr, size_t _num) noexcept
		{
			if (format_data.item_num != 0) [[unlikely]]
				release();
			format_data = pod_dynamic_empty_array_create(sizeof(T), _num);
			T* item_data = reinterpret_cast<T*>(format_data.pitems);

			for (size_t i = 0; i < _num; i++)
			{
				std::construct_at(item_data + i, ptr[i]);
			}
		}

		// load C abi input data
		void load(pod_dynamic_array format_input) noexcept { format_data = format_input; }

		// delete end
		void pop_back() noexcept 
		{
			if (format_data.item_num > 0) [[likely]]
				std::destroy_at((reinterpret_cast<T*>(format_data)) + (format_data.item_num - 1));
			format_data = pod_dynamic_array_pop_back(format_data);
		}

		// adjusts the size of a dynamic_array, adding or removing elements as needed,
		// optionally initializing new elements with a specified value.
		void resize(size_t _num) noexcept
		{
			destory_over_flow(_num);
			pod_dynamic_array_resize(format_data, _num);
		}

		// capacity is the number of elements the dynamic_array can hold without reallocating memory
		void capacity(size_t _num) noexcept
		{
			destory_over_flow(_num);
			pod_dynamic_array_capacity(format_data, _num);
		}

		// empty dynamic_array return true
		bool empty()const noexcept { return format_data.item_num == 0; }

		// push some item which is class-dynamic_array in dynamic_array end
		void push_back(const dynamic_array<T>& darr) noexcept
		{
			push_back(darr.const_data(), darr.item_size());
		}

		// push some item which is pointer in dynamic_array end
		void push_back(const T* ptr, size_t _num) noexcept
		{
			size_t start_ofs = format_data.item_num;
			format_data = pod_dynamic_array_push_back(format_data, ptr, _num);
			T* item_data = reinterpret_cast<T*>(format_data.pitems);
			for (size_t i = 0; i < _num; i++)
			{
				std::construct_at(item_data + (start_ofs + i), ptr[i]);
			}
		}

		// push a item in dynamic_array end
		void push_back(T item) noexcept
		{
			push_back(&item, 1);
		}

		T at(size_t num) noexcept
		{
			return (reinterpret_cast<T*>(format_data.pitems))[num];
		}

		// get item number
		constexpr size_t num() const noexcept { return format_data.item_num; }

		// get item size
		constexpr size_t item_size() const noexcept { return format_data.item_size; }

		// get unsafe item data
		T* mut_data() noexcept { return reinterpret_cast<T*>(format_data.pitems); }

		// get unsafe item data
		const T* const_data() const noexcept { return reinterpret_cast<const T*>(format_data.pitems); }

		// move c api struct to other progream code c style struction
		pod_dynamic_array move_pod() noexcept
		{
			pod_dynamic_array output = format_data;
			format_data.pitems = nullptr;
			return output;
		}

		// return c api struct to other progream code c abi function
		const pod_dynamic_array get_pod() const noexcept { return format_data; }

		// full 0 in format_data
		void release()
		{
			T* item_data = reinterpret_cast<T*>(format_data.pitems);
			for (size_t i = 0; i < format_data.item_num; i++)
			{
				std::destroy_at(item_data + i);
			}
			format_data = pod_dynamic_array_release(format_data);
		}

		// destory
		~dynamic_array() { release(); }

	private:

		void destory_over_flow(size_t _num)
		{
			if (_num < format_data.item_num)
			{
				T* item_data = reinterpret_cast<T*>(format_data.pitems);
				for (size_t i = _num; i < format_data.item_num; i++)
				{
					std::destroy_at(item_data + i); std::destroy_at();
				}
			}
		}

		pod_dynamic_array format_data;
	};
} // namespace wr is end

#endif // _WR_DYNAMIC_ARRAY_HPP_ IS END