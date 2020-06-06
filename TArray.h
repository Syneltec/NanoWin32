#ifndef __TARRAY_H__
#define __TARRAY_H__

#include "windows.h"
#include "TIterator.h"
#include "Exception.h"

/** \ingroup collections 
    \brief Define an iteration direction */
typedef enum
{
  eidNormal,
  eidReverse
} tArrayBrowseDirection ;

/** \ingroup collections 
    \brief Class representing a buffer exception */
class cArrayException : public cException
{
public:
  static void ThrowIndexOutOfBound(int index, int maxSize);
  static void ThrowArrayToSmall(int requestedSize, int maxSize);
  static void ThrowNotEmpty();
};

/** \ingroup collections 
    \brief Define an array part */
struct tArrayPartRef
{
  /** \brief Start index of the array part */
  int Start;
  /** \brief Number of elements of the array part */
  int Count;

  tArrayPartRef()
  {
    Start = 0 ;
    Count = 0 ;
  }
  tArrayPartRef(int aStart, int aCount)
  {
    Start = aStart ;
    Count = aCount ;
  }

  tArrayPartRef(const tArrayPartRef & src)
  {
    Start = src.Start ;
    Count = src.Count ;
  }
};

/** \ingroup collections 
    \brief Class representing a generic array
    \param TYPE Data type of each array item
 */
