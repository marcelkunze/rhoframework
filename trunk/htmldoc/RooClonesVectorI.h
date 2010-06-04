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
#ifndef ROOCLONESVECTORI_HH
#define ROOCLONESVECTORI_HH


// Base class header:
#include "KangaSchema/RooPersObj.h"


// Collaborating class header:
#include "TClonesArray.h"


// Forward declarations:


// Class declaration:
class RooClonesVectorI : public RooPersObj {

public:

  // Constructor:
  RooClonesVectorI();

  // Destructor:
  virtual ~RooClonesVectorI();

  // Accsessors:
  virtual const UInt_t & getNEntries() const { return _nent; }
  virtual RooPersObj* at( const int & index ) const { 
    return (RooPersObj*) _array->At( index );
  }
  virtual TClonesArray* getArray() const { return _array; }

  // Modifieres:
  virtual void setOwnArray( Bool_t ownArray ) { _ownArray= ownArray; }

  // Static member functions:
  static void setTheArray( TClonesArray* );

protected:

  // Data members:
  UInt_t _nent;                   // Number of entries in TClonesArray
  TClonesArray* _array;           // TClonesArray
  static TClonesArray* _theArray; // Pointer to shared actual memory
  Bool_t _ownArray;                 //! Do we own the TClonesArray?

private:

  // Data members:

  // Special ROOT declarations:
  ClassDef(RooClonesVectorI,1) // BABAR Kanga

};

#endif
