#ifndef _INC_WR_COMPILER_H_
#define _INC_WR_COMPILER_H_

#if defined(__GNUC__) || defined(__clang__)
#define forceinline		inline __attribute__((always_inline))
#define exportfunc		__attribute__((visibility("default")))
#elif defined(_MSC_VER)
#define forceinline		__forceinline
#define exportfunc		__declspec(dllexport)
#else
#define forceinline		inline
#define exportfunc		extern
#endif

#endif // _INC_WR_COMPILER_H_ IS EOF