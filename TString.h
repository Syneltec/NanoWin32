#ifndef __TSTRING_H__
#define __TSTRING_H__

#include "TBuffer.h"

/** \ingroup strings
    \brief Class representing a string abstraction */
class cString : public virtual cGenericBufferNullTerminated
{
public:
  /** Define comportment with operator = and LPCSTR type*/
  cString & operator = (LPCSTR aValue);

  /** Define comportment with operator = and tGenericArray type*/
  cString & operator = (const tGenericArray<char> & aValue);

  /** Parse a string and convert it to boolean */
  static bool ParseBool(LPCSTR aValue);
};


/** \cond FALSE */
// Trick in type definition to ensure code completion under VS6
struct __cExternalStringBaseClass
{
  typedef tExternalArrayAndCount<DWORD,char> type;
};
/** \endcond  */

/** \ingroup strings
    \brief Class representing a external string 
    
    This kind of string can be used to manipulate or strings out of the object itself, like 
    shared memories */
class cExternalString : public __cExternalStringBaseClass::type, virtual public cString
{
protected:
  DWORD mCount;

public:
  typedef __cExternalStringBaseClass::type cParentExternalArray ;

  cExternalString();

  /** \brief Constructor
      \param [in] aString Initial string value
      \param [in] aCapacity Define max capacity(Optional). If it's not defined, the capacity will be automatically set by aString size
  */
  cExternalString(LPSTR aString, DWORD aCapacity=0);

  /** \brief Set the internal buffer of the string */
  void SetExternal(LPSTR aString, DWORD aCapacity=0);

protected:
  virtual void CheckCapacity(int newCount)
  {
    cParentExternalArray::CheckCapacity(newCount);
  }

  void CheckIndex(int anIndex, bool acceptOneMore=false) const
  {
    cParentExternalArray::CheckIndex(anIndex,acceptOneMore);
  }

  virtual void InternalSetCount(int aCount)
  {
    cParentExternalArray::InternalSetCount(aCount);
    OnInternalSetCountDone(aCount);
  }

  virtual char * GetItemPointer(int anIndex)
  {
    return cParentExternalArray::GetItemPointer(anIndex);
  }

  virtual const char * GetItemPointer(int anIndex) const
  {
    return cParentExternalArray::GetItemPointer(anIndex);
  }

  virtual int GetReservedCount() const
  {
    return cString::GetReservedCount();
  }

  virtual void OnInternalSetCountDone(int aCount)
  {
    cString::OnInternalSetCountDone(aCount);
  }


public:
  /** \brief Return char numbers (without null char)*/
  virtual int GetCount() const
  {
    return cParentExternalArray::GetCount();
  }

  /** \brief Return char max capacity (whith null char)*/
  virtual int GetCapacity() const
  {
    return cParentExternalArray::GetCapacity();
  }

  /** \brief Return free size (Capacity - total char - null char)*/
  virtual int GetAvailableCount() const
  {
    return cParentExternalArray::GetAvailableCount();
  }

  cExternalString & operator = (LPCSTR aValue)
  {
    SetContentFromStr(aValue);
    return *this;
  }

  cExternalString & operator = (const tGenericArray<char> & anArray)
  {
    SetContent(anArray);
    return *this;
  }

  operator LPCSTR () const
  {
    return GetPointer();
  }

};

/** \cond FALSE */
// Trick in type definition to ensure code completion under VS6
template <class SIZE_TYPE, SIZE_TYPE SIZE> struct __tGenericStringBaseClass
{
  typedef tBufferNullTerminated<SIZE_TYPE,SIZE> type;
};
/** \endcond */


/** \ingroup strings
    \brief Class representing a generic string 
    \param SIZE_TYPE The integer type used to represent the length of the string
    \param SIZR The maximal length of the string 
    
    This string is hard realtime compliant: there is no dynamic memory allocation to store the content 
    and this is why the maximal length is given as a template argument.
    
    It would be better if you use direct subclass, like #cUltraSmallString (64), #cSmallString (255), #cMediumString (1000), 
    #cLargeString (5000), #cUltraLargeString (15000) instead of a new template instance of the generic string.*/