template <class TYPE> class tGenericArray
{
protected:
  virtual void CheckCapacity(int newCount)
  {
    if ( (newCount+GetReservedCount())>GetCapacity() || newCount<0 )
      cArrayException::ThrowArrayToSmall(newCount+GetReservedCount(),GetCapacity());
  }
  void CheckIndex(int anIndex, bool acceptOneMore=false) const
  {
    int overSize ;
    overSize = acceptOneMore ? 1 : 0 ;
    if ( anIndex>=(GetCount()+overSize) || anIndex<0 )
      cArrayException::ThrowIndexOutOfBound(anIndex,GetCapacity());
  }

  virtual void OnInternalSetCountDone(int aCount)
  {
  }

  virtual void InternalSetCount(int aCount)
  {
    CheckCapacity(aCount);
    // complete the job
  }

  virtual TYPE * GetItemPointer(int anIndex) = 0 ;
  virtual const TYPE * GetItemPointer(int anIndex) const = 0 ;

public:
  int DebugCount() const ;
  int DebugCapacity() const ;
  const TYPE * DebugPointer() const ;

public:
  /** \brief Get the number of element in the array */
  virtual int GetCount() const = 0 ;

  /** \brief Get the capacity of the array */
  virtual int GetCapacity() const = 0 ;

  /** \brief Get the free items in the array before reaching its capacity */
  int GetAvailableCount() const
  {
    return GetCapacity() - GetCount() - GetReservedCount();
  }

  /** \brief Get the number of items to reserve at the end of the array

      Basically, this routine always returns 0. But for strings it will return 1 to ensure
      a position for the null character.*/
  virtual int GetReservedCount() const
  {
    return 0 ;
  }

public:
  tGenericArray()
  {
    // Trick to ensure routine are always instanciated in template and not removed by linker optimization
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
      int (tGenericArray<TYPE>::*lDebugCount)   () const ;
      int (tGenericArray<TYPE>::*lDebugCapacity)() const ;
      const TYPE * (tGenericArray<TYPE>::*lDebugPointer)() const ;
    #pragma GCC diagnostic pop

    lDebugCount    = & tGenericArray<TYPE>::DebugCount;
    lDebugCapacity = & tGenericArray<TYPE>::DebugCapacity;
    lDebugPointer  = & tGenericArray<TYPE>::DebugPointer ;
  }

  virtual ~tGenericArray()
  {
  }


  /** \brief Resize the array (if needed!)
      \post GetCount() == aCount */
  void SetCount(int aCount)
  {
    if ( aCount != (int)GetCount() )
    {
      InternalSetCount(aCount);
    }
  }

  /** \brief Resize the array to its maximal size, defined by its capacity
      \post GetCount() == GetCapacity() - GetReserved() */
  void SetCountToMax()
  {
    SetCount( GetCapacity() - GetReservedCount() );
  }
  /** \brief Ensure the array has a minimal size

      If the array is bigger, the size is not reduced. But if the array is smaller, then the
      size is increased.

      \param aCount Minimal size to ensure
      \post GetCount()>=aCount */
  void EnsureMinimalCount(int aCount)
  {
    if ( aCount > (int)GetCount() )
    {
      SetCount(aCount);
    }
  }

  /** \brief Clear array
      \post GetCount()==0 */
  void Clear()
  {
    SetCount(0);
  }

  // ------------------------------------------------------------------------

  /** \brief Get item value on an index*/
  const TYPE & GetItem(int Index) const
  {
    CheckIndex(Index,false);
    return *GetItemPointer(Index) ;
  }

  TYPE & GetItem(int Index)
  {
    CheckIndex(Index,false);
    return *GetItemPointer(Index) ;
  }

  /** \brief Get the first item of the array*/
  TYPE & FirstItem()
  {
    return GetItem(0);
  }

  /** \brief See FirstItem() */
  const TYPE & FirstItem() const
  {
    return GetItem(0);
  }

  /** \brief Get the last item of the array*/
  TYPE & LastItem()
  {
    return GetItem(GetCount()-1);
  }

  /** \brief See LastItem() */
  const TYPE & LastItem() const
  {
    return GetItem(GetCount()-1);
  }

  // ------------------------------------------------------------------------

  /** \brief Return true if the array is empty */
  bool IsEmpty() const
  {
    return 0 == GetCount();
  }

  /** \brief Return true if the array has no free space */
  bool IsFull() const
  {
    return 0 == GetAvailableCount();
  }

  // ------------------------------------------------------------------------

  /** \brief Set content from a pointer

      Each element in the given C array will be copied into the current array.

      \param [in] data Start pointer
      \param [in] dataSize items numbers
  */
  void SetContentFromPointer(const TYPE * data, int dataSize)
  {
    SetCount(dataSize);
    for (int i=0; i<dataSize; i++ )
    {
      *GetItemPointer(i) = data[i] ;
    }
  }
  /** \brief Set content from another array
      \param [in] anArray Reference on an external array
  */
  void SetContent(const tGenericArray<TYPE> & anArray)
  {
    int i ;
    SetCount(anArray.GetCount());
    for(i=0; i<anArray.GetCount(); i++ )
    {
      *GetItemPointer(i) = *anArray.GetItemPointer(i);
    }
  }

  // ------------------------------------------------------------------------
protected:
  void PrepareInsert(int offset, int dataSize)
  {
    int i, shiftCount ;

    CheckIndex(offset,true);
    shiftCount = GetCount() - offset ;
    SetCount(GetCount() + dataSize);

    // move existing content to end of array
    for (i=shiftCount-1; i>=0; i-- )
    {
      *GetItemPointer(offset+dataSize+i) = *GetItemPointer(offset+i);
    }
  }

public:
  /** \brief Insert an external full array
      \param [in] offset Insert offset
      \param [in] anArray External array pointer
  */
  void Insert(int offset, const tGenericArray<TYPE> & anArray)
  {
    int i ;
    PrepareInsert(offset, anArray.GetCount());
    for ( i=0; i<anArray.GetCount(); i++ )
    {
      *GetItemPointer(offset+i) = anArray[i];
    }
  }
  /** \brief Insert some data of the same type in the array
      \param [in] offset Insert offset
      \param [in] data pointer
      \param [in] dataSize Items numbers
  */
  void Insert(int offset, const TYPE * data, int dataSize)
  {
    int i ;

    PrepareInsert(offset, dataSize);
    for (i=0; i<dataSize; i++ )
    {
      *GetItemPointer(offset+i) = data[i];
    }
  }

  // ------------------------------------------------------------------------
  /** \brief Allocate an item at the end of the array and return a reference on it */
  TYPE & AllocateItem()
  {
    int lIndex = GetCount();
    SetCount(lIndex+1);
    return GetItem(lIndex);
  }

  /** \brief Append an item at the end of the current array
      \param [in] data The item to append at the end (value is copied)*/
  void Append(const TYPE & data)
  {
    AllocateItem() = data ;
  }

  /** \brief Append the content of an array at the end of the current array
      \param [in] anArray The array to append at the end (values are copied)*/
  void Append(const tGenericArray<TYPE> & anArray)
  {
    int i, lOffset ;

    lOffset = GetCount();
    SetCount(lOffset+anArray.GetCount());
    for (i=0; i<anArray.GetCount(); i++ )
    {
      *GetItemPointer(lOffset+i) = anArray[i];
    }
  }

  /** \brief Append the content of a C array at the end of the current array
      \param [in] data Pointer on the C array (values are copied)
      \param [in] dataSize Number of elements in the C array */
  void Append(const TYPE * data, int dataSize)
  {
    Insert(GetCount(),data,dataSize);
  }

  // ------------------------------------------------------------------------
  /** \brief Remove items at a given position

      The items after the range defined by the arguments will be moved to remove hole created by the remove operation.
      \param [in] offset The index of the first element to remove
      \param [in] count The number of items to remove
   */
  void Remove(int offset, int count=1)
  {
    RemoveRef(tArrayPartRef(offset,count));
  }

  /** \brief Remove items defined by a #tArrayPartRef (see Remove()) */
  virtual void RemoveRef(const tArrayPartRef aRef)
  {
    int i ;

    if ( aRef.Count == 0 )
    {
      CheckIndex(aRef.Start,true);
      return;
    }
    else
    {
      CheckIndex(aRef.Start,false);
      CheckIndex(aRef.Start+aRef.Count,true);
    }

    // move existing content to end of array
    for (i=0; i<(GetCount()-aRef.Start-aRef.Count); i++ )
    {
      *GetItemPointer(aRef.Start+i) = *GetItemPointer(aRef.Start+aRef.Count+i);
    }

    SetCount(GetCount() - aRef.Count);
  }

  // ------------------------------------------------------------------------
  /** \brief Read a part of the current array

      Copies a part of the current array into an external C array.
      \param offset [in] First element to copy
      \param data [in,out] C array to fill
      \param dataSize [in] number of element to copy into the C array */
  void Read(int offset, TYPE * data, int dataSize)
  {
    int i ;
    CheckIndex(offset,false);
    CheckIndex(offset+dataSize,true);

    for (i=0; i<dataSize; i++ )
    {
      data[i] = *GetItemPointer(offset+i);
    }
  }
  /** \brief Write data from an external C array

      Copies the content of the external C array into the current array, at position defined by offset.
      If needed, the size of the array is increased

      \param offset [in] Position of the first element to write
      \param data [in] C array to copy from
      \param dataSize [in] Number of element to copy from the C array */
  void Write(int offset, const TYPE * data, int dataSize)
  {
    int i ;

    CheckIndex(offset,true);
    EnsureMinimalCount(offset+dataSize);

    for (i=0; i<dataSize; i++ )
    {
      *GetItemPointer(offset+i) = data[i] ;
    }
  }

  // ------------------------------------------------------------------------
  /** \brief Search a pattern in the array

      Search for the next occurence of a complete pattern into the current array.

      \param [in] direction Direction of the search
      \param [in] startOffset Where starts the search
      \param [in] data C array containing the pattern to search for
      \param [in] dataSize Size of the pattern to search for
      \param [out] aResult Position of the found occurence
      \return True when the pattern has been found
  */
  bool Search(tArrayBrowseDirection direction, int startOffset, const TYPE * data, int dataSize, tArrayPartRef & aResult) const;

  /** \brief See Search()*/
  bool Search(tArrayBrowseDirection direction, int startOffset, const tGenericArray<TYPE> & aPattern, tArrayPartRef & aResult) const
  {
    int lArraySize, lPatternSize, loopInitValue, loopConditionValue, loopIncrement ;
    int i, j;

    lPatternSize = aPattern.GetCount();
    if ( lPatternSize > 0 )
    {
      lArraySize = GetCount() - startOffset ;

      switch (direction)
      {
        case eidNormal:
          loopConditionValue = lArraySize-lPatternSize;
          loopInitValue = 0 ;
          loopIncrement = 1 ;
          break;
        case eidReverse:
          loopConditionValue = 0;
          loopInitValue = lArraySize-lPatternSize;
          loopIncrement = -1 ;
          break;
      }

      for (i=loopInitValue; (eidNormal==direction) ? (i<=loopConditionValue) : (i>=loopConditionValue); i=i+loopIncrement )
      {
        bool isMatching = true ;
        for (j=0; j<aPattern.GetCount(); j++ )
        {
          if ( *GetItemPointer(startOffset+i+j) != aPattern[j] )
          {
            isMatching = false ;
            break ;
          }
        }
        if ( isMatching )
        {
          aResult.Start = startOffset + i ;
          aResult.Count = lPatternSize ;
          return true ;
        }
      }
    }
    return false ;
  }
  /** \brief See Search()*/
  bool Search(int startOffset, const TYPE * data, int dataSize, tArrayPartRef & aResult) const
  {
    return Search(eidNormal, startOffset, data, dataSize, aResult );
  }
  /** \brief See Search()*/
  bool Search(int startOffset, const tGenericArray<TYPE> & aPattern, tArrayPartRef & aResult) const
  {
    return Search(eidNormal, startOffset, aPattern, aResult );
  }

  // ------------------------------------------------------------------------
  /** \brief Search the first occurence of a pattern

      \param [in] startOffset Search start position
      \param [in] data C array containing the pattern to search for
      \param [in] dataSize Size of the pattern to search for
      \return Position of the first item of found occurence or -1 when not found
  */
  int IndexOf(int startOffset, const TYPE * data, int dataSize) const
  {
    tArrayPartRef lResult ;
    if ( Search(startOffset,data,dataSize,lResult) )
      return lResult.Start ;
    else
      return -1 ;
  }
  /** \brief See IndexOf()*/
  int IndexOf(int startOffset, const tGenericArray<TYPE> & aPattern) const
  {
    tArrayPartRef lResult ;
    if ( Search(startOffset,aPattern,lResult) )
      return lResult.Start ;
    else
      return -1 ;
  }
  /** \brief Same as IndexOf() but for a unique item */
  int IndexOfItem(int startOffset, const TYPE & data) const
  {
    return IndexOf(startOffset,&data,1);
  }

  /** \brief Get the position of the last occurence of an item
      \return The position of the occurence or -1 when not found */
  int LastIndexOfItem(const TYPE & data) const
  {
    tArrayPartRef lResult ;
    if ( Search(eidReverse,0,&data,1,lResult) )
      return lResult.Start ;
    else
      return -1 ;

  }

  // ------------------------------------------------------------------------
  /** \brief Copies a part of the array into another one

      Note that the output array will be completely overwritten.

      \param [in] offset Start point
      \param [in] count Items number to copy
      \param [out] output Output array to fill
  */
  void Mid(int offset, int count, tGenericArray<TYPE> & output) const
  {
    int i ;
    CheckIndex(offset,true);
    if ( count < 0 )
    {
      count = this->GetCount() - offset ;
    }
    CheckIndex(offset+count,true);

    // special case on self object
    if ( & output == this )
    {
      for (i=0; i<count; i++ )
      {
        *(TYPE*)(const TYPE*)GetItemPointer(i) = *GetItemPointer(offset+i);
      }
      output.SetCount(count);
    }
    else
    {
      output.SetCount(count);
      for (i=0; i<count; i++ )
      {
        *output.GetItemPointer(i) = *GetItemPointer(offset+i);
      }
    }
  }
  /** \brief See Mid()*/
  void Mid(tArrayPartRef aRef, tGenericArray<TYPE> & output) const
  {
    Mid(aRef.Start,aRef.Count, output);
  }

  /** \brief Copies items located at the beginning of the current array

      See Mid() for more details.

      \param count [in] Number of items to copy
      \param output [out] Output array to fill  */
  void Left(int count, tGenericArray<TYPE> & output) const
  {
    Mid(0,count,output);
  }

  /** \brief Copies items located at the end of the current array

      See Mid() for more details.

      \param count [in] Number of items to copy
      \param output [out] Output array to fill  */
  void Right(int count, tGenericArray<TYPE> & output) const
  {
    Mid(GetCount()-count,count,output);
  }

  // ------------------------------------------------------------------------

protected:
  bool RefBeforePattern(const tGenericArray<TYPE> & aPattern, bool includesPattern, tArrayPartRef & output) const
  {
    int lIndex ;

    lIndex = IndexOf(0,aPattern);
    if ( lIndex < 0 )
      return false ;

    if ( includesPattern )
      lIndex += aPattern.GetCount();

    output.Start = 0 ;
    output.Count = lIndex ;
    return true ;
  }

  bool RefAfterPattern(const tGenericArray<TYPE> & aPattern, bool includesPattern, tArrayPartRef & output) const
  {
    int lIndex ;

    lIndex = IndexOf(0,aPattern);
    if ( lIndex < 0 )
      return false ;

    if ( ! includesPattern )
      lIndex += aPattern.GetCount();

    output.Start = lIndex;
    output.Count = GetCount() - output.Start ;

    return true ;
  }

  // ------------------------------------------------------------------------

public:
  /** \brief Copie a part of array in an output

      \warning This method remove the contents extracted. Use Mid(), Left() or Right() if you want not change the target.
      \param [in] aPartRef Start and count
      \param [out] output Pointer on other array
  */
  void Extract(const tArrayPartRef & aPartRef, tGenericArray<TYPE> & output)
  {
    Mid(aPartRef.Start,aPartRef.Count,output);

    if ( & output != this )
    {
      RemoveRef(aPartRef);
    }
  }

  /** \brief See Extract()*/
  void Extract(int offset, int count, tGenericArray<TYPE> & output)
  {
    Extract(tArrayPartRef(offset,count),output);
  }

  void ExtractBeforeIndex(int offset, tGenericArray<TYPE> & output)
  {
    CheckIndex(offset,true);

    Extract(0,offset,output);
  }

  /** \brief Extract after an item index

      See Extract()
  */
  void ExtractAfterIndex(int offset, tGenericArray<TYPE> & output)
  {
    CheckIndex(offset,true);

    Extract(offset,GetCount()-offset,output);
  }

  /** \brief Extract before a pattern and define if pattern is included

      See Extract()
  */
  bool ExtractBeforePattern(const tGenericArray<TYPE> & aPattern, bool includesPattern, tGenericArray<TYPE> & output)
  {
    tArrayPartRef lRef ;

    if ( ! RefBeforePattern(aPattern,includesPattern,lRef) )
      return false ;

    Mid(lRef,output);
    if ( & output != this )
    {
      RemoveRef(lRef);
    }
    return true ;
  }

  /** \brief Extract after a pattern and define if pattern is included
      See Extract()
  */
  bool ExtractAfterPattern(const tGenericArray<TYPE> & aPattern, bool includesPattern, tGenericArray<TYPE> & output)
  {
    tArrayPartRef lRef ;

    if ( ! RefAfterPattern(aPattern,includesPattern,lRef) )
      return false ;

    Mid(lRef,output);
    if ( & output != this )
    {
      RemoveRef(lRef);
    }
    return true ;
  }

  // ------------------------------------------------------------------------
  /** \brief As ExtractBeforePattern() but without remove content after pattern*/
  bool GetBeforePattern(const tGenericArray<TYPE> & aPattern, bool includesPattern, tGenericArray<TYPE> & output) const
  {
    tArrayPartRef lRef ;

    if ( ! RefBeforePattern(aPattern,includesPattern,lRef) )
      return false ;

    Mid(lRef,output);
    return true ;
  }

  /** \brief As ExtractAfterPattern() but without remove content before pattern*/
  bool GetAfterPattern(const tGenericArray<TYPE> & aPattern, bool includesPattern, tGenericArray<TYPE> & output) const
  {
    tArrayPartRef lRef ;

    if ( ! RefAfterPattern(aPattern,includesPattern,lRef) )
      return false ;

    Mid(lRef,output);
    return true ;
  }

  // ------------------------------------------------------------------------
  /** \brief Verify that two arrays are equal

      Each value of both arrays are compared to check if they are equal.

      \warning To use this function properly the type or class of the items stored
      in the array must have a == operator defined */
  bool IsEqual(const tGenericArray<TYPE> & value) const
  {
    int i ;

    if ( value.GetCount() != GetCount() )
      return false ;

    for (i=0; i<GetCount(); i++ )
    {
      if ( ! (*GetItemPointer(i) == *value.GetItemPointer(i)) ) // not a mistake: to force usage of the == operator
        return false ;
    }
    return true ;
  }

  /** \brief Verify that two arrays are not equal (see IsEqual()) */
  bool IsNotEqual(const tGenericArray<TYPE> & value) const
  {
    return ! IsEqual(value);
  }

  /** \brief Operator bound to the IsEqual() routine */
  bool operator == (const tGenericArray<TYPE> & value) const
  {
    return IsEqual(value);
  }

  /** \brief Operator bound to the IsNotEqual() routine */
  bool operator != (const tGenericArray<TYPE> & value) const
  {
    return IsNotEqual(value);
  }

  /** \brief Operator constant pointer*/
  operator const TYPE * () const
  {
    return GetItemPointer(0) ;
  }

  /** \brief Operator pointer*/
  operator TYPE * ()
  {
    return GetItemPointer(0) ;
  }

  // ------------------------------------------------------------------------

  /** \brief Get constant array pointer*/
  const TYPE * GetPointer() const
  {
    return GetItemPointer(0);
  }

  /** \brief Get array pointer*/
  TYPE * GetPointer()
  {
    return GetItemPointer(0);
  }

  /** \brief Get constant array pointer*/
  inline const tGenericArray<TYPE> & GetArray() const
  {
    return *this ;
  }

  /** \brief Get array pointer*/
  inline tGenericArray<TYPE> & GetArray()
  {
    return *this ;
  }

  // ------------------------------------------------------------------------
  /** Define the behavior of the operator []*/
  TYPE & operator [] (int Index)
  {
    return GetItem(Index);
  }

  /** Define the behavior of the operator []*/
  const TYPE & operator [] (int Index) const
  {
    return GetItem(Index);
  }

  /** Define the behavior of the operator []*/
  TYPE & operator [] (unsigned int Index)
  {
    return GetItem((int)Index);
  }

  /** Define the behavior of the operator []*/
  const TYPE & operator [] (unsigned int Index) const
  {
    return GetItem((int)Index);
  }

  /** Define the behavior of the operator []*/
  TYPE & operator [] (short Index)
  {
    return GetItem((int)Index);
  }

  /** Define the behavior of the operator []*/
  const TYPE & operator [] (short Index) const
  {
    return GetItem((int)Index);
  }

  /** Define the behavior of the operator []*/
  TYPE & operator [] (unsigned short Index)
  {
    return GetItem((int)Index);
  }

  /** Define the behavior of the operator []*/
  const TYPE & operator [] (unsigned short Index) const
  {
    return GetItem((int)Index);
  }

  /** Define the behavior of the operator []*/
  TYPE & operator [] (long Index)
  {
    return GetItem((int)Index);
  }

  /** Define the behavior of the operator []*/
  const TYPE & operator [] (long Index) const
  {
    return GetItem((int)Index);
  }

  /** Define the behavior of the operator []*/
  TYPE & operator [] (unsigned long Index)
  {
    return GetItem((int)Index);
  }

  /** Define the behavior of the operator []*/
  const TYPE & operator [] (unsigned long Index) const
  {
    return GetItem((int)Index);
  }

} ;

