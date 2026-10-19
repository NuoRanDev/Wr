#include <os/wrNew.hpp>
#include <core/wrDynamicArray.hpp>
#include <iostream>
#include <string>
#include <core/wrString.hpp>


int main()
{
	system("chcp 65001");
	wr::wr_string str = "六六来六，66来6，ぼっち来な魔王と俺の塔";
	auto c = str.mov_pod();
	wr::u8_str_release(c);

	return 0;
}