#ifndef __ITERATOR_H__
#define __ITERATOR_H__

// ---------------------------------------------------------------------------

/** \brief Abstract class basis of all iterators of the library
    \param cItemClass Type to iterate
 */
template<typename cItemClass> class tIterator
{
protected:
    cItemClass * mItem ;

public:
	tIterator() : mItem(NULL)
	{
	}

  /** \brief Reset item reference*/
	virtual void Reset()
	{
		mItem = NULL ;
	}

  /** \brief Abstraction of Next to override
      \return True if an item is found
  */
	virtual bool Next() = 0 ;

  /** \brief Read current item iterated*/
	cItemClass & Item()
	{
		return * mItem ;
	}

  /** \brief Copy item reference in a sub class*/
	template<typename cItemSubClass> void GetItem(cItemSubClass * & aResult)
	{
		aResult = (cItemSubClass*) mItem ;
	}
};

// ---------------------------------------------------------------------------

/** \brief Common "foreach" based iterator
 */
template<typename cItemClass> class tForEachIterator
{
protected:
	tIterator<cItemClass> & mIterator ;

public:
  /** \brief Constructor with reference on an iterator*/
  tForEachIterator(tIterator<cItemClass> & anIterator) : mIterator(anIterator)
	{
	}
	
  /** \brief Define function to callback on each iteration
      Iterate until callback function return false. This class is usefull for #cTree.
      \param [in] aCallee Pointer on class to iterate
      \param [in] aCallback Pointer on callback function
      \return True all iteration is done
  */
	template<class Callee> bool ForEach(Callee * aCallee, bool (Callee::*aCallback)(cItemClass & aTree))
	{
		mIterator.Reset();
		while ( mIterator.Next() )
		{
			if ( ! (aCallee->*aCallback)(mIterator.Item()) )
				return false;
		}
        return true;
	}
};

// ---------------------------------------------------------------------------

#endif