//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaCandListR.cxx,v 1.1.1.1 2001-05-15 07:01:56 marcel Exp $
//
// Description:
//      Stores list of BtaCandPtr for the packing/unpacking of the
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
#include "KangaSchema/BtaCandListR.h"


//-------------
// C Headers --
//-------------
// extern "C" {}


//---------------
// C++ Headers --
//---------------


//-------------------------------
// Collaborating Class Headers --
//-------------------------------
//#include "RooUtils/RooEvtObjLocReg.h"
#include "KangaSchema/BtaCandPtrR.h"
//#include "BetaTools/BtaCandList.h"
//#include "BetaTools/BtaCandPtr.h"
//#include "ProxyDict/IfdStrKey.h"
//#include "PDT/Pdt.h"
//#include "ErrLogger/ErrLog.h"


//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------
ClassImp(BtaCandListR)


//----------------
// Constructors --
//----------------
BtaCandListR::BtaCandListR() :
  RooEvtObj<BtaCandList>(),
  _key(),
  _array()

{}
/*
BtaCandListR::BtaCandListR( const BtaCandList* transient,
			    RooEvtObjLocReg & aRegistry ) : 
   RooEvtObj<BtaCandList>(),
   _key( transient->name() ), 
   _array( transient->length() )

{

  // Store the location of this object, and the corresponding BtaCandList
  // into the registry:
  registerThis( transient, aRegistry ) ;

  for( unsigned i= 0; i < transient->length(); i++ ) {

    // Store persistent BtaCandPtr in array:
    BtaCandPtr* bcp= transient->_l[i];
    _array.AddAt( new BtaCandPtrR( bcp, aRegistry ), i );

  }

}
*/

//--------------
// Destructor --
//--------------
BtaCandListR::~BtaCandListR() {

  _array.Delete();

}


//--------------
// Operations --
//--------------
/*
BtaCandList* BtaCandListR::transient( RooEvtObjLocReg & aRegistry ) const {

  // Create transient object:
  IfdStrKey key( _key.GetString().Data() );
  BtaCandList* transient= new BtaCandList( &key );

  // And fill it with data:
  for( unsigned i= 0; i < _array.GetEntriesFast(); i++ ) {

    // Get persistent BtaCandPtrR from array and make corresponding
    // transient BtaCandPtr object:
    BtaCandPtr* bcp= 0;
    BtaCandListR* self= (BtaCandListR*) this;
    const TObject* obj= (self->_array)[i];
    if( obj->IsA() == BtaCandPtrR::Class() ) {
      const BtaCandPtrR* bcpr= (const BtaCandPtrR*) obj;
      bcp= bcpr->transient( aRegistry );
    }
    else {
      ErrMsg(fatal) << " error, object in TObjArray is not a BtaCandPtrR "
		    << obj->ClassName() << endl;
    }

    // Add BtaCandPtr to transient BtaCandList:
    transient->append( bcp );

  }

  // Register new transient object in registry:
  registerThis( transient, aRegistry );

  return transient;

}
*/

//              -------------------------------------------
//              -- Protected Function Member Definitions --
//              -------------------------------------------
