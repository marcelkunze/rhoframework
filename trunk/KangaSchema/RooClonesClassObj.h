//  	-*- Mode: c++ -*-
//
// Class to hold TClonesArray of root 
// persistent objects. Memory reuse is supported through the 
// static data member "TClonesArray* _theArray" and the static
// member function "setTheArray( TClonesArray* )". If _theArray is
// set before objects of this class are created the constructor will
// not create a new TClonesArray and use the one pointed at by
// _theArray. This can be done in a "Scribe" by creating a TClonesArray
// and passing a pointer to it to this class by executing 
// "setTheArray( TClonesArray* )" before the execution of
// TBranch::GetEvent( ... ). This allows to reuse memory when reading data. 
//
// Author: Stefan Kluth
// Contributors:
// History:
//
// (C) 1999 LBNL
//
#ifndef ROOCLONESCLASSOBJ_HH
#define ROOCLONESCLASSOBJ_HH


//----------------------
// Base Class Headers --
//----------------------
#include "KangaSchema/RooEvtObj.h"


// Collaborating class header:
#include "TClonesArray.h"


// Forward declarations:


// Class declaration:
template <class T> 
class RooClonesClassObj : public RooEvtObj<T> {

public:

  // Constructor:
  RooClonesClassObj();

  // Destructor:
  virtual ~RooClonesClassObj();

  // Accsessors:
  virtual TClonesArray* getArray() const { return _array; }

  // Modifieres:
  virtual void setOwnArray( Bool_t ownArray ) { _ownArray= ownArray; }

  // Static member functions:
  static void setTheArray( TClonesArray* );

//protected:

  // Data members:
  Int_t _nent;                    // Actual number of entries in TClonesArray
  TClonesArray* _array;           // Array
  static TClonesArray* _theArray; //! Pointer to shared actual memory
  Bool_t _ownArray;                 //! Do we own the TClonesArray?

private:


  ClassDefT(RooClonesClassObj<T>,1)


};


// Even more special ROOT declarations for template classes:
ClassDefT2(RooClonesClassObj,T) // BABAR Kanga


// Need to protect BaBar template instantiation against rootcint processing
#ifndef __CINT__
#ifdef BABAR_COMP_INST
#include "KangaSchema/RooClonesClassObj.cxx"
#endif // BABAR_COMP_INST
#endif // __CINT__


#endif
