#ifndef __NANOWINTHREADS_H__
#define __NANOWINTHREADS_H__

#include "inc/NanoWinTypes.h"

#if defined LINUX

/* handler routine control signal type */
#define CTRL_C_EVENT        0
#define CTRL_BREAK_EVENT    1
#define CTRL_CLOSE_EVENT    2
#define CTRL_LOGOFF_EVENT   5
#define CTRL_SHUTDOWN_EVENT 6

/* CreateThread: dwCreationFlag, Error codes */
#define THREAD_PRIORITY_LOWEST          1
#define THREAD_PRIORITY_BELOW_NORMAL    31
#define THREAD_PRIORITY_NORMAL          63
#define THREAD_PRIORITY_ABOVE_NORMAL    95
#define THREAD_PRIORITY_HIGHEST         127

#define CREATE_SUSPENDED                 0x00000004
#define THREAD_PRIORITY_ERROR_RETURN    (0x7fffffff)
#define SYNCHRONIZE                      0x00100000


NW_EXTERN_C_BEGIN

typedef DWORD(*PTHREAD_START_ROUTINE)  (LPVOID lpThreadParameter);
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;

typedef BOOL (WINAPI *PHANDLER_ROUTINE)(DWORD dwCtrlType);
extern  BOOL  WINAPI SetConsoleCtrlHandler (PHANDLER_ROUTINE,BOOL);


extern HANDLE CreateThread
(
	LPSECURITY_ATTRIBUTES  lpThreadAttributes,
	SIZE_T                 dwStackSize,
	LPTHREAD_START_ROUTINE lpStartAddress,
	LPVOID                 lpParameter,
	DWORD                  dwCreationFlags,
	LPDWORD                lpThreadId
);

extern BOOL TerminateThread
(
	HANDLE hThread,
	DWORD  dwExitCode
);

extern DWORD  WINAPI SuspendThread         (HANDLE);
extern DWORD  WINAPI ResumeThread          (HANDLE);
extern HANDLE WINAPI GetCurrentThread      (void);
extern DWORD  WINAPI GetCurrentThreadId    (void);

extern VOID WINAPI ExitThread(_In_ DWORD dwExitCode);

extern BOOL CloseThreadHandle(HANDLE hThread);

extern DWORD WaitForSingleThread(HANDLE hThread, DWORD dwMilliseconds);

extern DWORD WaitForMultipleThreads // WaitForMultipleObjects implementation for events
(
  DWORD         nCount,
  const HANDLE *lpHandles,
  BOOL          bWaitAll,
  DWORD         dwMilliseconds
);

//NOTE: this function is NanoWin-specific and does not exist in WinAPI
extern HANDLE        GetEventByThread (HANDLE hThread);

NW_EXTERN_C_END

#endif

#endif
