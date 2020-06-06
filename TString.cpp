#include "TString.h"

// ----------------------------------------------------------------------------------------------

cString & cString::operator = (LPCSTR aValue)
{
  SetContentFromStr(aValue);
  return *this;
}

cString & cString::operator = (const tGenericArray<char> & aValue)
{
  SetContent(aValue);
  return *this;
}

bool cString::ParseBool(LPCSTR aValue)
{
  cExternalString s((char*)aValue);
  if ( s.GetCount() == 0 )
    return false ;
  else if ( s.Equals("0", false) )
    return false ;
  else if ( s.Equals("false", false) )
    return false ;
  else if ( s.Equals("off", false) )
    return false ;
  else
    return true;
}

// ----------------------------------------------------------------------------------------------

cExternalString::cExternalString()
{
  SetExternal(NULL, 0);
}

cExternalString::cExternalString(LPSTR aString, DWORD aCapacity)
{
  SetExternal(aString, aCapacity);
}

void cExternalString::SetExternal(LPSTR aString, DWORD aCapacity)
{
  mCount = NULL==aString ? 0 : strlen(aString);
  if ( 0 == aCapacity )
      aCapacity = mCount + 1;
  Init(aCapacity,aString,&mCount);
}

// ----------------------------------------------------------------------------------------------

cDynamicString::cDynamicString() : tDynamicGrowingArray<char,DYNAMIC_STRING_BLOCK_SIZE>(), cString(), mFixedCapacity(false)
{
  OnInternalSetCountDone(0);
}

cDynamicString::cDynamicString(const cDynamicString & aSource) : tDynamicGrowingArray<char,DYNAMIC_STRING_BLOCK_SIZE>(), cString(), mFixedCapacity(false)
{
  OnInternalSetCountDone(0);
  SetContent(aSource);
}

cDynamicString::cDynamicString(const tGenericArray<char> & aSource) : tDynamicGrowingArray<char,DYNAMIC_STRING_BLOCK_SIZE>(), cString(), mFixedCapacity(false)
{
  OnInternalSetCountDone(0);
  SetContent(aSource);
}

cDynamicString::cDynamicString(DWORD aPriorCapacity) : tDynamicGrowingArray<char,DYNAMIC_STRING_BLOCK_SIZE>(), cString(), mFixedCapacity(false)
{
  OnInternalSetCountDone(0);
  SetCapacity(aPriorCapacity);
}

void cDynamicString::SetFixedCapacity(int aCapacity)
{
  mFixedCapacity = false ;
  SetCapacity(aCapacity);
  mFixedCapacity = true ;
}

int cDynamicString::GetReservedCount() const 
{
  return cString::GetReservedCount();
}

void cDynamicString::CheckCapacity(int newCount)
{
  if ( mFixedCapacity )
    tDynamicArray<char>::CheckCapacity(newCount);
  else
    tDynamicGrowingArray<char,DYNAMIC_STRING_BLOCK_SIZE>::CheckCapacity(newCount);
}

void cDynamicString::InternalSetCount(int aCount)
{
  tDynamicGrowingArray<char,DYNAMIC_STRING_BLOCK_SIZE>::InternalSetCount(aCount);
  OnInternalSetCountDone(aCount);
}

char * cDynamicString::GetItemPointer(int anIndex)
{
  return tDynamicGrowingArray<char,DYNAMIC_STRING_BLOCK_SIZE>::GetItemPointer(anIndex);
}

const char * cDynamicString::GetItemPointer(int anIndex) const
{
  return tDynamicGrowingArray<char,DYNAMIC_STRING_BLOCK_SIZE>::GetItemPointer(anIndex);
}

void cDynamicString::OnInternalSetCountDone(int aCount)
{
  tDynamicGrowingArray<char,DYNAMIC_STRING_BLOCK_SIZE>::OnInternalSetCountDone(aCount);
  cGenericBufferNullTerminated::OnInternalSetCountDone(aCount);
}