template <class SIZE_TYPE, SIZE_TYPE SIZE> class tGenericString : public __tGenericStringBaseClass<SIZE_TYPE,SIZE>::type, virtual public cString
{
public:
  tGenericString(): tBufferNullTerminated<SIZE_TYPE,SIZE>()
  {
  }

  explicit tGenericString(LPCSTR aValue): tBufferNullTerminated<SIZE_TYPE,SIZE>(aValue)
  {
  }

  /* \brief Assignment operator 
  
     The assignment operator is required to ensure the compiler does not perform some implicit 
     declaration of local variable coupled to the copy constructor, for instructions like:
     \code
       cUltraLargeString lStr ;
  	   lStr = "Hello" ;
     \endcode

     This implicit copy constructor call causes an allocation of an additional 16k on the stack 
     for an empty #cUltraLargeString assignment !*/
  tGenericString<SIZE_TYPE,SIZE> & operator = (LPCSTR aValue)
  {
    SetContentFromStr(aValue);
    return *this;
  }

  tGenericString<SIZE_TYPE,SIZE> & operator = (const tGenericArray<char> & anArray)
  {
    SetContent(anArray);
    return *this;
  }

  operator LPCSTR () const
  {
    return cString::operator LPCSTR();
  }

protected:
  virtual void CheckCapacity(int newCount)
  {
    tBufferNullTerminated<SIZE_TYPE, SIZE>::CheckCapacity(newCount);
  }

  void CheckIndex(int anIndex, bool acceptOneMore=false) const
  {
    tBufferNullTerminated<SIZE_TYPE, SIZE>::CheckIndex(anIndex,acceptOneMore);
  }

  virtual void InternalSetCount(int aCount)
  {
    tBufferNullTerminated<SIZE_TYPE, SIZE>::InternalSetCount(aCount);
  }

  virtual char * GetItemPointer(int anIndex)
  {
    return tBufferNullTerminated<SIZE_TYPE, SIZE>::GetItemPointer(anIndex);
  }

  virtual const char * GetItemPointer(int anIndex) const
  {
    return tBufferNullTerminated<SIZE_TYPE, SIZE>::GetItemPointer(anIndex);
  }

  virtual int GetReservedCount() const
  {
    return cString::GetReservedCount();
  }

  virtual void OnInternalSetCountDone(int aCount)
  {
    cString::OnInternalSetCountDone(aCount);
  }


public:
  /** \brief Return char numbers (without null char)*/
  virtual int GetCount() const
  {
    return tBufferNullTerminated<SIZE_TYPE, SIZE>::GetCount();
  }

  /** \brief Return char max capacity (whith null char)*/
  virtual int GetCapacity() const
  {
    return tBufferNullTerminated<SIZE_TYPE, SIZE>::GetCapacity();
  }

  /** \brief Return free size (Capacity - total char - null char)*/
  virtual int GetAvailableCount() const
  {
    return tBufferNullTerminated<SIZE_TYPE, SIZE>::GetAvailableCount();
  }

};

// -----------------------------------------------------------------------------
/** \cond FALSE */
//  Trick in type definition to ensure code completion under VS6
struct __cUltraSmallStringBaseClass
{
  typedef tGenericString<unsigned char, 64>   type ;
};

//  Trick in type definition to ensure code completion under VS6
struct __cSmallStringBaseClass
{
  typedef tGenericString<unsigned char, 255>   type ;
};

//  Trick in type definition to ensure code completion under VS6
struct __cMediumStringBaseClass
{
  typedef tGenericString<unsigned short,1000>  type ;
};

//  Trick in type definition to ensure code completion under VS6
struct __cLargeStringBaseClass
{
  typedef tGenericString<unsigned short,5000>  type ;
};

