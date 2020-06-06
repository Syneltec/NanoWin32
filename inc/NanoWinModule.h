// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Module functions

#if !defined(NanoWinModuleIncluded)
#define NanoWinModuleIncluded

#include "inc/NanoWinTypes.h"

#if defined LINUX

/* CreateProcess: dwCreationFlag values*/
#define DEBUG_PROCESS               0x00000001
#define DEBUG_ONLY_THIS_PROCESS     0x00000002
//efine CREATE_SUSPENDED            0x00000004 // defined in NaniWinThread
#define DETACHED_PROCESS            0x00000008
#define CREATE_NEW_CONSOLE          0x00000010
/* STARTUPINFO.dwFlags */
#define STARTF_USESHOWWINDOW  0x00000001
/*** ShowWindow() codes ***/
#define SW_HIDE                0
#define SW_SHOWNORMAL          1
#define SW_NORMAL              SW_SHOWNORMAL
#define SW_SHOWMINIMIZED       2
#define SW_SHOWMAXIMIZED       3
#define SW_MAXIMIZE            SW_SHOWMAXIMIZED
#define SW_SHOWNOACTIVATE      4
#define SW_SHOW                5
#define SW_MINIMIZE            6

typedef struct _PROCESS_INFORMATION{
  HANDLE  hProcess;
  HANDLE  hThread;
  DWORD   dwProcessId;
  DWORD   dwThreadId;
} PROCESS_INFORMATION, *PPROCESS_INFORMATION, *LPPROCESS_INFORMATION;

typedef struct _STARTUPINFOA{
        DWORD cb;   /* 00: size of struct */
        LPSTR lpReserved; /* 04: */
        LPSTR lpDesktop;  /* 08: */
        LPSTR lpTitle;    /* 0c: */
        DWORD dwX;    /* 10: */
        DWORD dwY;    /* 14: */
        DWORD dwXSize;    /* 18: */
        DWORD dwYSize;    /* 1c: */
        DWORD dwXCountChars;  /* 20: */
        DWORD dwYCountChars;  /* 24: */
        DWORD dwFillAttribute;  /* 28: */
        DWORD dwFlags;    /* 2c: */
        WORD wShowWindow; /* 30: */
        WORD cbReserved2; /* 32: */
        BYTE *lpReserved2;  /* 34: */
        HANDLE hStdInput; /* 38: */
        HANDLE hStdOutput;  /* 3c: */
        HANDLE hStdError; /* 40: */
} STARTUPINFOA, *LPSTARTUPINFOA;

typedef struct _STARTUPINFOW{
        DWORD cb;
        LPWSTR lpReserved;
        LPWSTR lpDesktop;
        LPWSTR lpTitle;
        DWORD dwX;
        DWORD dwY;
        DWORD dwXSize;
        DWORD dwYSize;
        DWORD dwXCountChars;
        DWORD dwYCountChars;
        DWORD dwFillAttribute;
        DWORD dwFlags;
        WORD wShowWindow;
        WORD cbReserved2;
        BYTE *lpReserved2;
        HANDLE hStdInput;
        HANDLE hStdOutput;
        HANDLE hStdError;
} STARTUPINFOW, *LPSTARTUPINFOW;

DECL_WINELIB_TYPE_AW(STARTUPINFO)
DECL_WINELIB_TYPE_AW(LPSTARTUPINFO)

NW_EXTERN_C_BEGIN

#define NW_HANDLE_CURR_PROCCESS        NW_HANDLE_SPEC_VALUE(-2) // Under win32 is "-1", this is bad since -1 is same as INVALID_HANDLE_VALUE (we use -2 that better a bit)

#if defined(__cplusplus)
inline bool NanoWinIsCurrentProccessHandle(HANDLE proccess)
{
  if (proccess == NW_HANDLE_CURR_PROCCESS) { return(true); }
  if (proccess == INVALID_HANDLE_VALUE)    { return(true); } // for Win32 compatibility, since curr proccess pseudo handle is -1 (same value as INVALID_HANDLE_VALUE)
  return(false);
}
#endif

extern BOOL    WINAPI CreateProcessA(LPCSTR,LPSTR,LPSECURITY_ATTRIBUTES,LPSECURITY_ATTRIBUTES,BOOL,DWORD,LPVOID,LPCSTR,LPSTARTUPINFOA,LPPROCESS_INFORMATION);
extern BOOL    WINAPI CreateProcessW(LPCWSTR,LPWSTR,LPSECURITY_ATTRIBUTES,LPSECURITY_ATTRIBUTES,BOOL,DWORD,LPVOID,LPCWSTR,LPSTARTUPINFOW,LPPROCESS_INFORMATION);

extern VOID    WINAPI ExitProcess     (DWORD);
extern BOOL    WINAPI TerminateProcess(HANDLE,DWORD);

// The return value is a pseudo handle to the current process.
// The pseudo handle as a special handle value used to identify current proccess
extern HANDLE  WINAPI GetCurrentProcess();

// Retrieves the process identifier of the calling (current) process.
// Until the process terminates, the process identifier uniquely identifies the process throughout the system.
extern DWORD   WINAPI GetCurrentProcessId(void);

extern HMODULE WINAPI GetModuleHandleA(LPCSTR_NULLONLY  lpModuleName);
extern HMODULE WINAPI GetModuleHandleW(LPCWSTR_NULLONLY lpModuleName);

//NOTE: only hModule == NULL argument is supported
extern DWORD WINAPI   GetModuleFileNameA(_In_opt_ HMODULE hModule, _Out_ LPSTR lpFilename, _In_ DWORD nSize);
extern DWORD WINAPI   GetModuleFileNameW(_In_opt_ HMODULE hModule, _Out_ LPWSTR lpFilename, _In_ DWORD nSize);

// Dlls

// Loads the specified module into the address space of the calling process. The specified module may cause other modules to be loaded.
// If lpFileName does not include a path and there is more than one loaded module with the same base name and extension, the function returns a handle to the module that was loaded first
// If the function fails, the return value is NULL. (Use GetLastError)
extern HMODULE WINAPI LoadLibraryA(_In_ LPCSTR  lpFileName);

// Loads the specified module into the address space of the calling process. The specified module may cause other modules to be loaded.
// If lpFileName does not include a path and there is more than one loaded module with the same base name and extension, the function returns a handle to the module that was loaded first
// If the function fails, the return value is NULL (use GetLastError to get error code)
extern HMODULE WINAPI LoadLibraryW(_In_ LPCWSTR lpFileName);

// Frees the loaded dynamic-link library
extern BOOL    WINAPI FreeLibrary(_In_ HMODULE hModule);

// Retrieves the address of an exported function or variable from the specified dynamic-link library
// If the function fails, the return value is NULL (use GetLastError to get error code)
// Note: The only version exists in Win32 is ASCII?
extern FARPROC WINAPI GetProcAddress(_In_ HMODULE hModule, _In_ LPCSTR lpProcName); 

#if defined(UNICODE) || defined(_UNICODE)
typedef STARTUPINFOW, *LPSTARTUPINFOW
#define CreateProcess         CreateProcessW
#define GetModuleHandle       GetModuleHandleW
#define GetModuleFileName     GetModuleFileNameW
#define LoadLibrary           LoadLibraryW
#else
#define CreateProcess         CreateProcessA
#define GetModuleHandle       GetModuleHandleA
#define GetModuleFileName     GetModuleFileNameA
#define LoadLibrary           LoadLibraryA
#endif

NW_EXTERN_C_END

#endif // linux
#endif // ...Included
