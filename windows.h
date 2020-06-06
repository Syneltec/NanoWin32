#if !defined(Nano_windows_h_Included)
#define Nano_windows_h_Included

// Avoids some RTX redefinitions
#define _WINDEF_

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
//#include "inc/NanoWinMFCAfxStr.h"
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
#define ROUND_UP(Dividend, Divisor)     ((((Dividend) + (Divisor) - 1) / (Divisor)) * (Divisor))
#define ROUND_DOWN(Dividend, Divisor)   (((Dividend) / (Divisor)) * (Divisor))

// TODO REORGANIZE
//----------------
WINBASEAPI void WINAPI DebugBreak(void);
WINBASEAPI LPSTR       WINAPI GetCommandLineA(void);
WINBASEAPI LPWSTR      WINAPI GetCommandLineW(void);
#define                       GetCommandLine WINELIB_NAME_AW(GetCommandLine)


//Serial Port TODO Adapt for linux
typedef struct tagCOMSTAT
{
    DWORD fCtsHold : 1;
    DWORD fDsrHold : 1;
    DWORD fRlsdHold : 1;
    DWORD fXoffHold : 1;
    DWORD fXoffSent : 1;
    DWORD fEof : 1;
    DWORD fTxim : 1;
    DWORD fReserved : 25;
    DWORD cbInQue;
    DWORD cbOutQue;
} COMSTAT, *LPCOMSTAT;

typedef struct tagDCB
{
    DWORD DCBlength;
    DWORD BaudRate;
    unsigned fBinary               :1;
    unsigned fParity               :1;
    unsigned fOutxCtsFlow          :1;
    unsigned fOutxDsrFlow          :1;
    unsigned fDtrControl           :2;
    unsigned fDsrSensitivity       :1;
    unsigned fTXContinueOnXoff     :1;
    unsigned fOutX                 :1;
    unsigned fInX                  :1;
    unsigned fErrorChar            :1;
    unsigned fNull                 :1;
    unsigned fRtsControl           :2;
    unsigned fAbortOnError         :1;
    unsigned fDummy2               :17;
    WORD wReserved;
    WORD XonLim;
    WORD XoffLim;
    BYTE ByteSize;
    BYTE Parity;
    BYTE StopBits;
    char XonChar;
    char XoffChar;
    char ErrorChar;
    char EofChar;
    char EvtChar;
    WORD wReserved1;
} DCB, *LPDCB;

typedef struct tagCOMMCONFIG {
  DWORD dwSize;
  WORD  wVersion;
  WORD  wReserved;
  DCB   dcb;
  DWORD dwProviderSubType;
  DWORD dwProviderOffset;
  DWORD dwProviderSize;
  DWORD wcProviderData[1];
} COMMCONFIG, *LPCOMMCONFIG;

typedef struct tagCOMMPROP {
  WORD  wPacketLength;
  WORD  wPacketVersion;
  DWORD dwServiceMask;
  DWORD dwReserved1;
  DWORD dwMaxTxQueue;
  DWORD dwMaxRxQueue;
  DWORD dwMaxBaud;
  DWORD dwProvSubType;
  DWORD dwProvCapabilities;
  DWORD dwSettableParams;
  DWORD dwSettableBaud;
  WORD  wSettableData;
  WORD  wSettableStopParity;
  DWORD dwCurrentTxQueue;
  DWORD dwCurrentRxQueue;
  DWORD dwProvSpec1;
  DWORD dwProvSpec2;
  WCHAR wcProvChar[1];
} COMMPROP, *LPCOMMPROP;

#define SP_SERIALCOMM ((DWORD)1)



#endif
