// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFCApp functions
// Subset of Windows API GUI functions stubs for MFC Np-UI (NUI) applications

#if !defined(NanoWinMFCAfxWinIncluded)
#define NanoWinMFCAfxWinIncluded

#include "inc/NanoWinRECT.h"

#if defined LINUX

// Win32 GUI API Subset
// ==================================
// Move me to NanoWinNUI.h

// Some basic Win32 GUI related types

typedef LONG_PTR LPARAM;
typedef UINT_PTR WPARAM;
typedef LONG_PTR LRESULT;

#ifndef NW_HWND_DEFINED
#define NW_HWND_DEFINED
NW_PNULLONLY_TYPE(HWND);    // typedef HANDLE HWND;    // Can assigned or passed NULL only
#endif

NW_PNULLONLY_TYPE(HCURSOR); // typedef HANDLE HCURSOR; // Can assigned or passed NULL only
NW_PNULLONLY_TYPE(HICON);   // typedef HANDLE HICON;   // Can assigned or passed NULL only

// WinUser.h
// Dialog Box Command IDs

// Return "pressed button" values
#define IDOK                        1
#define IDCANCEL                    2
#define IDABORT                     3
#define IDRETRY                     4
#define IDIGNORE                    5
#define IDYES                       6
#define IDNO                        7
#define IDCLOSE                     8
#define IDHELP                      9
#define IDTRYAGAIN                  10
#define IDCONTINUE                  11

// Input "button to show values"
#define MB_OK                       0x00000000L
#define MB_OKCANCEL                 0x00000001L
#define MB_ABORTRETRYIGNORE         0x00000002L
#define MB_YESNOCANCEL              0x00000003L
#define MB_YESNO                    0x00000004L
#define MB_RETRYCANCEL              0x00000005L
#define MB_CANCELTRYCONTINUE        0x00000006L
#define MB_HELP                     0x00004000L // Bitmask to add help Button

// Icon to show bitmask
#define MB_ICONHAND                 0x00000010L
#define MB_ICONQUESTION             0x00000020L
#define MB_ICONSTOP                 MB_ICONHAND
#define MB_ICONERROR                MB_ICONHAND

// Default button bitmask
#define MB_DEFBUTTON1               0x00000000L // The first button is the default button, MB_DEFBUTTON1 is the default unless MB_DEFBUTTON2, MB_DEFBUTTON3, or MB_DEFBUTTON4 is specified.
#define MB_DEFBUTTON2               0x00000100L
#define MB_DEFBUTTON3               0x00000200L
#define MB_DEFBUTTON4               0x00000300L

#define NW_MB_GET_BUTTON(nType)     (nType & 0xF)
#define NW_MB_GET_DEFBUTTON(nType)  (nType & 0xF00)
#define NW_MB_GET_ICON(nType)       (nType & 0xF0)

/* Windows Messages (USER subset) */
#define WM_INITDIALOG          0x0110
#define WM_COMMAND             0x0111
#define WM_SYSCOMMAND          0x0112
#define WM_TIMER               0x0113

/* PeekMessage() options */
#define PM_NOREMOVE       0x0000
#define PM_REMOVE         0x0001
#define PM_NOYIELD        0x0002

typedef struct tagMSG
{
    HWND    hwnd;
    UINT    message;
    WPARAM  wParam;
    LPARAM  lParam;
    DWORD   time;
    POINT   pt;
} MSG, *PMSG, *LPMSG;


// Returns default message box button as pressed
NW_EXTERN_C int WINAPI MessageBoxA(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType);
NW_EXTERN_C int WINAPI MessageBoxW(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType);
NW_EXTERN_C int WINAPI PostThreadMessageA    (DWORD,UINT,WPARAM,LPARAM);
NW_EXTERN_C int WINAPI PostThreadMessageW    (DWORD,UINT,WPARAM,LPARAM);
NW_EXTERN_C int WINAPI OutputDebugStringA    (LPCSTR);
NW_EXTERN_C int WINAPI OutputDebugStringW    (LPCWSTR);

NW_EXTERN_C LRESULT WINAPI DispatchMessageA(const MSG*);
NW_EXTERN_C LRESULT WINAPI DispatchMessageW(const MSG*);
NW_EXTERN_C BOOL    WINAPI PeekMessageA(LPMSG,HWND,UINT,UINT,UINT);
NW_EXTERN_C BOOL    WINAPI PeekMessageW(LPMSG,HWND,UINT,UINT,UINT);

#if defined(UNICODE) || defined(_UNICODE)
#define PeekMessage        PeekMessageW
#define DispatchMessage    DispatchMessageW
#define MessageBox         MessageBoxW
#define PostThreadMessage  PostThreadMessageW
#define OutputDebugString  OutputDebugStringW
#else
#define PeekMessage        PeekMessageA
#define DispatchMessage    DispatchMessageA
#define MessageBox         MessageBoxA
#define PostThreadMessage  PostThreadMessageA
#define OutputDebugString  OutputDebugStringA
#endif

#endif // linux
#endif // ...Included
