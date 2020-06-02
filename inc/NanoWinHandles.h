#ifndef __NANOWINHANDLES_H__
#define __NANOWINHANDLES_H__

#include "inc/NanoWinTypes.h"

#if defined LINUX



NW_EXTERN_C_BEGIN

WINBASEAPI BOOL WINAPI CloseHandle (HANDLE);

NW_EXTERN_C_END

#endif

#endif
