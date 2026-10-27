#ifndef _WR_OS_INFO_HPP_
#define _WR_OS_INFO_HPP_

#if defined(_WIN32)
#define os_namespace windows
#elif defined(__linux__)
#define os_namespace linux
#endif // choose os

#endif // _WR_OS_INFO_HPP_ IS EOF