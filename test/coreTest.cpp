#include <core/wrDynamicArray.hpp>
#include <iostream>
#include <core/wrString.hpp>
#include <os/wrPath.hpp>
#include <chrono>
#include <format>
#include <os/wrFileMmapViewer.hpp>

int main()
{
	system("chcp 65001");
	wr::wr_string str = "C:\\Users\\wunuo\\Desktop\\大证\\屏幕截图 2026-07-30 215940.png";
	wr::path fp(str);

	auto gggg = fp.split();

	wr::dynamic_array<wr::wr_string> str_list(3);
	str_list.push_back(gggg.vale);
	str_list.push_back(fp.abs_path());
	str_list.push_back(gggg.key);

	auto ct = std::chrono::system_clock::from_time_t(fp.get_create_time());
	std::cout << std::format("file name: {0}\n", str_list.at(0).c_str());
	std::cout << std::format("abs path: {0}\n", str_list.at(1).c_str());
	std::cout << std::format("path: {0}\n", str_list.at(2).c_str());
	std::cout << std::format("size: {0}\n", fp.get_size());
	std::cout << "create: " << ct << std::endl;

	char png[4];
	char* ppng = (char*)(&png);

	wr::mmap_file_viewer vm;
	vm.open_file(fp);
	memcpy(ppng, (((char*)(vm.file_info.pfile_start)) + 1), 3);
	ppng[3] = 0;
	puts(ppng);
	vm.close_file();

	return 0;
}