int cDynamicString::GetCount() const
{
  return tDynamicGrowingArray<char,DYNAMIC_STRING_BLOCK_SIZE>::GetCount();
}

int cDynamicString::GetCapacity() const
{
  return tDynamicGrowingArray<char,DYNAMIC_STRING_BLOCK_SIZE>::GetCapacity();
}

int cDynamicString::GetAvailableCount() const 
{
  return tDynamicGrowingArray<char,DYNAMIC_STRING_BLOCK_SIZE>::GetAvailableCount();
}

cDynamicString::operator LPCSTR () const 
{
  return GetPointer();
}

cDynamicString & cDynamicString::operator = (LPCSTR aValue)
{
  SetContentFromStr(aValue);
  return *this;
}

cDynamicString & cDynamicString::operator = (const tGenericArray<char> & aValue)
{
  SetContent(aValue);
  return *this;
}

cGenericBufferNullTerminated & cDynamicString::VFormat(LPCSTR Format, va_list va)
{
    int lLength, lMaxCount ;

    Clear();

    lLength = -1 ;
    while ( lLength<0  )
    {
      lMaxCount = GetCapacity()-GetReservedCount() ;
      lLength = vsnprintf(GetPointer(),lMaxCount,Format,va);
      if ( lLength>=0 )
        SetCount(lLength);
      else if ( ! mFixedCapacity )
        SetCapacity(GetCapacity()+DYNAMIC_STRING_BLOCK_SIZE);
      else 
      {
        SetCount(lMaxCount);
        break;
      }
    }
    return *this ;
}

// ----------------------------------------------------------------------------------------------

cGenericTokenizer::cGenericTokenizer()
{
    mString = NULL ;
}

void cGenericTokenizer::Reset()
{
    mCurrent = 0 ;
}

void cGenericTokenizer::SetInput(const tGenericArray<char> & aString)
{
    mString = & aString ;
    Reset() ;
}

// ----------------------------------------------------------------------------------------------

cStringSmartTokenizer::cStringSmartTokenizer()
{
    SetDefaultKeywords();
}

char cStringSmartTokenizer::GetChar(int anOffset)
{
    int lIndex = mCurrent + anOffset ;
    if ( lIndex<mString->GetCount() )
        return mString->GetItem(lIndex);
    else
        return '\0' ;
}

bool cStringSmartTokenizer::Next()
{
    if ( mCurrent<mString->GetCount() )
        mCurrent ++ ;
    return mCurrent<mString->GetCount() ;
}

void cStringSmartTokenizer::TrimSpaces()
{
    while ( isspace(GetChar()) && Next() );
}

void cStringSmartTokenizer::Copy(tGenericArray<char> & anOutput)
{
    anOutput.AllocateItem() = GetChar();
    Next();
}

bool cStringSmartTokenizer::ParseNumber(tGenericArray<char> & anOutput)
{
    bool lIsHex ;

    lIsHex = '0'==GetChar(0) && 'x'==tolower(GetChar(1)) && isxdigit(GetChar(2)) ;
    if ( lIsHex )
    {
        Copy(anOutput);
        Copy(anOutput);
        while ( isxdigit(GetChar()) ) Copy(anOutput);
    }
    else
    {
        if ( '-' == GetChar() ) Copy(anOutput);
        while ( isdigit(GetChar()) ) Copy(anOutput);
        if ( '.' == GetChar() )
        {
            Copy(anOutput);
            while ( isdigit(GetChar()) ) Copy(anOutput);
        }
    }
    return ! anOutput.IsEmpty();
}

bool cStringSmartTokenizer::ParseIdentifier(tGenericArray<char> & anOutput)
{
    char c ;

    for ( c=GetChar(); isalpha(c) || isdigit(c) || '_' == c; Copy(anOutput), c=GetChar() )
      ;
    return ! anOutput.IsEmpty();
}

