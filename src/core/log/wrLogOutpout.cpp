// statement
#include <log/wrLogOutput.hpp>

#if defined(_DEBUG)
#define OPEN_CLR
#endif // is defined _DEBUG

// std
#include <exception>
#include <iostream>
#include <format>

namespace wr
{

	void WR_CLR_WRITE(const char* src, const char* color)
	{
		std::cout << std::string(color) + src + WR_CLR_COLOR::WHITE;
	}

	void WR_CLR_WRITE_LINE(const char* src, const char* color)
	{
		std::cout << std::string(color) + src + WR_CLR_COLOR::WHITE << std::endl;
	}

	void WR_CLR_OUTPUT(const char* color, const char* color_string, const char* output_type, const char* addon_type, const char* output_text, const char* file, int line, const char* function)
	{
		std::string address = std::format("{0} line:{1} {2}(...)", file, line, function);
#if defined(OPEN_CLR)
		std::cout << std::string(color) + color_string + ":\t" + WR_CLR_COLOR::WHITE + "< " + output_type + " " + addon_type + " -> " + output_text + "> in " + address << std::endl;
		return;
#endif // OPEN_CLR
		std::string out_text = std::string(color_string) + "< " + output_type + " " + addon_type + " -> " + output_text + "> in " + address + "\n";
		return;
	}

	void WR_CLR_OUTPUT_EXIT(
		const char* color, const char* color_string,
		const char* output_type,
		const char* addon_type, const char* output_text,
		const char* file, int line, const char* function)
	{
		WR_CLR_OUTPUT(color, color_string, output_type, addon_type, output_text, file, line, function);
		std::terminate();
	}
} // namespace wr is end