template <class TYPE> int tGenericArray<TYPE>::DebugCount() const 
{
  return GetCount();
}

template <class TYPE> int tGenericArray<TYPE>::DebugCapacity() const 
{
  return GetCapacity();
}

template <class TYPE> const TYPE * tGenericArray<TYPE>::DebugPointer() const
{
  return GetPointer();
}



/** \ingroup collections 
    \brief Class representing an iterator on an array
    \param TYPE Item type of the array
 */
template<typename TYPE> class tArrayIterator : public tIterator<TYPE>
{
protected:
  tGenericArray<TYPE> * mArray;
  int mItemIndex ;

public:
  /** \brief Constructor 
      \param anArray[in] Array to iterate on */
  explicit tArrayIterator(tGenericArray<TYPE> & anArray) : tIterator<TYPE>()
  {
    mArray = & anArray ;
    Reset();
  }

  /** \brief Reset the iterator (restart) */
  void Reset()
  {
    tIterator<TYPE>::Reset();
    mItemIndex = -1 ;
  }

  /** \brief Move to the next item 
      \return true when succeed, false if iterator has reached the end of the array */
  bool Next()
  {
    bool lResult = (mItemIndex+1) < mArray->GetCount();
    if ( lResult )
    {
      mItemIndex++ ;
      tIterator<TYPE>::mItem = & (*mArray) [mItemIndex] ;
    }
    return lResult ;
  }

  /** \brief Return the index of the current iterator item */
  int Index() const
  {
    return mItemIndex ;
  }

};