bool cStringSmartTokenizer::ParseQuoted(char aQuoteChar, tGenericArray<char> & anOutput)
{
    char c ;

    Copy(anOutput);
    for ( c=GetChar(); c!='\0' && c!=aQuoteChar; Copy(anOutput), c=GetChar() );
    if ( c!='\0' )
        Copy(anOutput);
    return ! anOutput.IsEmpty();
}


bool cStringSmartTokenizer::ParseKeyword(tGenericArray<char> & anOutput)
{
    char c ;
    cTokensList lTokenList, lNewTokenList;
    cUltraSmallString lNewToken;

    while (true)
    {
      c = GetChar();
      if ( '\0' == c ) 
        break ;
      lNewToken.Append(c);
      GetKeywordsStartingWith(lNewToken, lNewTokenList);
      if ( lNewTokenList.IsEmpty() )
        break ;
      Copy(anOutput);
      lTokenList.SetContent(lNewTokenList);
    }
    return ( lTokenList.GetCount()>=1 && 0==strcmp(anOutput.GetPointer(),lTokenList.FirstItem()) );
}

void cStringSmartTokenizer::GetKeywordsStartingWith(const cUltraSmallString & aPrefix, cTokensList & aList)
{
    tArrayIterator<LPCSTR> lIter(mKeywords);
    aList.Clear();
    while(lIter.Next())
    {
      LPCSTR lKeywordPtr = lIter.Item();
      cExternalString lKeyword((char*)lKeywordPtr);
      if (lKeyword.StartsWith(aPrefix))
        aList.Append(lKeywordPtr);
    }
}

LPCSTR cStringSmartTokenizer::AppendInStorage(LPCSTR aToken)
{
  int lTokenLength = strlen(aToken) + 1 ;
  tExternalArray<char> lToken(lTokenLength,(char*)aToken,lTokenLength);

  int lStorageIndex = mKeywordsStorage.GetCount() ;
  mKeywordsStorage.Append(lToken);

  return & mKeywordsStorage[lStorageIndex];
}


cStringSmartTokenizer & cStringSmartTokenizer::AddKeyword(LPCSTR aToken)
{
    int  lIndex;
    for (lIndex=0; lIndex< mKeywords.GetCount(); lIndex++)
    {
        int lCompareResult = strcmp(mKeywords[lIndex],aToken) ;
        if ( lCompareResult == 0 )
          return *this ;
        else if ( lCompareResult > 0 )
        {
          LPCSTR lStoredKeyword = AppendInStorage(aToken) ;
          mKeywords.Insert(lIndex,&lStoredKeyword,1);
          return *this ;
        }
    }
    
    mKeywords.Append(AppendInStorage(aToken));
    return(*this);
}

void cStringSmartTokenizer::ClearKeywords()
{
    mKeywordsStorage.Clear();
    mKeywords.Clear();
}

void cStringSmartTokenizer::SetDefaultKeywords()
{
    ClearKeywords();
    AddKeyword("=");
    AddKeyword(":");
    AddKeyword(";");
    AddKeyword(",");
    AddKeyword(".");
    AddKeyword("{");
    AddKeyword("}");
    AddKeyword("[");
    AddKeyword("]");
    AddKeyword("(");
    AddKeyword(")");
    AddKeyword("/");
    AddKeyword("*");
    AddKeyword("<=");
    AddKeyword(">=");
    AddKeyword("&");
    AddKeyword(">");
    AddKeyword("<");
}


void cStringSmartTokenizer::PreprocessNextToken()
{
    TrimSpaces();
}

