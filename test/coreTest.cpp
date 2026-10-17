#include <os/wrNew.hpp>
#include <core/wrDynamicArray.hpp>
#include <iostream>
#include <string>
#include <core/string/wrU8String.hpp>


int main()
{
	system("chcp 65001");
	wr::u8_string str = { 0 };
	const char* cstr = "六六来六，66来6，ぼっち来な魔王と俺の塔";
	str = wr::u8_str_load_c_str(cstr, 58);

	wr::unicode_t char_32 = U'来';

	str = wr::u8_str_append_unicode_char(str, char_32);

	wr::u8_string sp = { 0 };
	const char* cstrsp = "来";
	sp = wr::u8_str_load_c_str(cstrsp, 4);

	auto list = wr::dynamic_array<wr::u8_string>(wr::u8_str_split(str, sp));
	for (size_t i = 0; i < list.num(); i++)
	{
		auto cl = list.at(i).char_data;
		if (cl)
			puts((char*)(cl));
		wr::u8_str_release(list.at(i));
	}
	puts((char*)(str.char_data));

	auto cprr = wr::u8_str_slice(str, 8, 12);
	puts((char*)(cprr.char_data));

	wr::unicode_t char_y = wr::u8_str_at(str, 1);
	wr::u8_str_release(str);
	wr::u8_str_release(sp);
	wr::u8_str_release(cprr);
	return 0;
}