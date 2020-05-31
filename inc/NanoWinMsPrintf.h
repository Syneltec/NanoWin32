// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MSVC-compatible wprintf/wcsanf format support
// (removes differences between processing %s,%c in "wide" printf functions between MSVC and GLIBC)
// Include this file in your code if you need ms-compatible wprintf/wscanf functions
// By default, all headers of NanoWin library does not alter operation of wprintf/wscanf functions
// Note: The remapping is not "free" and has some restrictions / edge cases
// 1. You cannot get addr of func anymore
// 2. You cannot name any object with these names (say, you cannot name your method as swprintf)

#if !defined(NanoWinMsPrintfIncluded)
#define NanoWinMsPrintfIncluded

#if !defined(NanoWinForceNativePrintfFormat) // Force avoid remapping of printf functions

#if defined(__GNUC__)

#include "../inc/NanoWinTypes.h"
#include "../inc/NanoWinMsWFormatProc.h"

#include <wchar.h>

#define NW_WPRINTF_FORMAT_REMAPPED // Flag that remapping is active

#define swprintf(buffer, count, format, ...)          NW_WPRINTF_RESULT_2_UNIX(int,swprintf(buffer, count, NW_WPRINTF_FORMAT_2_UNIX(format), ## __VA_ARGS__))
#define vswprintf(buffer, count, format, va_args)     NW_WPRINTF_RESULT_2_UNIX(int,vswprintf(buffer, count, NW_WPRINTF_FORMAT_2_UNIX(format), va_args))
#define wprintf(format, ...)                          NW_WPRINTF_RESULT_2_UNIX(int,wprintf(NW_WPRINTF_FORMAT_2_UNIX(format), ## __VA_ARGS__))
#define vwprintf(format, va_args)                     NW_WPRINTF_RESULT_2_UNIX(int,vwprintf(NW_WPRINTF_FORMAT_2_UNIX(format), va_args))
#define fwprintf(stream, format, ...)                 NW_WPRINTF_RESULT_2_UNIX(int,fwprintf(stream, NW_WPRINTF_FORMAT_2_UNIX(format), ## __VA_ARGS__))
#define vfwprintf(stream, format, va_args)            NW_WPRINTF_RESULT_2_UNIX(int,vfwprintf(stream, NW_WPRINTF_FORMAT_2_UNIX(format), va_args))

// Redefine functions from NanoWinMsExtra.*

#define snwprintf(buffer, count, format, ...)         NW_WPRINTF_RESULT_2_UNIX(int,snwprintf(buffer, count, NW_WPRINTF_FORMAT_2_UNIX(format), ## __VA_ARGS__))
#define vsnwprintf(buffer, count, format, va_args)    NW_WPRINTF_RESULT_2_UNIX(int,vsnwprintf(buffer, count, NW_WPRINTF_FORMAT_2_UNIX(format), va_args))

// Redefine "safe" functions from NanoWinMsSafePrintf.* (bit tricky)

#if defined(vswprintf_s)
#undef  vswprintf_s
#define vswprintf_s     vswprintf_s_ms
#endif

#if defined(swprintf_s)
#undef  swprintf_s
#define swprintf_s      swprintf_s_ms
#endif

#define NW_WSCANF_FORMAT_REMAPPED // Flag that remapping is active

#define swscanf(buffer, format, ...)                  NW_WSCANF_RESULT_2_UNIX(int,swscanf(buffer,NW_WSCANF_FORMAT_2_UNIX(format), ## __VA_ARGS__))
#define vswscanf(buffer, format, va_args)             NW_WSCANF_RESULT_2_UNIX(int,vswscanf(buffer,NW_WSCANF_FORMAT_2_UNIX(format), va_args))
#define wscanf(format, ...)                           NW_WSCANF_RESULT_2_UNIX(int,wscanf(NW_WSCANF_FORMAT_2_UNIX(format), ## __VA_ARGS__))
#define vwscanf(format, va_args)                      NW_WSCANF_RESULT_2_UNIX(int,vwscanf(NW_WSCANF_FORMAT_2_UNIX(format), va_args))
#define fwscanf(stream, format, ...)                  NW_WSCANF_RESULT_2_UNIX(int,fwscanf(buffer,NW_WSCANF_FORMAT_2_UNIX(format), ## __VA_ARGS__))
#define vfwscanf(stream, format, va_args)             NW_WSCANF_RESULT_2_UNIX(int,vfwscanf(buffer,NW_WSCANF_FORMAT_2_UNIX(format), av_args))

#endif // GCC

#endif // NanoWinForceNativePrintfFormat

#endif // ...Included

