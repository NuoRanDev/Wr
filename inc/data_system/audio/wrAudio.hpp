#ifndef _INC_WR_AUDIO_HPP_
#define _INC_WR_AUDIO_HPP_

#include "type/wrOrdinals.hpp"

#include <functional>

namespace wr
{
	enum class PCM_SAMPLE_BIT_SIZE : uint16_t
	{
		L8 = 8, L16 = 16, L24 = 24, L32 = 32, L64 = 64
	};

	enum PCM_SAMPLE_TYPE : uint16_t
	{
		U = 0, S = 255, F = 65535
	};

	constexpr size_t get_pcm_byte_size(PCM_SAMPLE_BIT_SIZE format) { return static_cast<uint16_t>(format) / 8; }

	struct PcmHeader
	{
		PCM_SAMPLE_BIT_SIZE sample_bit_size;
		PCM_SAMPLE_TYPE sample_type;

		uint32_t sample_rate;
		uint32_t channels = 1;
	};

	struct PcmBlock
	{
		PcmHeader header;

		uint64_t data_size;
		byte_t* pcm_data;
	};

} // namespace wr is end

#endif // _INC_WR_AUDIO_HPP_ IS EOF