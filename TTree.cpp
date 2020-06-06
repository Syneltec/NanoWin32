#include "TTree.h"

cTree::cTree()
{
    mParent = mNext = mPrev = mChild = NULL ;
}

cTree::cTree(cTree & aParent)
{
    mParent = mNext = mPrev = mChild = NULL ;
    aParent.AddChild(*this);
}

void cTree::RemoveChild(cTree * aChild)
{
    if ( aChild->mParent == this )
	{
		if ( NULL != aChild->mNext )
			aChild->mNext->mPrev = aChild->mPrev ;
		if ( NULL != aChild->mPrev )
			aChild->mPrev->mNext = aChild->mNext ;
		
		if ( mChild == aChild )
			mChild = aChild->mNext ;

        aChild->mNext = NULL ;
        aChild->mPrev = NULL ;
		aChild->mParent = NULL ;
	}
}

void cTree::Detach()
{
    if ( NULL != mParent )
    {
        mParent->RemoveChild(this);
        mParent = NULL ;
    }
}

void cTree::AddChild(cTree & aChild)
{
    aChild.Reparent(*this);
}

void cTree::Reparent(cTree & aNewParent)
{
    if ( mParent!=&aNewParent && this!=&aNewParent )
    {
        Detach();
        mParent = & aNewParent ;
		if ( NULL == aNewParent.mChild )
		{
			aNewParent.mChild = this ;
		}
		else
        {
            cTree * lLastChild = aNewParent.mChild;
			while ( NULL != lLastChild->mNext )
				lLastChild = lLastChild->mNext ;

            lLastChild->mNext = this ;
			this->mPrev = lLastChild ;
        }
    }
}

size_t cTree::HierarchyCount() const 
{
    size_t result = 1 ;

    if ( NULL != mChild )
    {
        result += mChild->HierarchyCount();
        
        cTree * lNextChild ;
        for ( lNextChild=mChild->mNext; NULL!=lNextChild; lNextChild = lNextChild->mNext )
        {
            result += lNextChild->HierarchyCount();
        }
    }

    return result;;
}

size_t cTree::ChildrenCount() const 
{
    size_t result = 0;
    
    cTree * lNextChild ;
    for ( lNextChild=mChild; NULL!=lNextChild; lNextChild=lNextChild->mNext )
    {
        result ++;
    }

    return result;;
}

bool cTree::HasChild() const 
{
	return NULL != mChild;
}

cTree * cTree::GetParent() const 
{
	return mParent ;
}

cTree * cTree::GetNext() const 
{
	return mNext ;
}

cTree * cTree::GetPrev() const 
{
	return mPrev ;
}

cTree * cTree::GetFirstChild() const 
{
	return mChild ;
}

cTree * cTree::GetLastChild() const 
{
	cTree * lResult = mChild ;
	
	if ( NULL != lResult )
	{
		while ( NULL != lResult->mNext )
		{			
			lResult = lResult->mNext ;
		}
	}
	return lResult ;
}

// ----------------------------------------------------------------------------

cTreeIterator::cTreeIterator(cTree & aRootNode) : mRoot(&aRootNode), mStarted(false)
{
	Reset();
}

void cTreeIterator::Reset()
{
	mStarted = false ;
	mItem = NULL ;
}

cTree & cTreeIterator::Item()
{
	return *mItem;
}

// ---------------------------------------------------------------------------

cTreeTopDownIterator::cTreeTopDownIterator(cTree & aRootNode) : cTreeIterator(aRootNode)
{
}


bool cTreeTopDownIterator::Next()
{
	if ( mStarted && NULL == mItem )
		return false ;
	else
	{
		if ( NULL == mItem )
		{
			mStarted = true ;
			mItem = mRoot ;
		}
		else if ( NULL != mItem->mChild )
			mItem = mItem->mChild ;
		else {
			cTree * lItem = mItem ;
			while ( NULL!=lItem && NULL==lItem->mNext )
				lItem = lItem->mParent ;
			if ( NULL != lItem )
				lItem = lItem->mNext ;
			mItem = lItem ;
		}
	}
	return mItem != NULL ;
}

// ---------------------------------------------------------------------------

cTreeBottomUpIterator::cTreeBottomUpIterator(cTree & aRootNode) : cTreeIterator(aRootNode)
{
}
	
bool cTreeBottomUpIterator::Next()
{
	if ( mStarted && NULL == mItem )
		return false ;
	else
	{
		if ( NULL == mItem )
		{
			mStarted = true ;
			mItem = mRoot ;
			while ( mItem->HasChild() )
			{
				mItem = mItem->GetLastChild();
			}
		}
		else if ( NULL != mItem->mPrev )
		{
			mItem = mItem->mPrev ;
			while ( mItem->HasChild() )
			{
				mItem = mItem->GetLastChild();
			}
		}
		else {
			if ( NULL != mItem )
				mItem = mItem->mParent ;
		}
	}
	return mItem != NULL ;
}
