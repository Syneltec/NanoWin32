// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFCFile functions

#if !defined(NanoWinMFCAfxFileIncluded)
#define NanoWinMFCAfxFileIncluded

#include "NanoWinTypes.h"

#if defined LINUX

#include <libgen.h>

#include "NanoWinFileFind.h"
#include "NanoWinFile.h"

#include "NanoWinMFCAfx.h"
#include "NanoWinMFCAfxStr.h"

#if !defined(CAtlTransactionManager)
class    CAtlTransactionManager; // // [Dummy object] Define it somewhere
#define  CAtlTransactionManager  CAtlTransactionManager // [Dummy object] // just flag its defined
#endif

class CFileFind : public CObject
{
  protected:
  WIN32_FIND_DATA  *m_found_state;
  WIN32_FIND_DATA  *m_next_state;
  HANDLE            m_handle;
  BOOL              m_state_filled;
  CString           m_search_root;

  static BOOL IsValidHandle(HANDLE handle)
  {
    if ((handle == NULL) || (handle == INVALID_HANDLE_VALUE))
	{
      return(FALSE);
	}

    return(TRUE);
  }

  virtual void CloseContext()
  {
    if (!IsValidHandle(m_handle))
    {
      // Already closed or not open
    }
	else
	{
      FindClose(m_handle);
	}

   m_handle = NULL;
   m_state_filled = FALSE;

   m_search_root.Empty();
  }

  public:

  CFileFind()
  {
    m_handle = NULL;
    m_state_filled = FALSE;

    m_next_state  = (WIN32_FIND_DATA*)malloc(sizeof(WIN32_FIND_DATA));
    m_found_state = (WIN32_FIND_DATA*)malloc(sizeof(WIN32_FIND_DATA));

    if (m_next_state == NULL || m_found_state == NULL)
    {
      free(m_next_state);
      free(m_found_state);

      throw new CMemoryException();
    }
  }

  virtual ~CFileFind()
  {
    free(m_next_state);
    free(m_found_state);

    CloseContext();
  }

  virtual BOOL FindFile(LPCTSTR pstrName = NULL, DWORD dwUnused = 0)
  {
    CloseContext();

    LPCTSTR fileMask = pstrName != NULL ? pstrName : WILDCARD_ALL_FILES;

	m_handle = ::FindFirstFile(fileMask, m_next_state);

    if (IsValidHandle(m_handle))
    {
      if (!BuildSearchRootDir(fileMask))
      {
        CloseContext();
      }
    }

	if (IsValidHandle(m_handle))
	{
      return(TRUE);
	}
	else
	{
      memset(m_next_state, 0, sizeof(*m_next_state)); // just in case
      memset(m_found_state, 0, sizeof(*m_found_state)); // just in case
      return(FALSE);
	}
  }

  virtual BOOL FindNextFile()
  {
    if (!IsValidHandle(m_handle))
	{
      return(FALSE);
	}

    m_state_filled = TRUE;

    WIN32_FIND_DATA *temp_data;
    
    temp_data     = m_found_state;
    m_found_state = m_next_state;
    m_next_state  = temp_data;

	BOOL result = ::FindNextFile(m_handle, m_next_state);

	return(result);
  }

  void Close()
  {
    // Note: we can restart by rusing CFindFile object, but MFC API does not gurantee that
    CloseContext();
  }

  // Found file info

  // Returns the file name, including the extension. 
  // c:\myhtml\myfile.txt returns the file name myfile.txt
  virtual CString GetFileName()
  const
  {
    if (!m_state_filled) { return CString(); } // TODO: Check should we throw exception?
	return(CString(m_found_state->cFileName));
  }

