#ifndef __GENERIC_SERVICE__
#define __GENERIC_SERVICE__

#include "TArray.h"

/** \ingroup collections 
    \brief Class representing a tree collection

    It's usefull for deploy object methods in automatic */
class cTree
{
    friend class cTreeIterator ;
    friend class cTreeTopDownIterator ;
    friend class cTreeBottomUpIterator ;
public:
    /** \brief Constructor of a root tree item */
    cTree();
    /** \brief Constructor of a child tree item */
    explicit cTree(cTree & aParent);

    /** \brief Detach from parent*/
    void Detach();
    /** \brief Add a child in the child list*/
    void AddChild(cTree & aChild);
    /** \brief Change current parent of the the current tree item */
    void Reparent(cTree & aNewParent);

    /** \brief Count the complete number of items hold by the current tree item*/
    size_t HierarchyCount() const ;
    /** \brief Count the number of direct children*/
    size_t ChildrenCount() const ;
    /** \brief Verify if the current tree item has one or more children */
    bool HasChild() const ;

protected:
    void RemoveChild(cTree * aChild);

    cTree * mParent,
          * mChild,
          * mNext,
          * mPrev;

public:
    /** \brief Get parent reference*/
    cTree * GetParent() const ;
    /** \brief Get next item reference in the same level*/
    cTree * GetNext() const ;
    /** \brief Get preview item reference in the same level*/
    cTree * GetPrev() const ;
    /** \brief Get first dîrect child reference*/
    cTree * GetFirstChild() const ;
    /** \brief last first dîrect child reference*/
    cTree * GetLastChild() const ;
};

/** \ingroup collections 
    \brief Class representing a generic tree iterator */
class cTreeIterator : public tIterator<cTree>
{
protected:
    bool  mStarted ;
    cTree *mRoot ;

public:
    /** \brief Constructor of a tree iterator*/
    explicit cTreeIterator(cTree & aRootNode);
    /** \brief Reset iterator to the first item*/
    virtual void Reset();
    /** \brief Get generic item*/
    virtual bool Next() = 0 ;
    cTree & Item();

    template<class cSubTreeClass> void GetItem(cSubTreeClass * & result)
    {
        result = (cSubTreeClass*) mItem ;
    }
};

/** \ingroup collections 
    \brief Class representing a tree top down iterator

    It is usefull for call generic method like save, run, initialize... */
class cTreeTopDownIterator : public cTreeIterator
{
public:
    /** \brief Constructor of a tree iterator*/
    explicit cTreeTopDownIterator(cTree & aRootNode);
    /** \brief Verify and move reference on next item
        \return True if an item is found */
    virtual bool Next();
};

/** \ingroup collections 
    \brief Class representing a tree top down iterator

    It is usefull for call generic method like save, run, initialize... */
class cTreeBottomUpIterator : public cTreeIterator
{
public:
    /** \brief Constructor of a tree iterator*/
    explicit cTreeBottomUpIterator(cTree & aRootNode);
   /** \brief Check and move reference on next item
        \return True if an item is found
    */
    virtual bool Next();
};

#endif
