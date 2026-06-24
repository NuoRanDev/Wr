#ifndef _WR_BIG_NUMBER_HPP_
#define _WR_BIG_NUMBER_HPP_
// core
#include <type/wrOrdinals.hpp>
#include <type/wrDataStruction.hpp>
#include <type/wrTemplate.hpp>
// std
#include <cstring>
#include <type_traits>
#include <bit>

namespace wr
{

	class BigInteger

	{
	public:
		BigInteger() noexcept
		{
			bit_count = 0;
			is_negative = false;
		}

		template<Integer T> BigInteger(T x) noexcept { load(x); }

		BigInteger(BigInteger&& src)
		{
			this->bit_count = src.bit_count;
			src.bit_count = 0;
			this->is_negative = src.is_negative;
			this->number_data.swap(src.number_data);
		}
		
		template<Integer T> void load(T x) noexcept
		{
			using UT = std::make_unsigned_t<T>;
			bit_count = static_cast<uint64_t>(std::numeric_limits<UT>::digits - std::countl_zero(UT(x))) - 1;
			if (x < 0) { is_negative = true; }
			else { is_negative = false; }
		}

		BigInteger operator+(const BigInteger& i1)
		{
			uint64_t temp = 0;
		}

		BigInteger operator-(const BigInteger& i1);

		BigInteger operator*(const BigInteger& i1);

		BigInteger operator/(const BigInteger& i1);

		uint64_t log2i() const noexcept { return bit_count; }

	private:

		friend class BigInteger;

		uint64_t bit_count;

		bool is_negative;

		dynamic_array<uint32_t> number_data;
	};

}// namespace wr is end

#endif // _WR_BIG_NUMBER_HPP_ IS EOF