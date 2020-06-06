#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <stdarg.h>
//#include <typeinfo.h>
//#include "MxMacros.h"

// ---------------------------------------------------------------------------

class cException
{
protected:
    char mMessage[512];

public:
    cException();
    
    LPCSTR GetMessage() const ;
    void SetMessage(LPCSTR anExceptionClassName, LPCSTR aFormat, va_list aVa);

};

// ---------------------------------------------------------------------------

class cNotImplementedException : public cException
{
};

// ---------------------------------------------------------------------------
#ifndef USING_VS6
extern LPCSTR gExceptionFunction;
#endif
extern LPCSTR gExceptionFile;
extern DWORD gExceptionLine;

// This routine must NEVER be inlined (RTX2011 weird crash) and the 
// __declspec(noinline) is not supported by VS6. But this option can be removed 
// without problem: the compiler will never inline it because of of the variable 
// argument list!
template<typename EXCEPTION_TYPE> EXCEPTION_TYPE TemplateThrow(LPCSTR aFormat,...)
{
  EXCEPTION_TYPE gException ;
  va_list lArgList;

  va_start(lArgList,aFormat);
  gException.SetMessage(typeid(gException).name(), aFormat,lArgList);
  va_end(lArgList);

  throw gException;

  return gException;
}

#ifndef USING_VS6
# define Throw gExceptionFunction=__FUNCTION__,gExceptionFile=__FILE__,gExceptionLine=__LINE__,TemplateThrow
#else
# define Throw gExceptionFile=__FILE__,gExceptionLine=__LINE__,TemplateThrow
#endif

// ---------------------------------------------------------------------------

#endif
