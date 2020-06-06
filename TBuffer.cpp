#include "TBuffer.h"

// ---------------------------------------------------------------------------

LPCVOID cGenericBuffer::GetData() const
{
  return & FirstItem();
}

void cGenericBuffer::SetData(LPCVOID aData, DWORD aDataCount)
{
  SetContentFromPointer((LPCSTR)aData,aDataCount);
}

void cGenericBuffer::CopyTo(LPVOID aTarget) const
{
  LPBYTE lTarget ;
  LPCBYTE lSource ;
  int i, lCount ;
  lCount = GetCount() + GetReservedCount() ;
  lTarget = (LPBYTE) aTarget ;
  for ( i=0; i<lCount; i++ )
  {
    lSource = (LPCBYTE) (LPCVOID) GetItemPointer(i);
    lTarget[i] = *lSource ;
  }
}

void cGenericBuffer::CopyFrom(LPCVOID aSource, int aSourceSize )
{
  LPCBYTE lSource ;
  int i ;

  SetCount(aSourceSize);
  lSource = (LPCBYTE) aSource ;
  for ( i=0; i<aSourceSize; i++ )
  {
    *GetItemPointer(i) = lSource[i];
  }
}

// ---------------------------------------------------------------------------

void cGenericBufferNullTerminated::OnInternalSetCountDone(int aCount)
{
  /* always append null char */
  *GetItemPointer(aCount) = '\0';
}

int cGenericBufferNullTerminated::GetReservedCount() const
{
  /* for null char */
  return 1;
}


void cGenericBufferNullTerminated::UpdateCount()
{
  SetCount(strlen(GetPointer()));
}

void cGenericBufferNullTerminated::AppendStr(LPCSTR aValue)
{
  Append(aValue,strlen(aValue));
}

bool cGenericBufferNullTerminated::ExtractBeforeStr(LPCSTR aPattern, bool includesPattern, cGenericBufferNullTerminated & output)
{
  tArray<BYTE,0xFF,char> lPattern;
  if ( aPattern )
  {
    lPattern.SetContentFromPointer(aPattern,strlen(aPattern));
  }
  return ExtractBeforePattern( lPattern, includesPattern, output);
}

bool cGenericBufferNullTerminated::ExtractAfterStr(LPCSTR aPattern, bool includesPattern, cGenericBufferNullTerminated & output)
{
  tArray<BYTE,0xFF,char> lPattern;
  if ( aPattern )
  {
    lPattern.SetContentFromPointer(aPattern,strlen(aPattern));
  }
  return ExtractAfterPattern( lPattern, includesPattern, output);
}

bool cGenericBufferNullTerminated::GetBeforeStr(LPCSTR aPattern, bool includesPattern, cGenericBufferNullTerminated & output) const
{
  int lCount = strlen(aPattern);
  tExternalArray<char> lPattern(lCount+1,(LPSTR)aPattern,lCount) ;

  return GetBeforePattern(lPattern,includesPattern,output);
}

bool cGenericBufferNullTerminated::GetAfterStr(LPCSTR aPattern, bool includesPattern, cGenericBufferNullTerminated & output) const
{
  int lCount = strlen(aPattern);
  tExternalArray<char> lPattern(lCount+1,(LPSTR)aPattern,lCount) ;

  return GetAfterPattern(lPattern,includesPattern,output);
}

void cGenericBufferNullTerminated::SetContentFromStr(LPCSTR value)
{
  if ( value )
    SetContentFromPointer(value,strlen(value));
  else
    Clear();
}

int cGenericBufferNullTerminated::IndexOfStr(int startOffset, LPCSTR data) const
{
  return tGenericArray<char>::IndexOf(startOffset,data,strlen(data));
}

int cGenericBufferNullTerminated::Replace(int startOffset, LPCSTR search, LPCSTR replace, int count) 
{
  int result = 0;
  int searchLen, replaceLen;
  searchLen = strlen(search);
  replaceLen = strlen(replace);
  for(result=0; count!=0; )
  {
    int offset = IndexOfStr(startOffset,search);
    if ( offset < 0 )
      break;
    Remove(offset, searchLen);
    Insert(offset, replace, replaceLen);
    startOffset = offset+1 ;
    if ( count > 0 )
      count--;
    result ++;
  }
  return result;
}


bool cGenericBufferNullTerminated::Contains(LPCSTR aPattern) const
{
  return IndexOfStr(0,aPattern) >= 0 ;
}

