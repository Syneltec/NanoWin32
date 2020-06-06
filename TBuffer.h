#ifndef __TBUFFER_H__
#define __TBUFFER_H__

#include "TArray.h"

/** \cond FALSE */
// Trick in type definition to ensure code completion under VS6
struct __cGenericBufferBaseClass
{
  typedef tGenericArray<char> type ;
};
    
typedef __cGenericBufferBaseClass::type cGenericBufferBase ;
/** \endcond */

/** \ingroup buffers
    \brief Class representing a generic buffer, based on the #tGenericArray class */
class cGenericBuffer : public virtual tGenericArray<char>
{
public:
  /** \brief Return reference on the first char*/
  LPCVOID GetData() const;

  /** \brief Set data in the buffer*/
  void SetData(LPCVOID aData, DWORD aDataCount);

  /** \brief Copy current buffer in other one*/
  void CopyTo(LPVOID aTarget) const;

  /** \brief Copy an other buffer in the current*/
  void CopyFrom(LPCVOID aSource, int aSourceSize );
};


/** \ingroup buffers
    \brief Class representing a generic null terminated buffer

    It's used by #cString class
*/
class cGenericBufferNullTerminated : public virtual cGenericBuffer
{
protected:
  virtual void OnInternalSetCountDone(int aCount);
  virtual int GetReservedCount() const;

public:
  /** \brief Update count value if the buffer is set outside*/
  void UpdateCount();

  /** \brief Append a string after the current*/
  void AppendStr(LPCSTR aValue);

  /** \brief See tArray::ExtractBeforePattern()*/
  bool ExtractBeforeStr(LPCSTR aPattern, bool includesPattern, cGenericBufferNullTerminated & output);

  /** \brief See tArray::ExtractAfterPattern()*/
  bool ExtractAfterStr(LPCSTR aPattern, bool includesPattern, cGenericBufferNullTerminated & output);

  /** \brief See tArray::GetBeforePattern()*/
  bool GetBeforeStr(LPCSTR aPattern, bool includesPattern, cGenericBufferNullTerminated & output) const;

  /** \brief See tArray::GetAfterPattern()*/
  bool GetAfterStr(LPCSTR aPattern, bool includesPattern, cGenericBufferNullTerminated & output) const;

  /** \brief Redefine string content*/
  void SetContentFromStr(LPCSTR value);

  /** \brief Search position of a string
      \param [in] startOffset Start point
      \param [in] data String to find
      \return Position of the string (-1 = not found)
   */
  int IndexOfStr(int startOffset, LPCSTR data) const;

  /** \brief Replace occurences of a string
      \param [in] startOffset Start point
      \param [in] search String to find
      \param [in] replace String to replace with
      \param [in] count Number of occurences to replace (if <0 then all)
      \return Number of replacements processed
   */
  int Replace(int startOffset, LPCSTR search, LPCSTR replace, int count=-1) ;

  /** \brief Search if a part of string is in the buffer
      \param [in] aPattern The string to search for
      \return Position of the pattern (-1 = not found)
   */
  bool Contains(LPCSTR aPattern) const;

  /** \brief Remove space on the left and right of the buffer*/
  void Trim();

  /** \brief Remove space on the left of the buffer*/
  void TrimLeft();

  /** \brief Remove space on the right of the buffer*/
  void TrimRight();

  /** \brief See tArray::GetCount()*/
  int Length() const;

  /** Returns the pointer on the string content */
  LPCSTR GetNullTerminatedString() const;

  /** \brief Check the end buffer pattern*/
  bool EndsWith(LPCSTR aPattern) const;

  /** \brief Check the start buffer pattern*/
  bool StartsWith(LPCSTR aPattern) const;

  /** \brief Check if the pattern is equals with this buffer (Case sensitive)*/
  bool Equals(LPCSTR aPattern) const;

  /** \brief Check if the pattern is equals with this buffer (Case sensitive customizable)*/
  bool Equals(LPCSTR aPattern, bool aCaseSensitive) const;

  /** \brief Value Relationship of buffer to pattern
      \return < 0 buffer less than pattern
      \return 0 buffer identical to pattern 
      \return > 0 buffer greater than pattern 
   */
  int Compare(LPCSTR aPattern, bool aCaseSensitive) const;

  /** \brief Format string from null char to the end*/
  void SetCountFromNullChar();

  /** \brief Assignment operator with an LPCSTR type*/
  cGenericBufferNullTerminated & operator = (LPCSTR aValue);

  /** \brief Assignment operator with an #tGenericArray type*/
  cGenericBufferNullTerminated & operator = (const tGenericArray<char> & aValue);

  /** \brief Function allias sprintf()*/
  cGenericBufferNullTerminated & Format(LPCSTR Format,...);

  /** \brief Function allias vsprintf()*/
  virtual cGenericBufferNullTerminated & VFormat(LPCSTR Format, va_list va);

public:
  /** \brief Write formatted output to the buffer*/
  static tArray<BYTE,255,char> GlobalFormat(LPCSTR Format,...);

  /** \brief Write formatted output using a pointer to a list of arguments
      For more details, see MSDN help under "_vsnprintf"
   */
  static tArray<BYTE,255,char> GlobalVFormat(LPCSTR Format, va_list va);
};

// ---------------------------------------------------------------------------

/** \cond FALSE */
// Trick in type definition to ensure code completion under VS6
template <class SIZE_TYPE, SIZE_TYPE SIZE> struct __tCharArrayBaseClass
{
  typedef tArray<SIZE_TYPE,SIZE,char> type ;
};
/** \endcond */

