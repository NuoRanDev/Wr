#ifndef _WR_RWAD_AUDIO_HPP_
#define _WR_RWAD_AUDIO_HPP_

// core
#include <type/wrOrdinals.hpp>
// data system
#include <audio/wrAudio.hpp>
#include <file/audio/wrAudioFile.hpp>

namespace wr
{
#if defined(USE_FLAC)
#endif // USE_FLAC
	bool read_memory_flac_audio_all_pcm(const AudioFile& file, PcmBlock& pcm_out) noexcept;
#if defined(USE_OGG)
	bool read_memory_ogg_audio_pcm(const AudioFile& file, PcmBlock& audio_out) noexcept;
#endif // USE_OGG
	//bool read_memory_pcm_audio_pcm(const AudioFile& file, AudioPcmAllBlock& audio_out) noexcept { return true; }

} // namespace wr is end

#endif // _WR_RWAD_AUDIO_HPP_ IS EOF