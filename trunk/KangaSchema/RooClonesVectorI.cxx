#ifndef ROOCLONESVECTORI_CC
#define ROOCLONESVECTORI_CC
//
// Implementation of RooClonesVectorI:
//

// C++ headers:


// This class header:
#include "KangaSchema/RooClonesVectorI.h"


// Collaborating class headers:
#include "TBuffer.h"


// Special ROOT definitions:
ClassImp(RooClonesVectorI)


// Static data members:
TClonesArray* RooClonesVectorI::_theArray= 0;


// Static member functions:
void RooClonesVectorI::setTheArray( TClonesArray* theArray ) { 

  _theArray= theArray;

}


// Constructor:
RooClonesVectorI::RooClonesVectorI() :
  RooPersObj(),
  _ownArray( kTRUE ), 
  _nent( 0 ), 
  _array( 0 )
{}


// Destructor:
RooClonesVectorI::~RooClonesVectorI() {}

#endif



