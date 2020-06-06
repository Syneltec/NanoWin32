#ifndef __TCIRCULAR_H__
#define __TCIRCULAR_H__

#include "TArray.h"

/** \ingroup collections 
    @{ */

// ---------------------------------------------------------------------------

/** \brief Class implementing a circular array behavior over an external array
    \param TYPE Data type of each array item
 */
template <typename TYPE> class tCircularReferencedArray : public virtual tGenericArray<TYPE>
{
protected:
  tGenericArray<TYPE> * mReferencedArray ;

  int mFirstIndex, 
      mLastIndex ;

  /** \brief Adjust the given index to match the indexes range */
  int GetNormalizedIndex(int anIndex) const
  {
    if ( anIndex < 0 )
      return anIndex + GetCapacity() ;
    else
      return anIndex % GetCapacity() ;
  }

  /** \brief Returns the minimum the external array must have to hold the current circular array */
  int GetRequiredArrayCount() const
  {
    if ( mLastIndex <= mFirstIndex )
      return GetCapacity();
    else
      return mLastIndex ;
  }

  /** \brief Manager the array resize operations. 
     
      This routine has been reimplemented to ensure the circular behavior and the adjustment of the external
      array */
  virtual void InternalSetCount(int aCount)
  {
    int lNewLastIndex ;
    int lPrevCount;
    int lCapacity ;

    this->CheckCapacity(aCount);

    lCapacity = GetCapacity();
    lPrevCount = GetCount();
    if ( 0 == aCount )
    {
      mFirstIndex = -1 ;
      mLastIndex = 0 ;
    }
    else 
    {
      if ( lPrevCount == 0 )
      {
        mFirstIndex = mLastIndex = 0;
      }
      lNewLastIndex = (mLastIndex + aCount - lPrevCount) % lCapacity;
      if ( lNewLastIndex < 0 )
        lNewLastIndex += lCapacity;
      mLastIndex = lNewLastIndex ;
    }
   
    mReferencedArray->EnsureMinimalCount(GetRequiredArrayCount());
    OnCountChanged();
  }

  /** \brief Manage the circular behavior */
  virtual TYPE * GetItemPointer(int anIndex) 
  {
    anIndex = GetNormalizedIndex(mFirstIndex + anIndex);
    return & mReferencedArray->GetItem(anIndex);
  }

  /** \brief Manage the circular behavior */
  virtual const TYPE * GetItemPointer(int anIndex) const
  {
    anIndex = GetNormalizedIndex(mFirstIndex + anIndex);
    return & mReferencedArray->GetItem(anIndex);
  }

  /** \brief Called when the size of the circular array has changed */
  virtual void OnCountChanged()
  {
  }

  /** \brief Check the external array is assigned or raise an #cException */
  void CheckReferencedArrayAssigned() const
  {
    if ( NULL == mReferencedArray )
      Throw<cException>("tCircularReferencedArray::CheckReferencedArrayAssigned not verified !");
  }

  /** \brief Default constructor */
  tCircularReferencedArray()
  {
    mReferencedArray = NULL ;
    mFirstIndex = -1 ;
    mLastIndex = 0 ;
  }

public:
  /** \brief Constructor on an external array */
  explicit tCircularReferencedArray(tGenericArray<TYPE> & anArray) 
  {
    mReferencedArray = &anArray ;
    if ( mReferencedArray->IsEmpty() )
    {
      mFirstIndex = -1 ;
      mLastIndex = 0 ;
    }
    else
    {
      mFirstIndex = 0 ;
      mLastIndex = mReferencedArray->GetCount() ;
    }
  }

  /** \brief Retrieve the current number of element in the circular buffer 
     
      \warning This value is absolutely not the same as the external array count.*/
  virtual int GetCount() const 
  {
    if ( mFirstIndex < 0 )
      return 0 ;
    else if ( mLastIndex > mFirstIndex )
      return mLastIndex - mFirstIndex ;
    else 
      return (GetCapacity() - mFirstIndex) + mLastIndex ;
  }

  /** \brief Get the capacity of the circular array
     
      \warning This value is directly get from the external array.*/
  virtual int GetCapacity() const 
  {
    CheckReferencedArrayAssigned();
    return mReferencedArray->GetCapacity();
  }

  /** \brief Remove a part of the array
     
      This routine has been reimplemented to ensure the circula behavior.*/
  virtual void RemoveRef(const tArrayPartRef aRef)
  {
    if ( 0 == aRef.Start )
    {
      mFirstIndex = (mFirstIndex + aRef.Count) % GetCapacity();
      OnCountChanged();
    }
    else
    {
      tGenericArray<TYPE>::RemoveRef(aRef);
    }
  }

  /** \brief Append an item to the array
     
      If the array is full, remove the first item before the append.*/
  TYPE & AppendOver()
  {
    if ( this->IsFull() )
    {
      tGenericArray<TYPE>::Remove(0,1);
    }
    return this->AllocateItem();
  }

  void AppendOver(const TYPE & anItem)
  {
    AppendOver() = anItem ;
  }

};

// ---------------------------------------------------------------------------

/** \ingroup collections
    \brief Class implementing a circular buffer with an internal holder
    \param CAPACITY_TYPE The data type used to represent the size of the array
    \param CAPACITY The capacity of the holder array
    \param TYPE Data type stored in the array
 */
template <typename CAPACITY_TYPE, int CAPACITY, typename TYPE> class tCircularArray : public tCircularReferencedArray<TYPE>
{
protected:
   tArray<CAPACITY_TYPE,CAPACITY,TYPE> mArray ;

public:
  /** \brief Default constructor */
  tCircularArray() : tCircularReferencedArray<TYPE>()
  {
    this->mReferencedArray = & mArray ;
  }

  /** \brief Get the internal holder of the circular array */
  const tGenericArray<TYPE> & GetInternalArray() const
  {
    return mArray ;
  }

};

/** @}  */

#endif
