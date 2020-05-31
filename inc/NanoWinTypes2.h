typedef struct _FILETIME
{
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;


typedef UINT_PTR        WPARAM;
typedef LONG_PTR        LPARAM;
typedef LONG_PTR        LRESULT;
typedef LONG            HRESULT;

typedef BOOL (WINAPI *PHANDLER_ROUTINE)(DWORD dwCtrlType);

typedef void *HANDLE;
typedef HANDLE *PHANDLE, *LPHANDLE;


/* CreateProcess: dwCreationFlag, Error codes */
#define CREATE_SUSPENDED                 0x00000004
#define THREAD_PRIORITY_ERROR_RETURN    (0x7fffffff)
#define SYNCHRONIZE                      0x00100000

/* Shared Memory creation flags */
#define PAGE_NOACCESS       0x01
#define PAGE_READONLY       0x02
#define PAGE_READWRITE      0x04


#ifdef WINE_NO_UNICODE_MACROS
# define WINELIB_NAME_AW(func) \
    func##_must_be_suffixed_with_W_or_A_in_this_context \
    func##_must_be_suffixed_with_W_or_A_in_this_context
#else  /* WINE_NO_UNICODE_MACROS */
# ifdef UNICODE
#  define WINELIB_NAME_AW(func) func##W
# else
#  define WINELIB_NAME_AW(func) func##A
# endif
#endif  /* WINE_NO_UNICODE_MACROS */

#ifdef WINE_NO_UNICODE_MACROS
# define DECL_WINELIB_TYPE_AW(type)  /* nothing */
#else
# define DECL_WINELIB_TYPE_AW(type)  typedef WINELIB_NAME_AW(type) type;
#endif



WINBASEAPI BOOL        WINAPI CloseHandle          (HANDLE);
WINBASEAPI DWORD       WINAPI SuspendThread        (HANDLE);
WINBASEAPI DWORD       WINAPI ResumeThread         (HANDLE);
WINBASEAPI BOOL        WINAPI SystemTimeToFileTime (const SYSTEMTIME*,LPFILETIME);
WINBASEAPI BOOL        WINAPI GetFileTime(HANDLE,LPFILETIME,LPFILETIME,LPFILETIME);
WINBASEAPI BOOL        WINAPI SetConsoleCtrlHandler(PHANDLER_ROUTINE,BOOL);
WINBASEAPI void        WINAPI RaiseException(DWORD,DWORD,DWORD,const ULONG_PTR *);

WINUSERAPI BOOL        WINAPI PostThreadMessageA(DWORD,UINT,WPARAM,LPARAM);
WINUSERAPI BOOL        WINAPI PostThreadMessageW(DWORD,UINT,WPARAM,LPARAM);
#define                       PostThreadMessage WINELIB_NAME_AW(PostThreadMessage)

WINBASEAPI BOOL        WINAPI GetPrivateProfileStructA(LPCSTR,LPCSTR,LPVOID,UINT,LPCSTR);
WINBASEAPI BOOL        WINAPI GetPrivateProfileStructW(LPCWSTR,LPCWSTR,LPVOID,UINT,LPCWSTR);
#define                       GetPrivateProfileStruct WINELIB_NAME_AW(GetPrivateProfileStruct)
WINBASEAPI BOOL        WINAPI WritePrivateProfileStructA(LPCSTR,LPCSTR,LPVOID,UINT,LPCSTR);
WINBASEAPI BOOL        WINAPI WritePrivateProfileStructW(LPCWSTR,LPCWSTR,LPVOID,UINT,LPCWSTR);
#define                       WritePrivateProfileStruct WINELIB_NAME_AW(WritePrivateProfileStruct)


/* min and max macros */
#ifndef NOMINMAX
#ifndef max
#define max(a,b)   (((a) > (b)) ? (a) : (b))
#endif


#ifndef min
#define min(a,b)   (((a) < (b)) ? (a) : (b))
#endif
#endif  /* NOMINMAX */

#ifdef MAX_PATH /* Work-around for Mingw */
#undef MAX_PATH
#endif /* MAX_PATH */

char * WINAPI itoa(int,char*,int);

#define FILE_FLAG_OVERLAPPED            0x40000000


#define MAKE_HRESULT(sev,fac,code) \
    ((HRESULT) (((unsigned int)(sev)<<31) | ((unsigned int)(fac)<<16) | ((unsigned int)(code))) )


#define _HRESULT_TYPEDEF_(x) ((HRESULT)x)

#define NOERROR                                            _HRESULT_TYPEDEF_(0)
#define S_OK                                               _HRESULT_TYPEDEF_(0)
#define SEC_E_OK                                           _HRESULT_TYPEDEF_(0)
#define S_FALSE                                            _HRESULT_TYPEDEF_(1)


