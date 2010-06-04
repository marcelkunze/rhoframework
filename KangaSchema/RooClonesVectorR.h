//  	-*- Mode: c++ -*-
//
// Templateted container class to get type safe access to ROOT 
// persistent objects. It creates the TClonesArray for the concrete
// type. 
//
// Author: Stefan Kluth
// Contributors:
// History:
// 
//
// (C) 1999 LBNL
//
#ifndef ROOCLONESVECTORR_HH
#define ROOCLONESVECTORR_HH


// Base class header:
#include "KangaSchema/RooClonesVectorI.h"


// Collaborating class header:


// Forward declarations:


// Class declaration:
template <class T> class RooClonesVectorR : public RooClonesVectorI {

public:

  // Constructor:
  RooClonesVectorR( const unsigned & capacity= 50 );

  // Destructor:
  virtual ~RooClonesVectorR();

  // Accsessors:

  // Modifieres:
  T* addNewWithPlacement() { return (T*) (*_array)[_nent++]; }
  void clear();

  // Static member functions:

private:

  // Data members:

  // Special ROOT declarations:
  ClassDefT(RooClonesVectorR<T>,1) // BABAR Kanga

};

// Even more special ROOT declarations for template classes:
ClassDefT2(RooClonesVectorR,T)

// Need to protect BaBar template instantiation against rootcint processing
#ifndef __CINT__
#ifdef BABAR_COMP_INST
#include "KangaSchema/RooClonesVectorR.cxx"
#endif // BABAR_COMP_INST
#endif // __CINT__

#endif
