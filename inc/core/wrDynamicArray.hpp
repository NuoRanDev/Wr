#ifndef _WR_DYNAMIC_ARRAY_HPP_
#define _WR_DYNAMIC_ARRAY_HPP_

// base
#include <wrType.hpp>
// core
#include <core/dynamic_array/wrPodDynamicArray.hpp>

namespace wr
{
	template<typename T> 
	class dynamic_array
	{
	public:
		// create empty 
		dynamic_array() noexcept { format_data = pod_create(sizeof(T)); }

		// create
		dynamic_array(size_t _num) noexcept { format_data = pod_create(sizeof(T), _num); }

		// move
		dynamic_array(dynamic_array<T>&& temp) noexcept
		{
			format_data = temp.format_data;
			temp.format_data.pitems = nullptr;
		}

		// by form(*)
		dynamic_array(const T* ptr, size_t number) noexcept { form(ptr, number); }

		// load data form external ptr
		void form(const T* ptr, size_t _num) noexcept
		{
			pod_form(format_data, ptr, _num, sizeof(T));
		}

		// load C abi input data
		void load(pod_dynamic_array format_input) noexcept { format_data = format_input; }

		// delete end
		void pop_back() noexcept { format_data = pod_pop_back(format_data); }

		// adjusts the size of a dynamic_array, adding or removing elements as needed,
		// optionally initializing new elements with a specified value.
		void resize(size_t _num) noexcept
		{
			pod_resize(format_data, _num);
		}

		// capacity is the number of elements the dynamic_array can hold without reallocating memory
		void capacity(size_t _num) noexcept
		{
			pod_capacity(format_data, _num);
		}

		// is empty?
		bool empty()const noexcept { return format_data.item_num == 0; }

		//
		void push_back(const dynamic_array<T>& darr) noexcept
		{
			return push_back(darr.const_data(), darr.item_size());
		}

		//
		void push_back(const T* ptr, size_t _num) noexcept
		{
			format_data = pod_push_back(format_data, ptr, _num);
		}

		//
		void push_back(T item) noexcept
		{
			format_data = pod_push_back(format_data, &item, 1);
		}

		T at(size_t num) noexcept
		{
			return reinterpret_cast<T*>(format_data.pitems)[num];
		}

		// get item number
		constexpr size_t num() const noexcept { return format_data.item_num; }

		// get item size
		constexpr size_t item_size() const noexcept { return format_data.item_size; }

		// get unsafe item data
		T* unsafe_data() noexcept { return reinterpret_cast<T*>(format_data.pitems); }

		// get unsafe item data
		const T* const_data() const noexcept { return reinterpret_cast<const T*>(format_data.pitems); }

		// move pod
		pod_dynamic_array move_pod() noexcept
		{
			pod_dynamic_array output = format_data;
			format_data.pitems = nullptr;
			return output;
		}

		const pod_dynamic_array get_pod() const noexcept { return format_data; }

		// full 0 in format_data
		void release()
		{
			format_data = pod_release(format_data);
		}

		// destory
		~dynamic_array() { release(); }

	private:

		pod_dynamic_array format_data;
	};
} // namespace wr is end

#endif // _WR_DYNAMIC_ARRAY_HPP_ IS END