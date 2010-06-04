#ifndef BtaCandListIterator_hh
#define BtaCandListIterator_hh
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// BtaCandListIterator							//
//                                                                      //
// Iterator class for BtaCandList					//
//									//
// Author List:								//
// Marcel Kunze,  RUB, Feb. 99						//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "TObject.h"

class BtaCandidate;
class BtaCandList;

class BtaCandListIterator : public TObject {
    
public:
    
    BtaCandListIterator(BtaCandList &);
    // Constructor taking a corresponding list as argument. Starting at the 
    // first element.
    
    BtaCandListIterator(const BtaCandListIterator &);
    // Copy constructor.
    
    ~BtaCandListIterator();
    
    BtaCandidate * next();
    // Returns a pointer to the current object in the associated list, moving
    // forward to the next. Returns 0 if all objects are done.
    
    BtaCandidate * previous();
    // Moves backward one step in the list and returns the object found there.
    // If current object is the first in the list, no stepping is done and 0 is
    // returned.
    
    BtaCandidate * current();
    // Returns a pointer to the current object in the associated list,
    // without incrementing the index.
    
    // public:
    
    int index() const;
    
    void rewind();
    // Rewinds the iterator to the first element of the list.
    
    void skip(int);
    // Move iterator forward or backward.
    
    void skipAll();
    // Move iterator past last element.
    
private:
    
    BtaCandList * l; //! Pointer to the associated list.
    
    int i; //! Current position in the associated list.
    
public:
    ClassDef(BtaCandListIterator,1) // Iterator for BtaCandList
};


#endif
