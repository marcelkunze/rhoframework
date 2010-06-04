//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaCandPtrR.cxx,v 1.1.1.1 2001-05-15 07:01:17 marcel Exp $
//
// Description:
//      Stores list of indices for the packing/unpacking of the
//      lists in the microdb
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Stefan Kluth     
//
// Copyright Information:
//	Copyright (C) 1999       LBNL
//
//------------------------------------------------------------------------

//-----------------------
// This Class's Header --
//-----------------------
#include "KangaSchema/BtaCandPtrR.h"


//-------------
// C Headers --
//-------------
/*
extern "C" 
{
#include <math.h>
#include <limits.h>
}
*/
//---------------
// C++ Headers --
//---------------


//-------------------------------
// Collaborating Class Headers --
//-------------------------------
//#include "RooUtils/RooEvtObjLocReg.h"
//#include "BetaTools/BtaCandPtr.h"
//#include "PDT/Pdt.h"
//#include "ErrLogger/ErrLog.h"


//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------
ClassImp(BtaCandPtrR)


//----------------
// Constructors --
//----------------
BtaCandPtrR::BtaCandPtrR(Int_t size) :
  RooEvtObj<BtaCandPtr>(),
  _lundId( 0 ),
  _indices(size)

{}
/*
BtaCandPtrR::BtaCandPtrR( const BtaCandPtr* transient,
			  RooEvtObjLocReg & aRegistry ) : 
  RooEvtObj<BtaCandPtr>(),
  _lundId( transient->lundId() ),
  _indices( transient->_indices.length() )
  
{

  // Set the Bool_t variable:
  if( transient->_oneToMany ) _oneToMany= kTRUE;
  else _oneToMany= kFALSE;

  // Set Lund ID:
  Int_t lundId= transient->lundId();
  if( abs( lundId ) > SHRT_MAX ) {
    ErrMsg(fatal) << "lundId " << lundId << " is too large " << endmsg;
  }
  _lundId= lundId;

  // Store indices:
  for( unsigned i= 0; i < transient->_indices.length(); i++ ) {
    Int_t index= transient->_indices[i];
    if( index > SHRT_MAX ) {
      ErrMsg(fatal) << " Index " << index << " too large" << endmsg;
    }
    _indices[i]= index;
  }
  
}
*/

//--------------
// Destructor --
//--------------
BtaCandPtrR::~BtaCandPtrR() {}


//--------------
// Operations --
//--------------
/*
BtaCandPtr* BtaCandPtrR::transient( RooEvtObjLocReg & aRegistry ) const {

  // Set PDT ID:
  PdtLund::LundType lundId= (PdtLund::LundType) _lundId;
  PdtEntry* pdt= 0;
  if( lundId != PdtLund::null ) pdt= Pdt::lookup( lundId );

  // Create new transient:
  BtaCandPtr* transient= new BtaCandPtr( pdt );

  // Set Bool_t variable:
  if( _oneToMany ) transient->setOneToMany();

  // Set index array:
  BtaCandPtrR* self= (BtaCandPtrR*) this;
  for( Int_t i= 0; i < _indices.GetSize(); i++ ) {
    transient->_indices.append( (self->_indices)[i] );
  }

  // The End:
  return transient;

}
*/

//              -------------------------------------------
//              -- Protected Function Member Definitions --
//              -------------------------------------------

//
// Constructor
//
/*
BtaCandPtrR::BtaCandPtrR( const PdtEntry* pdt )
  : _oneToMany(kFALSE) 
  , _lundId(PdtLund::null)
{
  if ( pdt != 0 ) _lundId = pdt->lundId();
}
*/ 

UInt_t
BtaCandPtrR::GetNumberOfDaughters(  )
{
  return _oneToMany? _indices.GetSize()-1:_indices.GetSize();
}

Bool_t
BtaCandPtrR::SetMother(Int_t im)
{
  if(!_oneToMany || ( _indices.GetSize() != 0)) return kFALSE;
  AddDaughter(im);
  return kTRUE;
}

Bool_t
BtaCandPtrR::GetMother(Int_t &im)
{
  if(!_oneToMany || ( _indices.GetSize() == 0)) return kFALSE;
  im = _indices.At(0);
  return kTRUE;
}

Int_t
BtaCandPtrR::operator()( Int_t i )
{
  if (i<GetNumberOfDaughters())
    return _oneToMany?  _indices.At(i+1):_indices.At(i);
  else
    return 0;
}

void 
BtaCandPtrR::AddDaughter(Int_t ind){_indices.AddAt(ind,_indices.GetSize());}