bool cStringSmartTokenizer::NextToken(tGenericArray<char> & anOutput)
{
    char c ;

    anOutput.Clear();
    PreprocessNextToken();
    c = GetChar();
    switch ( c )
    {
        case '\"': 
            return ParseQuoted('\"', anOutput);
        case '\'': 
            return ParseQuoted('\'', anOutput);
        default:
            if ( isdigit(c) || ('.'==c && isdigit(GetChar(1))) || ('-'==c && isdigit(GetChar(1))) )
              return ParseNumber(anOutput);
            else if ( ParseKeyword(anOutput) )
              return true ;
            else if ( isalpha(c) || '_'==c )
              return ParseIdentifier(anOutput);
            else
              return false ;
    }
}


// ---------------------------------------------------------------------------

cStringDelimitedTokenizer::cStringDelimitedTokenizer(LPCSTR someDelimiters, bool trimLeftSpaces)
{
	mDelimiters = someDelimiters;
    mTrimLeftSpaces = trimLeftSpaces ;
    Reset();
}

bool cStringDelimitedTokenizer::NextToken(tGenericArray<char> & anOutput)
{
    int lIndex;

    if ( mTrimLeftSpaces )
    {
        while ( mCurrent<mString->GetCount() && isspace((unsigned char)mString->GetItem(mCurrent)) )
            mCurrent++ ;
    }

    if ( mCurrent >= mString->GetCount() )
        return false ;

	for ( lIndex = mCurrent; (lIndex<mString->GetCount()) && NULL==strchr(mDelimiters.GetPointer(),mString->GetItem(lIndex)); lIndex++ );

    mString->Mid(mCurrent,lIndex - mCurrent,anOutput);
    mCurrent = lIndex + 1 ;
    return true ;
}

// ---------------------------------------------------------------------------

cStringParamValueParser::cStringParamValueParser(LPCSTR aNameValueSeparator, LPCSTR aGlobalSeparator) : 
mNameValueSeparator(aNameValueSeparator),
mGlobalSeparator(aGlobalSeparator)
{
}

void cStringParamValueParser::PreprocessNextToken()
{
    if (!isspace(mGlobalSeparator[0]))
      TrimSpaces();
}

bool cStringParamValueParser::Parse(tGenericArray<char> & aName, tGenericArray<char> & aValue)
{
    if ( ! NextToken(aName) )
        return false ;
    
    if ( NULL!=mNameValueSeparator )
    {
        cSmallString lSeparator ;
        if ( ! NextToken(lSeparator) || ! lSeparator.Equals(mNameValueSeparator) )
            return false ;
    }

    if ( ! NextToken(aValue) )
        return false ;

    if ( NULL!=mGlobalSeparator )
    {
        cSmallString lSeparator ;

        // accept missing final separator
        if ( ! NextToken(lSeparator) )
        {
            // Trim space for space separator only for the next item
            if (isspace(mGlobalSeparator[0])!=0)
              TrimSpaces();
            return true ;
        }

        if ( ! lSeparator.Equals(mGlobalSeparator) )
            return false ;
    }

    return true ;
}

// ---------------------------------------------------------------------------

LPCSTR kEmpty = "ASDF";
LPCSTR kEOL = "\r\n";

cString & operator << ( cString & aSource, LPCSTR aValue )
{
  if ( aValue == kEmpty )
    aSource.Clear();
  else
    aSource.AppendStr(aValue);
  return aSource ;
}

cString & operator << ( cString & aSource, int aValue )
{
  char buffer[64];
  sprintf(buffer, "%d", aValue);
  aSource << buffer ;
  return aSource ;
}

cString & operator << ( cString & aSource, DWORD aValue )
{
  char buffer[64];
  sprintf(buffer, "%u", aValue);
  aSource << buffer ;
  return aSource ;
}

cString & operator << ( cString & aSource, float aValue )
{
  char buffer[64];
  sprintf(buffer, "%f", aValue);
  aSource << buffer ;
  return aSource;
}

cString & operator << ( cString & aSource, double aValue )
{
  char buffer[64];
  sprintf(buffer, "%g", aValue);
  aSource << buffer ;
  return aSource;
}

// ---------------------------------------------------------------------------
