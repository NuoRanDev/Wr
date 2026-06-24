#ifndef _INC_WR_TEMPLATE_HPP_
#define _INC_WR_TEMPLATE_HPP_

namespace wr
{
	template <typename T> concept baise_type_no_struct_or_class =
		std::is_same<T, float>::value || std::is_same<T, double>::value ||
		std::is_same<T, int8_t>::value || std::is_same<T, uint8_t>::value ||
		std::is_same<T, int16_t>::value || std::is_same<T, uint16_t>::value ||
		std::is_same<T, int32_t>::value || std::is_same<T, uint32_t>::value ||
		std::is_same<T, int64_t>::value || std::is_same<T, uint64_t>::value;

	template <typename T> concept Integer =
		std::is_same<T, int8_t>::value  || std::is_same<T, uint8_t>::value ||
		std::is_same<T, int16_t>::value || std::is_same<T, uint16_t>::value ||
		std::is_same<T, int32_t>::value || std::is_same<T, uint32_t>::value ||
		std::is_same<T, int64_t>::value || std::is_same<T, uint64_t>::value;
} // namespace wr is end

#endif // _INC_WR_TEMPLATE_HPP_ IS EOF