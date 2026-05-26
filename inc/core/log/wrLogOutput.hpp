#ifndef _WR_ERROR_OURPUT_HPP_
#define _WR_ERROR_OURPUT_HPP_

namespace wr
{
	namespace WR_TYPE_NAME_OUTPUT
	{
		constexpr char LIB[] = "LIB";
		constexpr char APP[] = "APP";
	};

	namespace WR_CLR_COLOR
	{
		constexpr const char RESET[] = "\033[0m";
		constexpr const char RED[] = "\033[31m";
		constexpr const char GREEN[] = "\033[32m";
		constexpr const char YELLOW[] = "\033[33m";
		constexpr const char BLUE[] = "\033[34m";
		constexpr const char WHITE[] = "\033[37m";
	}

	void WR_CLR_OUTPUT(
		const char* color, const char* color_string, 
		const char* output_type,
		const char* addon_type, const char* output_text,
		const char* file, int line, const char* function);

	void WR_CLR_OUTPUT_EXIT(
		const char* color, const char* color_string,
		const char* output_type,
		const char* addon_type, const char* output_text,
		const char* file, int line, const char* function);

	void WR_CLR_WRITE(const char* src, const char* color = WR_CLR_COLOR::WHITE);

	void WR_CLR_WRITE_LINE(const char* src, const char* color = WR_CLR_COLOR::WHITE);

#define WR_FATAL_OUTPUT(output_type, addon_type, output_text)\
		WR_CLR_OUTPUT_EXIT(wr::WR_CLR_COLOR::RED, "FATAL", output_type, addon_type, output_text, __FILE__, __LINE__, __FUNCTION__);

#define WR_ERROR_OUTPUT(output_type, addon_type, output_text)\
		WR_CLR_OUTPUT(wr::WR_CLR_COLOR::RED, "ERROR", output_type, addon_type, output_text, __FILE__, __LINE__, __FUNCTION__);

#define WR_WARNING_OUTPUT(output_type, addon_type, output_text)\
		WR_CLR_OUTPUT(wr::WR_CLR_COLOR::YELLOW, "WARNING", output_type, addon_type, output_text, __FILE__, __LINE__, __FUNCTION__);

#define WR_INFO_OUTPUT(output_type, addon_type, output_text)\
		WR_CLR_OUTPUT(wr::WR_CLR_COLOR::GREEN, "INFO", output_type, addon_type, output_text, __FILE__, __LINE__, __FUNCTION__);

#define WR_DEBUG_OUTPUT(output_type, addon_type, output_text)\
		WR_CLR_OUTPUT(wr::WR_CLR_COLOR::BLUE, "INFO", output_type, addon_type, output_text, __FILE__, __LINE__, __FUNCTION__);
} // namespace wr is end

#endif // _WR_ERROR_OURPUT_HPP_ IS EOF