/** \ingroup collections 
    \brief Class representing a container array
    \param CAPACITY_TYPE Capacity type of of the attribute "mDataCount"
    \param CAPACITY Maximum items in the array
    \param TYPE Data type of each array item
 */
template <typename CAPACITY_TYPE, int CAPACITY, typename TYPE> class tArray : public virtual tGenericArray<TYPE>
{
protected:
  CAPACITY_TYPE       mDataCount ;// ! capacity & count
  TYPE                mData[CAPACITY];

protected:
  virtual void InternalSetCount(int aCount)
  {
    tGenericArray<TYPE>::InternalSetCount(aCount);
    mDataCount = (CAPACITY_TYPE) aCount ;
    tGenericArray<TYPE>::OnInternalSetCountDone(aCount);
  }

  virtual TYPE * GetItemPointer(int anIndex)
  {
    return mData + anIndex;
  }

  virtual const TYPE * GetItemPointer(int anIndex) const
  {
    return mData + anIndex ;
  }

public:
  /** \brief Constructor*/
  tArray()
  {
    InternalSetCount(0);
  }

  /** \brief Constructor copying the content of another array*/
  explicit tArray(const tGenericArray<TYPE> & anArray)
  {
    InternalSetCount(0);
    SetContent(anArray);
  }

  /** \brief Constructor copying the content of a C array */
  explicit tArray(const TYPE * aSrc, int aSrcCount)
  {
    InternalSetCount(0);
    SetContent(aSrc, aSrcCount);
  }

  /** \brief Constructor initializing the size of the array */
  explicit tArray(int anInitCount)
  {
    InternalSetCount(anInitCount);
  }

  /** \brief Destructor*/
  virtual ~tArray()
  {
  }

  /** \brief Implementation of the tGenericArray::GetCount() */
  virtual int GetCount() const
  {
    return mDataCount ;
  }

  /** \brief Implementation of the tGenericArray::GetCapacity() */
  virtual int GetCapacity() const
  {
    return CAPACITY ;
  }
} ;