  // Returns true if the found file is a directory
  BOOL IsDirectory()
  const
  {
    if (!m_state_filled) { return FALSE; } // TODO: Check should we throw exception?
    return((m_found_state->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
  }

  virtual BOOL IsDots()
  const
  {
    if (!m_state_filled) { return FALSE; } // TODO: Check should we throw exception?
    if ((m_found_state->cFileName[0] == '.') && (m_found_state->cFileName[1] == 0)) { return TRUE; } // Avoid string constants in inlines
    if ((m_found_state->cFileName[0] == '.') && (m_found_state->cFileName[1] == '.') && (m_found_state->cFileName[2] == 0)) { return TRUE; } // Avoid string constants in inlines
    return(FALSE);
  }

  // Returns file length in bytes.
  ULONGLONG GetLength()
  const
  {
    if (!m_state_filled) { return 0; } // TODO: Check should we throw exception?
    ULONGLONG result = m_found_state->nFileSizeHigh; 
	result <<= (sizeof(ULONG)*8);
	result |= m_found_state->nFileSizeLow;

    return result;
  }

  virtual CString GetFilePath() const
  {
    if (!m_state_filled) { return CString(); } // TODO: Check should we throw exception?

    CString result(m_search_root);

    result.Append(m_found_state->cFileName);

    return result;
  }

  private :

  static bool CStringEndsWithChar(const CString &str, TCHAR ch)
  {
    int len = str.GetLength();

    return len > 0 && str.GetAt(len - 1) == ch;
  }

  // returns string allocated by malloc
  static char *BuildAbsoluteSearchPath(const char *pstrName)
  {
    char *pstrCopy = strdup(pstrName);

    if (pstrCopy == NULL)
    {
      return NULL;
    }

    char *pathName = dirname(pstrCopy);
    char *absName  = realpath(pathName,NULL);

    free(pstrCopy);

    return absName;
  }

  #ifdef UNICODE
  bool BuildSearchRootDir (LPCWSTR pstrName)
  {
    bool  ok      = false;
    char *absName = NULL;

    try
    {
      std::string pstrNameMb = NanoWin::StrConverter::Convert(pstrName);

      absName = BuildAbsoluteSearchPath(pstrNameMb.c_str());

      if (absName != NULL)
      {
        std::wstring absNameW = NanoWin::StrConverter::Convert(absName);

        m_search_root.SetString(absNameW.c_str());

        if (CStringEndsWithChar(m_search_root, DIR_SEPARATOR_CHAR) || CStringEndsWithChar(m_search_root, DIR_SEPARATOR_ALT_CHAR))
        {
          // has slash already
        }
        else
        {
          m_search_root.AppendChar(DIR_SEPARATOR_CHAR);
        }

        ok = true;
      }
    }
    catch (...)
    {
    }

    free(absName);

    return ok;
  }
  #else
  bool BuildSearchRootDir (LPCSTR pstrName)
  {
    bool  ok      = false;
    char *absName = NULL;

    absName = BuildAbsoluteSearchPath(pstrName);

    if (absName != NULL)
    {
      m_search_root.SetString(absName);

      if (CStringEndsWithChar(m_search_root, DIR_SEPARATOR_CHAR) || CStringEndsWithChar(m_search_root, DIR_SEPARATOR_ALT_CHAR))
      {
        // has slash already
      }
      else
      {
        m_search_root.AppendChar(DIR_SEPARATOR_CHAR);
      }

      ok = true;
    }

    free(absName);

    return ok;
  }
  #endif

  private :

  static const TCHAR   WILDCARD_ALL_FILES[];
  static const TCHAR   DIR_SEPARATOR_CHAR     = NW_DIRECTORY_SEPARATOR_CHAR;
  static const TCHAR   DIR_SEPARATOR_ALT_CHAR = NW_DIRECTORY_SEPARATOR_ALT_CHAR;
};

// MFC Files
// ------------------------------------

class CFile : public CObject
{
  public:

  enum OpenFlags
  {
    modeRead         = (int)0x00000, // +
    modeWrite        = (int)0x00001, // +
    modeReadWrite    = (int)0x00002,

    shareCompat      = (int)0x00000,
    shareExclusive   = (int)0x00010,
    shareDenyWrite   = (int)0x00020,
    shareDenyRead    = (int)0x00030,
    shareDenyNone    = (int)0x00040, // +

//  modeNoInherit    = (int)0x00080,

    #if defined(UNICODE) || defined(_UNICODE)
    typeUnicode      = (int)0x00400, // used in derived classes (e.g. CStdioFile) only // +
    #endif

    modeCreate       = (int)0x01000, // +
    modeNoTruncate   = (int)0x02000,
    typeText         = (int)0x04000, // used in derived classes (e.g. CStdioFile) only // +
    typeBinary       = (int)0x08000, // used in derived classes (e.g. CStdioFile) only
//  osNoBuffer       = (int)0x10000,
//	osWriteThrough   = (int)0x20000,
//	osRandomAccess   = (int)0x40000,
//	osSequentialScan = (int)0x80000,
  };

  enum From
  {
	  begin   = (int)FILE_BEGIN,
	  current = (int)FILE_CURRENT,
	  end     = (int)FILE_END,
  };

  public:
  HANDLE                  m_hFile;
  BOOL                    m_bCloseOnDelete; // If open with external handle, do not close me on Delete
  CAtlTransactionManager* m_pTM; // For compatibility only, not used actually

  static const HANDLE     hFileNull; // = NULL;

  protected:

  BOOL IsOpen()
  {
    return(m_hFile != hFileNull);
  }

  void InitAsClosed()
  {
    m_hFile = INVALID_HANDLE_VALUE;
    m_bCloseOnDelete = FALSE;
	m_pTM = NULL;
  }

  public:

                     CFile (); // nothrow()
                     CFile (CAtlTransactionManager* pTM); // nothrow()
                     CFile (HANDLE hFile); // nothrow()
                     CFile (LPCSTR lpszFileName,  UINT nOpenFlags); // throw CFileException()
                     CFile (LPCSTR lpszFileName,  UINT nOpenFlags, CAtlTransactionManager* pTM); // throw CFileException()
                     CFile (LPCWSTR lpszFileName,  UINT nOpenFlags); // throw CFileException()
                     CFile (LPCWSTR lpszFileName,  UINT nOpenFlags, CAtlTransactionManager* pTM); // throw CFileException()
  virtual           ~CFile ();

  virtual BOOL       Open(LPCSTR lpszFileName, UINT nOpenFlags, CFileException* pError = NULL); // nothrow()
  virtual BOOL       Open(LPCSTR lpszFileName, UINT nOpenFlags, CAtlTransactionManager* pTM, CFileException* pError = NULL);  // nothrow()
  virtual BOOL       Open(LPCWSTR lpszFileName, UINT nOpenFlags, CFileException* pError = NULL); // nothrow()
  virtual BOOL       Open(LPCWSTR lpszFileName, UINT nOpenFlags, CAtlTransactionManager* pTM, CFileException* pError = NULL);  // nothrow()
  virtual void       Close(); // throw CFileException()
  virtual void       Abort(); // Close if open, // nothrow()
  virtual void       Flush(); // throw CFileException()

  // Action methods
  // All may throw CFileException() to signal error condition

  virtual UINT       Read  (void* lpBuf, UINT nCount);
  virtual void       Write (const void* lpBuf, UINT nCount);

  virtual ULONGLONG  Seek  (LONGLONG lOff, UINT nFrom);

  void               SeekToBegin();
  ULONGLONG          SeekToEnd();

  virtual ULONGLONG  GetLength() const;
};

class CStdioFile : public CFile
{
  public:

  FILE              *m_pStream;

                     CStdioFile   (); // nothrow()
                     CStdioFile   (CAtlTransactionManager* pTM); // nothrow()
                     CStdioFile   (FILE *stream); // nothrow()
                     CStdioFile   (LPCSTR lpszFileName, UINT nOpenFlags); // throw CFileException()
                     CStdioFile   (LPCSTR lpszFileName, UINT nOpenFlags, CAtlTransactionManager* pTM); // throw CFileException()
                     CStdioFile   (LPCWSTR lpszFileName, UINT nOpenFlags); // throw CFileException()
                     CStdioFile   (LPCWSTR lpszFileName, UINT nOpenFlags, CAtlTransactionManager* pTM); // throw CFileException()

  virtual BOOL       Open         (LPCSTR lpszFileName, UINT nOpenFlags, CFileException* pError = NULL); // nothrow()
  virtual BOOL       Open         (LPCSTR lpszFileName, UINT nOpenFlags, CAtlTransactionManager* pTM, CFileException* pError = NULL);  // nothrow()
  virtual BOOL       Open         (LPCWSTR lpszFileName, UINT nOpenFlags, CFileException* pError = NULL); // nothrow()
  virtual BOOL       Open         (LPCWSTR lpszFileName, UINT nOpenFlags, CAtlTransactionManager* pTM, CFileException* pError = NULL);  // nothrow()

  virtual void       Close(); // throw CFileException()
  virtual void       Abort(); // Close if open, // nothrow()
  virtual void       Flush(); // throw CFileException()

  // Action methods
  // All may throw CFileException() to signal error condition

  virtual void       WriteString  (LPCSTR lpsz);
  virtual void       WriteString  (LPCWSTR lpsz);

  virtual LPSTR      ReadString   (_Out_writes_z_(nMax) LPSTR lpsz, _In_ UINT nMax);
  virtual LPWSTR     ReadString   (_Out_writes_z_(nMax) LPWSTR lpsz, _In_ UINT nMax);
  virtual BOOL       ReadString   (CStringA& rString);
  virtual BOOL       ReadString   (CStringW& rString);

  private:

  LPWSTR             ReadToWideString (_Out_writes_z_(nMax) LPWSTR lpsz, _In_ UINT nMax);
  BOOL               ReadToWideChar   (WCHAR *wch);
  LPSTR              ReadToMbString   (_Out_writes_z_(nMax) LPSTR  lpsz, _In_ UINT nMax);
};

#endif // linux
#endif // ...Included