//  Trick in type definition to ensure code completion under VS6
struct __cUltraLargeStringBaseClass
{
  typedef tGenericString<unsigned short,15000> type ;
};
/** \endcond */

/** \ingroup strings
    \brief 64 characters length #tGenericString */
typedef __cUltraSmallStringBaseClass::type  cUltraSmallString ;
/** \ingroup strings
    \brief 255 characters length #tGenericString */
typedef __cSmallStringBaseClass::type       cSmallString ;
/** \ingroup strings
    \brief 1000 characters length #tGenericString */
typedef __cMediumStringBaseClass::type	    cMediumString ;
/** \ingroup strings
    \brief 5000 characters length #tGenericString */
typedef __cLargeStringBaseClass::type		    cLargeString ;
/** \ingroup strings
    \brief 15000 characters length #tGenericString */
typedef __cUltraLargeStringBaseClass::type  cUltraLargeString ;

// -----------------------------------------------------------------------------
#define DYNAMIC_STRING_BLOCK_SIZE 64
/** \ingroup strings
    \brief Class representing a string that can grows dynamically
    
    This clas can be used to manage string that need to grow indefinitely. As
    soon as an operation requires more space, the internal buffer will be 
    reallocated and the capacity growed up of DYNAMIC_STRING_BLOCK_SIZE.

    The capacity is never reduced, but the buffer is released in the destructor. 
    */
class cDynamicString : virtual public cString, virtual public tDynamicGrowingArray<char,DYNAMIC_STRING_BLOCK_SIZE>
{
protected:
  bool mFixedCapacity ;
  virtual void CheckCapacity(int newCount);  
  virtual void InternalSetCount(int aCount);
  virtual void OnInternalSetCountDone(int aCount);

public:
  cDynamicString();
  cDynamicString(const cDynamicString & aSource);
  explicit cDynamicString(const tGenericArray<char> & aSource);
  explicit cDynamicString(DWORD aPriorCapacity);

  void SetFixedCapacity(int aCapacity);

  virtual int GetReservedCount() const ;
  virtual char * GetItemPointer(int anIndex);
  virtual const char * GetItemPointer(int anIndex) const;

  cDynamicString & operator = (LPCSTR aValue);
  cDynamicString & operator = (const tGenericArray<char> & aValue);

  operator LPCSTR () const ;

  /** \brief Reimplements the cGenericBufferNullTerminated::VFormat to ensure growing */
  virtual cGenericBufferNullTerminated & VFormat(LPCSTR Format, va_list va);

  virtual int GetCount() const;
  virtual int GetCapacity() const;
  virtual int GetAvailableCount() const ;
};

// -----------------------------------------------------------------------------
/** \ingroup strings
    \brief Class representing a generic string tokenizer 
    
    A tokenizer is a useful tool helping to parse the content of a string.*/
class cGenericTokenizer
{
protected:
  const tGenericArray<char> * mString;
  int mCurrent ;

public:
  /** \brief Constructor*/
  cGenericTokenizer();
  /** \brief Define string to decode*/
  void SetInput(const tGenericArray<char> & aString);
  /** \brief Abstraction of Reset  to override*/
  virtual void Reset();

  /** \brief Abstraction of NextToken  to override*/
  virtual bool NextToken(tGenericArray<char> & anOutput) = 0;
};

// -----------------------------------------------------------------------------
#define TOKEN_LIST_SIZE 32
typedef tArray<BYTE, TOKEN_LIST_SIZE,LPCSTR> cTokensList;

/** \ingroup strings
    \brief Class representing a string smart tokenizer
    
    Splits the string according to the following delimiters: special characters, space, 
    number (float accepted). The null char is the end delimiter */
class cStringSmartTokenizer : public cGenericTokenizer
{
protected:
  tArray<WORD,512,char> mKeywordsStorage ;
  cTokensList mKeywords;