/** \ingroup collections 
    \brief Class representing an array object based on an external C array

    The item count must be external and given as reference to the constructor of the class. This number must be
    initialized with the initial count.

    \param CAPACITY_TYPE Capacity type of of the attribute #mDataCountRef
    \param TYPE Data type of each array item
 */
template <typename CAPACITY_TYPE, typename TYPE> class tExternalArrayAndCount : public virtual tGenericArray<TYPE>
{
protected:
  CAPACITY_TYPE *		mDataCountRef ;
  CAPACITY_TYPE		  mCapacity ;
  TYPE *            mData;

protected:
  virtual void InternalSetCount(int aCount)
  {
    tGenericArray<TYPE>::InternalSetCount(aCount);
    *mDataCountRef = (CAPACITY_TYPE) aCount ;
  }


  virtual TYPE * GetItemPointer(int anIndex)
  {
    return mData + anIndex;
  }

  virtual const TYPE * GetItemPointer(int anIndex) const
  {
    return mData + anIndex ;
  }

  void Init(CAPACITY_TYPE aCapacity, TYPE * aData, CAPACITY_TYPE * aDataCountRef)
  {
    mCapacity = aCapacity ;
    mDataCountRef = aDataCountRef ;
    mData = aData ;
  }

  tExternalArrayAndCount()
  {
    Init(0,NULL,NULL);
  }

public:
  /** \brief Constructor
      \param [in] aCapacity Maximum number of items
      \param [in] aData Pointer on external C array
      \param [in] aDataCountRef Pointer on the variable holding the current number of items in the array 
  */
  explicit tExternalArrayAndCount(CAPACITY_TYPE aCapacity, TYPE * aData, CAPACITY_TYPE * aDataCountRef) :
  mDataCountRef (aDataCountRef),
  mCapacity     (aCapacity),
  mData         (aData) {}

  /** \brief Implementation of the tGenericArray::GetCount() */
  virtual int GetCount() const
  {
    return *mDataCountRef ;
  }

  /** \brief Implementation of the tGenericArray::GetCapacity() */
  virtual int GetCapacity() const
  {
    return mCapacity ;
  }

} ;

