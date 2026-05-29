#ifndef _WR_BIG_NUMBER_HPP_
#define _WR_BIG_NUMBER_HPP_
// core
#include <type/wrOrdinals.hpp>
#include <type/wrDataStruction.hpp>
// std
#include <cstring>

namespace wr
{
	constexpr uint64_t WR_UINT64_MASK = 1llu << 63; // 2^63
	constexpr uint64_t WR_UINT32_MASK = 1llu << 31; // 2^63

	class BigNumber
	{
	public:
		BigNumber() = default;

		bool operator== (const BigNumber & other) const
		{
			if (other.number_data.size() != number_data.size())
				return false;
			if (other.is_negative != is_negative)
				return false;
			return memcmp(other.number_data.data(), number_data.data(), other.number_data.size());
		}

		bool operator< (const BigNumber& other) const
		{
			if (other.number_data.size() < number_data.size())
				return true;
			if (other.is_negative == true || is_negative == false)
				return true;
			for (size_t i = 0; i < number_data.size(); i++)
			{
				if (number_data[i] < other.number_data[i])
					return true;
			}
			return false;
		}

		BigNumber operator+(const BigNumber& other)
		{
			BigNumber result;
			size_t max_size = std::max(number_data.size(), other.number_data.size());
			int64_t carry = 0;
			
			if (other.is_negative == other.is_negative)
			{
				result.is_negative = is_negative;
				goto ADD_SUM;
			}

			result.number_data.resize(max_size + 1, 0);
			
			ADD_SUM:
			for (size_t i = 0; i < max_size; i++)
			{
				carry = (int64_t)(((other.number_data[i] >> 1 + number_data[i] >> 1) +
					(((other.number_data[i] ^ 1) + (number_data[i] ^ 1) + carry) ^ 1)) & WR_UINT64_MASK);
				result.number_data[i] = other.number_data[i] & WR_UINT64_MASK + number_data[i] & WR_UINT64_MASK + carry;
			}
			if (carry)
			{
				result.number_data[max_size] = carry;
			}
			_SUM:
			return result;
		}

		~BigNumber()
		{
		}

		int64_t is_negative;

		dynamic_array<uint64_t> number_data;
	};

}// namespace wr is end

#endif // _WR_BIG_NUMBER_HPP_ IS EOF