  LPCSTR AppendInStorage(LPCSTR aToken);
  char GetChar(int anOffset=0);
  bool Next();
  void Copy(tGenericArray<char> & anOutput);
  void TrimSpaces();
  bool ParseKeyword(tGenericArray<char> & anOutput);
  bool ParseNumber(tGenericArray<char> & anOutput);
  bool ParseIdentifier(tGenericArray<char> & anOutput);
  bool ParseQuoted(char aQuoteChar, tGenericArray<char> & anOutput);
  virtual void PreprocessNextToken();
  void GetKeywordsStartingWith(const cUltraSmallString & aPrefix, cTokensList & aList);

public:
  /** \brief Constructor
  
      Automatically add a list of keywords (#AddKeyword) */
  cStringSmartTokenizer();

  /** \brief Abstraction of NextToken  to override
      \param [out] anOutput Output string
      \return True if a new token found  */
  virtual bool NextToken(tGenericArray<char> & anOutput);

  /** \brief Clear current keywords list*/
  void ClearKeywords();

  /** \brief Set default keywords list*/
  void SetDefaultKeywords();

  /** \brief Add a custom keyword into the list
      By default a list of keywords is already configured by the constructor. If you want add more keywords,
      simply use #AddKeyword. But if you want to have your own list, call #ClearKeywords before the 
      #AddKeyword. */
  cStringSmartTokenizer & AddKeyword(LPCSTR aToken);
};

// -----------------------------------------------------------------------------
/** \ingroup strings
    \brief Class representing a string delimited tokenizer

    It splits the string according to the customizable delimiters in the constructor.
    The null char is the end delimiter */
class cStringDelimitedTokenizer : public cGenericTokenizer
{
protected:
  cSmallString mDelimiters ;
  bool mTrimLeftSpaces ;

public:
  /** \brief Constructor
      \param [in] someDelimiters User define delimiters
      \param [in] trimLeftSpaces Trim space between a delimiter and first character  */
  cStringDelimitedTokenizer(LPCSTR someDelimiters, bool trimLeftSpaces);

  /** \brief Get next token
      \param [out] anOutput Output string
      \return True if a new token found   */
  virtual bool NextToken(tGenericArray<char> & anOutput);
};

// -----------------------------------------------------------------------------
/** \ingroup strings
    \brief Class representing a string parser of parameters and values

    It splits the parameters and value according to the customizable delimiters given
    to the constructor. The null char is the end delimiter. */
class cStringParamValueParser : public cStringSmartTokenizer
{
protected:
  LPCSTR mNameValueSeparator,
         mGlobalSeparator;
  virtual void PreprocessNextToken();
public:
  /** \brief Constructor
      \param [in] aNameValueSeparator eg. Name=Value => Separator"="
      \param [in] aGlobalSeparator  eg. Name0=Value0; Name1=Value1 => Separator";" */
  cStringParamValueParser(LPCSTR aNameValueSeparator, LPCSTR aGlobalSeparator);
  /** \brief Get next parameter and value
      \param [out] aName Output string of parameter name
      \param [out] aValue Output string of parameter value
      \return True if a new instance found  */
  bool Parse(tGenericArray<char> & aName, tGenericArray<char> & aValue);
};

// -----------------------------------------------------------------------------

/** \brief Define the default buffer type used for communication purposes */
typedef cUltraLargeString cCommunicationBuffer ;

// -----------------------------------------------------------------------------
extern LPCSTR kEmpty;
extern LPCSTR kEOL;

extern cString & operator << ( cString & aSource, LPCSTR aValue );
extern cString & operator << ( cString & aSource, int    aValue );
extern cString & operator << ( cString & aSource, DWORD  aValue );
extern cString & operator << ( cString & aSource, float  aValue );
extern cString & operator << ( cString & aSource, double aValue );
// -----------------------------------------------------------------------------

#endif