// -----------------------------------------------------------------
/** \ingroup collections 
    \brief Array object on an external C array, with internal count management */
template <typename TYPE> class tExternalArray : public tExternalArrayAndCount<int,TYPE>
{
protected:
  int mCount ;

public:
  /** \brief Constructor on an external C array
      \param aCapacity [in] The capacity of the C array
      \param aData[in,out] The pointer to the C array
      \param aDataCount [in] The current number of items in the C array */
  explicit tExternalArray(int aCapacity, TYPE * aData, int aDataCount)
         : tExternalArrayAndCount<int,TYPE>(aCapacity,aData,&mCount), mCount(aDataCount)
  {
  }
};

// -----------------------------------------------------------------
/** \ingroup collections     
    \brief Class representing an array with a size fixed at construction
    
    The constructor of this class or the SetCapacity() routine perform dynamic memory allocation to create 
    items holder. That means these two operations are not realtime compliant; but the other operations are 
    realtime compliant.
    
    Such array objects can be created during the initialization phase or in a non realtime thread without 
    any kind of risk.

    \param TYPE Data type of each array item */
template<typename TYPE> class tDynamicArray : public virtual tGenericArray<TYPE>
{
protected:
  int     mCapacity  ;
  int   	mDataCount ;
  TYPE *  mData;

protected:
  virtual void InternalSetCount(int aCount)
  {
    tGenericArray<TYPE>::InternalSetCount(aCount);
    mDataCount = aCount ;
  }

  virtual TYPE * GetItemPointer(int anIndex)
  {
    return mData + anIndex;
  }

  virtual const TYPE * GetItemPointer(int anIndex) const
  {
    return mData + anIndex ;
  }

  void Init(int aCapacity)
  {
    mDataCount = 0 ;
    mCapacity = aCapacity ;
    mData = new TYPE[aCapacity];
  }

  void Destroy()
  {
    mDataCount = 0 ;
    mCapacity = 0 ;
    if ( mData )
    {
      delete [] mData ;
      mData = NULL ;
    }
  }

public:
  /** \brief Constructor
      \param [in] aCapacity Initial capacity of the array (must be >0)
  */
  explicit tDynamicArray(int aCapacity)
  {
    Init(aCapacity);
  }

  /** \brief Destructor*/
  virtual ~tDynamicArray()
  {
    Destroy();
  }

  /** \brief Implementation of the tGenericArray::GetCount() */
  virtual int GetCount() const
  {
    return mDataCount ;
  }

  /** \brief Implementation of the tGenericArray::GetCapacity() */
  virtual int GetCapacity() const
  {
    return mCapacity ;
  }

  /** \brief Set the capacity of the array
     
      This routine can be called only if the array is empty. If it not the case,
      an #cArrayException will be raised. */
  virtual void SetCapacity(int aCapacity)
  {
    if ( mDataCount > 0 )
    {
      cArrayException::ThrowNotEmpty();
    }
    else
    {
      Destroy();
      Init(aCapacity);      
    }
  }

};

