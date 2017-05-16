// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MFCAfx functions
// Micro stubs to allow code just to compile for command line run (no UI)

#if !defined(NanoWinMFCAfxIncluded)
#define NanoWinMFCAfxIncluded

#include "NanoWinTypes.h"
#include "NanoWinTCHAR.h"

#if defined LINUX

// Debug primitives (subset)
// --------------------------------------

#include <assert.h>

// "void" ASSERT(f)      evaluate f only on debug, fails if f is false (do not evaluates on release)
// "void" VERIFY(f)      evaluate f always, fail if false on debug (ignoring result on release)
// "void" DEBUG_ONLY(f)  evaluate f only on debug (result is not tested)

#define ASSERT(f)            assert(f)
#if defined(_DEBUG) && !defined(NDEBUG)
#define DEBUG_ONLY(f)        (f)
#define VERIFY(f)            ASSERT(f)
#else
#define DEBUG_ONLY(f)        ((void)0)
#define VERIFY(f)            ((void)(f))
#endif

// Basic CObject (subset)
// --------------------------------------

class CObject
{
  public:
  virtual ~CObject() = 0;  // Virtual destructors are manadatory to all decendants

  protected:
  CObject() { }; // You cannot instaniate CObject directly

  private:
  CObject(const CObject& objectSrc);              // You cannot copy, unless you do it explicitely // no implementation (so would produce link error)
  void operator=(const CObject& objectSrc);       // You cannot copy, unless you do it explicitely // no implementation (so would produce link error)
};

#define DECLARE_DYNAMIC(c) // Nothing as for now

// Basic CException (subset)
// --------------------------------------

class CException : public CObject
{
  // Abstract
  public:

  // Override this to report errors
  virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL) const;

  // Used to just call non-cost version? // No this hook in this implemenation
  // virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL);

  // Reports exception (somehow to stderror) [call-only]
  // Prototype differ from MFC, because no agrs are supported here, original prototype:
  // virtual int ReportError(UINT nType = MB_OK, UINT nMessageID = 0);
  // Stubbed ReportError() uses no params and is not virtual since override is not supported here
  int  ReportError  ();

  // Abstract since there is no destructor declared here
};

class CSimpleException : public CException
{
  // Just intermidiate class here
  // Abstract since there is no destructor declared here
};

// Some usefull exceptions
// --------------------------------------

class CMemoryException : public CSimpleException
{
  public:
  virtual ~CMemoryException() { }

  virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL) const;
};

class CNotSupportedException : public CSimpleException
{
  public:
  virtual ~CNotSupportedException() { }

  virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL) const;
};

class CInvalidArgException : public CSimpleException
{
  public:
  virtual ~CInvalidArgException() { }

  virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL) const;
};

class CFileException : public CSimpleException
{
  public:
  virtual ~CFileException() { }

  virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL) const;
};

#endif // linux
#endif // ...Included