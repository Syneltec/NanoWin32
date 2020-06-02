#if !defined(Nano_windows_h_Included)
#define Nano_windows_h_Included

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
#include "inc/NanoWinThreads.h"
#include "inc/NanoWinWaitableTimer.h"
#include "inc/NanoWinQueryPerf.h" // QueryPerformanceCounter and alike
#include "inc/NanoWinPrivateProfile.h" // .ini file functions

// Some xtrats
#include "inc/NanoWinTCHAR.h"        // Nanowin substitute for TCHAR.h
#include "inc/NanoWinHandles.h"
#include "inc/NanoWinShellEx.h"      // SHCreateDirectory // originaly declared in "Shlobj.h"
#include "inc/NanoWinMFCAfxWin.h"

#ifdef __cplusplus
#include "inc/NanoWinMFCAfxStr.h"
#include "inc/NanoWinMsExtra.h"      // Extra functions defined in MS VS that is missing under GNU/POSIX like _fullpath
#include "inc/NanoWinStrConvert.h"
#include "inc/NanoWinOAVariant.h"
#endif

#include "guiddef.h"

/* min and max macros */
#ifndef NOMINMAX
#ifndef max
#define max(a,b)   (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)   (((a) < (b)) ? (a) : (b))
#endif
#endif  /* NOMINMAX */

//----------------------------


#endif