// -----------------------------------------------------------------
/** \ingroup collections 
    \brief Class representing a dynamic container array
    It's almost the same of the #tDynamicArray but the capacity can be set when you want.
    \warning It is not a real time compliant class if you change often the capacity!
    \param TYPE Data type of each array item
    \param ALLOC_BLOCK_SIZE Maximum dynamic allocation size
 */
template<typename TYPE, int ALLOC_BLOCK_SIZE> class tDynamicGrowingArray : public tDynamicArray<TYPE>
{
protected:
  virtual void CheckCapacity(int newCount)
  {
    if ( newCount < 0 )
    {
      cArrayException::ThrowArrayToSmall(newCount+tDynamicArray<TYPE>::GetReservedCount(),tDynamicArray<TYPE>::GetCapacity());
    }
    else 
	{
		int lNewCountWithReserve = newCount + tDynamicArray<TYPE>::GetReservedCount();
		if ( lNewCountWithReserve > tDynamicArray<TYPE>::GetCapacity() )
		{
		  SetCapacity(ROUND_UP(lNewCountWithReserve,ALLOC_BLOCK_SIZE));
		}
	}
  }

public:
  tDynamicGrowingArray() : tDynamicArray<TYPE>(ALLOC_BLOCK_SIZE)
  {
  }

  /** \brief Return set max items numbers. (Can be called only one time if the "aCapacity" isn't set on the constructor*/
  virtual void SetCapacity(int aCapacity)
  {
    if ( aCapacity < (tDynamicArray<TYPE>::mDataCount+tDynamicArray<TYPE>::GetReservedCount()) )
    {
      cArrayException::ThrowArrayToSmall(aCapacity,tDynamicArray<TYPE>::mDataCount+tDynamicArray<TYPE>::GetReservedCount());
    }
    else
    {
      TYPE * lNewData = new TYPE[aCapacity] ;
      int i ;

      for (i=0; i < tDynamicArray<TYPE>::mDataCount; i++ )
      {
        lNewData[i] = tDynamicArray<TYPE>::mData[i];
      }
      tDynamicArray<TYPE>::mCapacity = aCapacity ;
      delete [] tDynamicArray<TYPE>::mData ;
      tDynamicArray<TYPE>::mData = lNewData ;
      tDynamicArray<TYPE>::InternalSetCount(tDynamicArray<TYPE>::mDataCount);
    }
  }

};

// -----------------------------------------------------------------
/** \ingroup collections 
    \brief Class representing an array copier*/
class cArrayCopier
{
public:
  /** \brief Copie a full array in an other one
      \param [in] aSource Array to copy (constant)
      \param [out] aTarget Target array
  */
  template<class TYPE> void Copy(const tGenericArray<TYPE> & aSource, LPVOID aTarget)
  {
    memcpy(aTarget,(const TYPE*)aSource,aSource.GetCount() + aSource.GetReservedCount());
  }
  /** \brief See cArrayCopier::Copy()*/
  template<class TYPE> void Copy(LPCVOID aSource, DWORD aSourceCount, tGenericArray<TYPE> & aTarget)
  {
    aTarget.SetContentFromPointer((const TYPE*)aSource, aSourceCount / sizeof(TYPE));
  }
} ;


template <class TYPE> bool Search(tArrayBrowseDirection direction, int startOffset, const TYPE * data, int dataSize, tArrayPartRef & aResult)
{
  tExternalArray<TYPE> lArray(dataSize,(TYPE*)data,dataSize);
  return Search(direction,startOffset,lArray, aResult);
}

#endif