/** \ingroup buffers
    \brief Class implementing a character based buffer 

    Reintroduce #tArray class with an array of type "char"
*/
template <class SIZE_TYPE, SIZE_TYPE SIZE> class tBuffer : public virtual cGenericBuffer, public virtual __tCharArrayBaseClass<SIZE_TYPE, SIZE>::type
{
protected:
  virtual void CheckCapacity(int newCount)
  {
    tArray<SIZE_TYPE, SIZE, char>::CheckCapacity(newCount);
  }

  void CheckIndex(int anIndex, bool acceptOneMore=false) const
  {
    tArray<SIZE_TYPE, SIZE, char>::CheckIndex(anIndex,acceptOneMore);
  }

  virtual void InternalSetCount(int aCount)
  {
    tArray<SIZE_TYPE, SIZE, char>::InternalSetCount(aCount);
  }

  virtual char * GetItemPointer(int anIndex)
  {
    return tArray<SIZE_TYPE, SIZE, char>::GetItemPointer(anIndex);
  }

  virtual const char * GetItemPointer(int anIndex) const
  {
    return tArray<SIZE_TYPE, SIZE, char>::GetItemPointer(anIndex);
  }

public:
  /** \brief Constructor*/
  tBuffer()
  {
    InternalSetCount(0);
  }

  /** \brief Implementation of the tGenericArray::GetCount() */
  virtual int GetCount() const
  {
    return tArray<SIZE_TYPE, SIZE, char>::GetCount();
  }

  /** \brief Implementation of the tGenericArray::GetCapacity() */
  virtual int GetCapacity() const
  {
    return tArray<SIZE_TYPE, SIZE, char>::GetCapacity();
  }

  /** \brief Implementation of the tGenericArray::GetAvailableCount() */
  virtual int GetAvailableCount() const
  {
    return tArray<SIZE_TYPE, SIZE, char>::GetAvailableCount();
  }
};


/** \cond FALSE */
// Trick in type definition to ensure code completion under VS6
template <class SIZE_TYPE, SIZE_TYPE SIZE> struct __tBufferNullTerminatedBaseClass
{
  typedef tBuffer<SIZE_TYPE,SIZE> type ;
};
/** \endcond */


/** \ingroup buffers
    \brief Class implementing #cGenericBufferNullTerminated */
template <class SIZE_TYPE, SIZE_TYPE SIZE> class tBufferNullTerminated : public virtual __tBufferNullTerminatedBaseClass<SIZE_TYPE,SIZE>::type, public virtual cGenericBufferNullTerminated
{
public:
  /** \brief Constructor
      \warning Do not call parent constructor, because of redefinition of critical function
  */
  tBufferNullTerminated()
  {
    InternalSetCount(0);
  }

  /** \brief Constructor with an initial value
      \warning Do not call parent constructor, because of redefinition of critical function
  */
  explicit tBufferNullTerminated(LPCSTR aValue)
  {
    InternalSetCount(0);
    if ( aValue )
    {
      SetContentFromPointer(aValue,(int) strlen(aValue));
    }
  }

  /** \brief Constructor with an initial value
      \warning Do not call parent constructor, because of redefinition of critical function
  */
  explicit tBufferNullTerminated(LPCVOID aValue, SIZE_TYPE aValueLength)
  // do not call parent constructor, because of redefinition of critical function
  {
    InternalSetCount(0);
    SetContentFromPointer((LPCSTR)aValue,aValueLength);
  }

  /** \brief Assignment operator with an LPCSTR type (ReadOnly)*/
  operator LPCSTR () const
  {
    return cGenericBufferNullTerminated::operator LPCSTR();
  }

  /** \brief Assignment operator with an LPCSTR type (ReadOnly)*/
  tBufferNullTerminated<SIZE_TYPE,SIZE> & operator = (LPCSTR aValue)
  {
    SetContentFromStr(aValue);
    return *this;
  }


protected:
  virtual void CheckCapacity(int newCount)
  {
    tArray<SIZE_TYPE, SIZE, char>::CheckCapacity(newCount);
  }

  void CheckIndex(int anIndex, bool acceptOneMore=false) const
  {
    tArray<SIZE_TYPE, SIZE, char>::CheckIndex(anIndex,acceptOneMore);
  }

  virtual void InternalSetCount(int aCount)
  {
    tArray<SIZE_TYPE, SIZE, char>::InternalSetCount(aCount);
  }

  virtual char * GetItemPointer(int anIndex)
  {
    return tArray<SIZE_TYPE, SIZE, char>::GetItemPointer(anIndex);
  }

  virtual const char * GetItemPointer(int anIndex) const
  {
    return tArray<SIZE_TYPE, SIZE, char>::GetItemPointer(anIndex);
  }

  virtual int GetReservedCount() const
  {
    return cGenericBufferNullTerminated::GetReservedCount();
  }

  virtual void OnInternalSetCountDone(int aCount)
  {
    cGenericBufferNullTerminated::OnInternalSetCountDone(aCount);
  }


public:
  /** \brief Return char numbers (without null char)*/
  virtual int GetCount() const
  {
    return tArray<SIZE_TYPE, SIZE, char>::GetCount();
  }

  /** \brief Return char max capacity*/
  virtual int GetCapacity() const
  {
    return tArray<SIZE_TYPE, SIZE, char>::GetCapacity();
  }

  /** \brief Get free size*/
  virtual int GetAvailableCount() const
  {
    return tArray<SIZE_TYPE, SIZE, char>::GetAvailableCount();
  }

};

/** \cond FALSE */
// Trick in type definition to ensure code completion under VS6
struct __cBufferBaseClass
{
  typedef tBufferNullTerminated<WORD,512> type ;
};
/** \endcond*/

/** \ingroup buffers
    \brief Default buffer type (size of 512 bytes) */
typedef __cBufferBaseClass::type cBuffer ;

#endif
