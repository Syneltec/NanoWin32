// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// NanoWin is designed to help port C/C++ code written for MS VS to GNU Linux

// NanoWin.h substitution for Windows.h
// replace #include <Windows.h> with #include <NanoWin.h>
// You may also rename NanoWin.h to Windows.h (or use soft link to map)

// Other files you probably may need to include:

// Microsoft extra features
// NanoWinTCHAR.h        // Nanowin substitute for TCHAR.h
// NanoWinMsExtra.h      // Extra functions defined in MS VS that is missing under GNU/POSIX like _fullpath
// NanoWinMsSafeString.h // Safe string functions like strcpy_s, memset_s and MS VS templates for them
// NanoWinMsSafePrintf.h // Microsoft safe vsprintf_s, sprintf_s extensions
// NanoWinMsSafeScanf.h  // Microsoft safe sscanf_s, fscanf_s extensions (some exotic cases not supported, see note)

// Microsoft MFC subset
// NanoWinMFCAfx*.h      // Nanowin MFC Classes simulators (include it instead of MFCAfx*.h files)
// NanoWinMFCAfxNUI.h    // Nanowin NoGUI stub for some basic MFC GUI functions (actions will be just output to console)

// Quirks and tricks (include if you really need it)
// NanoWinMsPrintf.h     // MSVC-compatible wprintf/wcsanf format support for %s,%c format chars (non-POSIX)
// NanoWinPathHook*.cpp  // Add to your project to allow path sep as '/' and '\\' (see link note in NanoWinPathHookGate.cpp)

#if !defined(NanoWinIncluded)
#define NanoWinIncluded

#include "inc/NanoWinTypes.h"
#include "inc/NanoWinSystem.h"
#include "inc/NanoWinOther.h"
#include "inc/NanoWinError.h"
#include "inc/NanoWinEnv.h"
#include "inc/NanoWinFileSys.h"
#include "inc/NanoWinFileFind.h"
#include "inc/NanoWinFile.h"
#include "inc/NanoWinCriticalSection.h"
#include "inc/NanoWinEvents.h"
#include "inc/NanoWinModule.h"
#include "inc/NanoWinBmp.h"
#include "inc/NanoWinRECT.h"
#include "inc/NanoWinStrConvert.h"
#include "inc/NanoWinThreads.h"
#include "inc/NanoWinWaitableTimer.h"
#include "inc/NanoWinQueryPerf.h" // QueryPerformanceCounter and alike
#include "inc/NanoWinPrivateProfile.h" // .ini file functions

// Some xtrats

#include "inc/NanoWinShellEx.h"      // SHCreateDirectory // originaly declared in "Shlobj.h"
#include "inc/NanoWinTCHAR.h"        // Nanowin substitute for TCHAR.h
#include "inc/NanoWinMsExtra.h"      // Extra functions defined in MS VS that is missing under GNU/POSIX like _fullpath

#include "inc/NanoWinMFCAfxStr.h"      // Extra functions defined in MS VS that is missing under GNU/POSIX like _fullpath

#endif
