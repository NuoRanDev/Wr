#include "log/wrLogOutput.hpp"
#include "filesystem/wrFileMmapStream.hpp"
//#include "wrAudioPlayerInstance.hpp"
//#include "wrSoundSource.hpp"
//#include "file/audio/wrReadAudioFile.hpp"
#include "platform/wrWindowsPlatformWindow.hpp"
#include "wrWindow.hpp"


#include <format>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>

using namespace wr;
/*
vec3f v;
vec3f pos;
float pi = 3.1415926f;
float w = pi / 10;
float r = 10;

void rander(SoundSource& ssrc, double ms)
{
	double st = (w * ms) - ((long long)((w * ms) / (2 * pi)) * (2 * pi));
	v = { w * r * cosf((float)(st)-pi / 2) ,
		w * r * sinf((float)(st)-pi / 2),
		0
	};
	ssrc.set_velocity(v);
	pos = { cosf((float)st) * r ,
		 sinf((float)st) * r,
		0
	};
	std::cout << "v Vx: " << v.x << " Vy: " << v.y << " Vz: " << v.z << "\n";
	std::cout << "pos x: " << pos.x << " y: " << pos.y << " z: " << pos.z << "\n";
	// windos
	system("cls");
}

void fakerander(SoundSource& ssrc, double ms)
{
	double st = (w * ms) - ((long long)((w * ms) / (2 * pi)) * (2 * pi));
	v = {
		sinf((float)st) * r,
		0,
		0
	};
	ssrc.set_velocity(v);
	pos = {
		cosf((float)st) * r ,
		0,
		0
	};
	std::cout << "v Vx: " << v.x << " Vy: " << v.y << " Vz: " << v.z << "\n";
	std::cout << "pos x: " << pos.x << " y: " << pos.y << " z: " << pos.z << "\n";
	// windos
	system("cls");
}*/

int main(int argc, char** argv)
{
#if defined(_WIN32)
	system("chcp 65001");
	system("cls");
#endif

	/*
	//String path = "C:\\Users\\wunuo\\Desktop\\In the HALL OF THE MOU - PHILHARMONIA ORCHESTRA of Hamburg.flac";
	Path path = Path(spath);
	time_t create_time = path.get_create_time();

	std::cout << "File : " << spath << std::endl;
	std::cout << "Create time : " << std::ctime(&create_time);
	std::cout << "Size : " << path.get_size() / 1024 << " KiB " << std::endl;
	
	
	Mmapfstream flac_file = Mmapfstream();
	flac_file.open_file(path);

	AudioFile adf;
	size_t sz;
	String u8_s = u8"Flac test";
	auto flac_data = flac_file.data(sz);
	adf.load_file_not_copy_in_memory(flac_data, sz, u8_s);

	PcmBlock pcm;
	read_memory_flac_audio_all_pcm(adf, pcm);
	flac_file.release();

	AudioPlayerInstance a_instc;
	a_instc.init();

	SoundSource sscrc = SoundSource();
	sscrc.lood_audio(pcm);
	sscrc.set_gain(1.0f);
	sscrc.set_is_loop(true);
	sscrc.set_pitch(1.0f);

	auto start = std::chrono::system_clock::now();	
	sscrc.play();
	using std::cout, std::endl;
	using std::chrono::duration_cast, std::chrono::system_clock;
	using std::chrono::milliseconds, std::chrono::seconds;

	while(true)
	{
		auto ms_cpp =
			duration_cast<milliseconds>(system_clock::now().time_since_epoch())
			.count();
		double ms = (double)ms_cpp / 1000.0;

		rander(sscrc, ms);
		//fakerander(sscrc, ms);
		// linux
		//system("clear");
		sscrc.set_position(pos);
	}

	wr::WindowManager wmsg = wr::WindowManager(argv);
	VulkanContext vtx = { 0 };
	uint32_t c;
	auto vc = SDL_Vulkan_GetInstanceExtensions(&c);
	init_vulkan_instance(vtx, u8"name", vc, c);
	find_gpu(vtx);
	auto name = wr::String("sasas");
	auto win = wmsg.create_window(600, 800, name);
	win->show();*/

	init_windows_env();

	String str = u8"六";
	U16StringRef _16str = str;
	create_window(_16str, 800, 600, WindowStyle::Close);
	while (switch_enevt())
	{
	}
	
	return EXIT_SUCCESS; // Success
}