void cGenericBufferNullTerminated::Trim()
{
  TrimRight();
  TrimLeft();
}

void cGenericBufferNullTerminated::TrimLeft()
{
  int lCount;

  for (lCount=0; lCount<GetCount() && isspace(GetItem(lCount)); lCount++);
  if ( lCount > 0 )
    Remove(0,lCount);
}

void cGenericBufferNullTerminated::TrimRight()
{
  int lOffset, lCount;

  lOffset = GetCount()-1;
  for (lCount=0; lCount<GetCount() && isspace(GetItem(lOffset - lCount)); lCount++);
  if ( lCount > 0 )
    Remove(GetCount() - lCount,lCount);
}

int cGenericBufferNullTerminated::Length() const
{
  return GetCount();
}

LPCSTR cGenericBufferNullTerminated::GetNullTerminatedString() const
{
  return (LPCSTR) GetData() ;
}

bool cGenericBufferNullTerminated::EndsWith(LPCSTR aPattern) const
{
  LPCSTR lStart ;

  lStart = GetPointer() + GetCount() - strlen(aPattern);
  return 0==strcmp(lStart,aPattern);
}

bool cGenericBufferNullTerminated::StartsWith(LPCSTR aPattern) const
{
  return 0==strncmp(GetPointer(),aPattern,strlen(aPattern));
}

bool cGenericBufferNullTerminated::Equals(LPCSTR aPattern) const
{
  return Equals(aPattern,true);
}

bool cGenericBufferNullTerminated::Equals(LPCSTR aPattern, bool aCaseSensitive) const
{
  return 0 == Compare(aPattern,aCaseSensitive);
}

int cGenericBufferNullTerminated::Compare(LPCSTR aPattern, bool aCaseSensitive) const
{
  int lResult ;
  if ( aCaseSensitive )
    lResult = strcmp(GetPointer(),aPattern);
  else
    lResult = _stricmp(GetPointer(),aPattern);

  // needed to ensure output value
  if ( lResult > 0 )
    return 1 ;
  else if ( lResult < 0 )
    return -1 ;
  else
    return 0;
}

void cGenericBufferNullTerminated::SetCountFromNullChar()
{
  if ( GetCount() > 0 )
  {
    LPCSTR lStartPointer = GetPointer();
    LPCSTR lNullCharPointer = strchr(lStartPointer,'\0');
    if ( NULL == lNullCharPointer )
    {
      SetCount(0);
    }
    else
    {
      SetCount((int)(lNullCharPointer - lStartPointer));
    }
  }
}
cGenericBufferNullTerminated & cGenericBufferNullTerminated::operator = (LPCSTR aValue)
{
  SetContentFromStr(aValue);
  return *this;
}

cGenericBufferNullTerminated & cGenericBufferNullTerminated::operator = (const tGenericArray<char> & aValue)
{
  SetContent(aValue);
  return *this;
}

cGenericBufferNullTerminated & cGenericBufferNullTerminated::Format(LPCSTR Format,...)
{
  va_list va ;

  va_start(va,Format);
  VFormat(Format,va);
  va_end(va);
  return *this ;
}

cGenericBufferNullTerminated & cGenericBufferNullTerminated::VFormat(LPCSTR Format, va_list va)
{
  int lLength, lMaxCount ;

  lMaxCount = GetCapacity()-GetReservedCount() ;
  lLength = vsnprintf(GetPointer(),lMaxCount,Format,va);
  if ( lLength < 0 )
    SetCount(lMaxCount);
  else
    SetCount(lLength);
  return *this ;
}

tArray<BYTE,255,char> cGenericBufferNullTerminated::GlobalFormat(LPCSTR Format,...)
{
  va_list va ;

  va_start(va,Format);
  tArray<BYTE,255,char> lResult = GlobalVFormat(Format,va);
  va_end(va);
  return lResult ;
}

tArray<BYTE,255,char> cGenericBufferNullTerminated::GlobalVFormat(LPCSTR Format, va_list va)
{
  tArray<BYTE,255,char> lResult ;
  int lLength, lMaxCount ;

  lMaxCount = lResult.GetCapacity()-1 ;
  lLength = vsnprintf(lResult.GetPointer(),lMaxCount,Format,va);
  if ( lLength < 0 )
    lResult.SetCount(lMaxCount);
  else
    lResult.SetCount(lLength);
  return lResult ;
}

// ---------------------------------------------------------------------------
