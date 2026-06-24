#ifndef _INC_WR_MATH_VECTOR_HPP_
#define _INC_WR_MATH_VECTOR_HPP_

// core
#include <type/wrTemplate.hpp>
// std
#include <type_traits>

namespace wr 
{
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
	using vec3u = vec3<uint32_t>;

	template<baise_type_no_struct_or_class T> struct vec2
	{
		T x, y;
	};

	template<baise_type_no_struct_or_class T> vec2<T> operator+(vec2<T>a, vec2<T>b)
	{
		return vec2<T>{ a.x + b.x, a.y + b.y};
	}

	template<baise_type_no_struct_or_class T> vec2<T> operator-(vec2<T>a, vec2<T>b)
	{
		return vec2<T>{ a.x - b.x, a.y - b.y};
	}

	template<baise_type_no_struct_or_class T> bool operator == (vec2<T>a, vec2<T>b)
	{
		return (a.x == b.x) && (a.y == b.y);
	}

	using vec2f = vec2<float>;
	using vec2d = vec2<double>;
	using vec2i = vec2<int32_t>;
	using vec2u = vec2<uint32_t>;

	template<baise_type_no_struct_or_class T> struct rect
	{
		T left, top, right, bottom;
	};

	using rectf = rect<float>;
	using rectd = rect<double>;
	using recti = rect<int32_t>;
	using rectu = rect<uint32_t>;

} // namespace wr is end

#endif // !_INC_WR_MATH_VECTOR_HPP_