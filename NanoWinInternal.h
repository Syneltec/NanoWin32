// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Internal functions

#if !defined(NanoWinInternalIncluded)
#define NanoWinInternalIncluded

#include "NanoWinTypes.h"

#if defined LINUX

NW_EXTERN_C_BEGIN

#if defined(UNICODE) || defined(_UNICODE)
#else
#endif

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
