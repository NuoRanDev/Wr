#include "log/wrLogOutput.hpp"
#include "type/wrOrdinals.hpp"

#include "file/audio/wrReadAudioFile.hpp"
#include "memory/wrAlloc.hpp"

#include <type_traits>
#include <format>

#define DRFLAC_IMPLEMENTATION
extern"C" 
{
#include "dr_flac.h"
}

namespace wr
{
	static void* dr_malloc(size_t sz, void* _none) noexcept
	{
		return wr_malloc<byte_t>(sz);
	}

	static void dr_free(void* p, void* _none) noexcept
	{
		wr_free(p);
	}

	static void* dr_realloc(void* p, size_t sz, void* _none) noexcept
	{
		return wr_realloc<byte_t>(reinterpret_cast<byte_t*>(p), sz);
	}

	static drflac_allocation_callbacks dr_flac_alloc_cb_struct =
	{
		.pUserData = nullptr,
		.onMalloc = dr_malloc,
		.onRealloc = dr_realloc,
		.onFree = dr_free,
	};

	template<typename T> 
	byte_t* type_drflac_full_read(drflac* flac_context, uint64_t& size,
		drflac_uint64(*drflac_read_pcm_frames_type_extension)(drflac*, drflac_uint64, T*))
	{
		uint64_t block_pcm_frame_number =  ((flac_context->maxBlockSizeInPCMFrames > 0) ? flac_context->maxBlockSizeInPCMFrames : 4096);
		//uint64_t block_pcm_frame_number = flac_context->totalPCMFrameCount;
		uint64_t need_byte_size = flac_context->totalPCMFrameCount * sizeof(T);

		WR_INFO_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrAudio :DecFlac", std::format("size: {0} MB", need_byte_size / 1024 / 1024 ).c_str());
		
		T* out_data = reinterpret_cast<T*>(wr_malloc<byte_t>(need_byte_size));
		T* cur_data = out_data;
		uint64_t all_read_size;

		for (all_read_size = 0; all_read_size < flac_context->totalPCMFrameCount;)
		{
			auto read_size = drflac_read_pcm_frames_type_extension(flac_context, block_pcm_frame_number, cur_data);
			if (read_size < 0)
			{
				WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrAudio :DecFlac", "File is broken!");
				drflac_close(flac_context);
				return reinterpret_cast<byte_t*>(cur_data);
			}
			//put_float(reinterpret_cast<float*>(cur_data), block_pcm_frame_number);
			cur_data += read_size;
			all_read_size += read_size;
		}
		size = need_byte_size;

		drflac_close(flac_context);
		return reinterpret_cast<byte_t*>(out_data);
	}

	drflac* read_memory_flac_audio_info(size_t flac_size, const byte_t* flac_data, PcmHeader& pcmh)
	{
		drflac* flac_context = drflac_open_memory(flac_data, flac_size, &dr_flac_alloc_cb_struct);
		if (flac_context == nullptr)
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrAudio :DecFlac", "Can't read memory file!");
			return nullptr;
		}
		pcmh.channels = flac_context->channels;
		pcmh.sample_rate = flac_context->sampleRate;
		pcmh.sample_type = U;
		return flac_context;
	}

	bool read_memory_flac_audio_all_pcm(const AudioFile& file, PcmBlock& pcm_out) noexcept
	{
		drflac* flac_context;
		size_t flac_size = 0;
		const byte_t* flac_data = file.get_file_data(flac_size);

		if (flac_size == 0 || flac_data == nullptr)
		{
			WR_WARNING_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrAudio: DecFlac", "Can't read memory file!");
			return false;
		};

		flac_context = read_memory_flac_audio_info(flac_size, flac_data, pcm_out.header);
		if (flac_context == nullptr)
			return false;

		switch(flac_context->bitsPerSample)
		{
		case 8:
		case 16:
			pcm_out.header.sample_bit_size = PCM_SAMPLE_BIT_SIZE::L16;
			pcm_out.header.sample_type = S;
			goto INT16_PCM_DEC;
			break;

		case 24:
		case 32:
			pcm_out.header.sample_bit_size = PCM_SAMPLE_BIT_SIZE::L32;
			pcm_out.header.sample_type = F;
			goto INT32_PCM_DEC;
			break;

		default:
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrAudio :DecFlac", "format not support!");
			return false;
		}

	INT16_PCM_DEC:
		//drflac_open_and_read_pcm_frames_f32();
		pcm_out.pcm_data = type_drflac_full_read<drflac_int16>(flac_context, pcm_out.data_size, &(drflac_read_pcm_frames_s16));
		if (pcm_out.pcm_data == nullptr)
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrAudio :DecFlac", "Decode is failed!");
			return false;
		}
		return true;
	INT32_PCM_DEC:

		pcm_out.pcm_data = type_drflac_full_read<float>(flac_context, pcm_out.data_size, &(drflac_read_pcm_frames_f32));
		if (pcm_out.pcm_data == nullptr)
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrAudio :DecFlac", "Decode is failed!");
			return false;
		}
		return true;
	}
} // namespace wr is end