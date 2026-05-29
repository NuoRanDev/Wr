#ifndef _INC_WR_MATH_VECTOR_HPP_
#define _INC_WR_MATH_VECTOR_HPP_

// std
#include <type_traits>

namespace wr 
{
	template <typename T> concept baise_type_no_struct_or_class =
		std::is_same<T, float>::value || std::is_same<T, double>::value ||
		std::is_same<T, int8_t>::value || std::is_same<T, uint8_t>::value ||
		std::is_same<T, int16_t>::value || std::is_same<T, uint16_t>::value ||
		std::is_same<T, int32_t>::value || std::is_same<T, uint32_t>::value ||
		std::is_same<T, int64_t>::value || std::is_same<T, uint64_t>::value;


	template<baise_type_no_struct_or_class T> struct vec3
	{
		T x, y, z;
	};

	template<baise_type_no_struct_or_class T> vec3<T> operator+(vec3<T>a, vec3<T>b)
	{
		return vec3<T>{ a.x + b.x, a.y + b.y, a.z + b.z};
	}

	template<baise_type_no_struct_or_class T> vec3<T> operator-(vec3<T>a, vec3<T>b)
	{
		return vec3<T>{ a.x - b.x, a.y - b.y, a.z - b.z};
	}

	template<baise_type_no_struct_or_class T> bool operator == (vec3<T>a, vec3<T>b)
	{
		return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
	}

	using vec3f = vec3<float>;
	using vec3d = vec3<double>;
	using vec3i = vec3<int32_t>;

} // namespace wr is end

#endif // !_INC_WR_MATH_VECTOR_HPP_