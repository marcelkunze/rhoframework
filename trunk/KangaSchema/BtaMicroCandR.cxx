//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaMicroCandR.cxx,v 1.2 2001-12-17 17:42:53 Marcel Exp $
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Stefan Kluth           Originator
//
// Copyright Information:
//	Copyright (C) 1999     LBNL
//
//------------------------------------------------------------------------

//-----------------------
// This Class's Header --
//-----------------------
#include "KangaSchema/BtaMicroCandR.h"

//-------------
// C Headers --
//-------------
// extern "C" {}

//---------------
// C++ Headers --
//---------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------
ClassImp(BtaMicroCandR)

//		-----------------------------------------------
// 		-- Static Data & Function Member Definitions --
//		-----------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
BtaMicroCandR::BtaMicroCandR() :
  RooEvtObj<BtaCandidate>(), 
  _fitParams(),
  _subObjStatus( 0 ), 
  _trkQual(), 
  _calQual(), 
  _pidQual(), 
  _pidInfo(),
  _ifrQual()
{}
/*
BtaMicroCandR::BtaMicroCandR( const BtaCandidate* aCandidate,
			      RooEvtObjLocReg & reg ) : 
  RooEvtObj<BtaCandidate>(),
  _fitParams( aCandidate->content()->_params ),
  _subObjStatus( 0 ), 
  _trkQual(), 
  _calQual(), 
  _pidQual(), 
  _pidInfo(),
  _ifrQual()

{

  // Register this object and its corresponding transient:
  registerThis( aCandidate, reg );

  // Create sub-objects:
  const BtaMicroAdapter* microAdapter= aCandidate->getMicroAdapter();
  if( microAdapter != 0 ) {
    const BtaTrkQual* btqp= microAdapter->getTrkQual();
    if( btqp != 0 ) {
      _trkQual= BtaTrkQualR( btqp );
      _subObjStatus |= ( 0x1 << 0 );
    }
    const BtaCalQual* bcqp= microAdapter->getCalQual();
    if( bcqp != 0 ) {
      _calQual= BtaCalQualR( bcqp );
      _subObjStatus |= ( 0x1 << 1 );
    }
    const BtaPidQual* bpqp= microAdapter->getPidQual();
    if( bpqp != 0 ) {
      _pidQual= BtaPidQualR( bpqp );
      _subObjStatus |= ( 0x1 << 2 );
    }
    const BtaPidInfo* bpip= microAdapter->getPidInfo();
    if( bpip != 0 ) {
      _pidInfo= BtaPidInfoR( bpip );
      _subObjStatus |= ( 0x1 << 3 );
    }
    const BtaIfrQual* biqp= microAdapter->getIfrQual();
    if( biqp != 0 ) {
      _ifrQual= BtaIfrQualR( biqp );
      _subObjStatus |= ( 0x1 << 4 );
    }
  }
  else {
    ErrMsg(error) << " Could not get BtaMicroAdapter" << endmsg;
  }

}
*/

//--------------
// Destructor --
//--------------
BtaMicroCandR::~BtaMicroCandR() {}

//-------------
// Operators --
//-------------

//-------------
// Selectors --
//-------------
/*
BtaCandidate* BtaMicroCandR::transient( RooEvtObjLocReg & reg ) const 
{

  //
  // Modified by Gautier (27-09-99)
  // Implementation in terms of BtaRooMicroAdapter class
  //

  BtaCandidate* result= 0;
  
  BtaFitParams* fitParams= _fitParams.transient();
  if( fitParams != 0 ) {

    // Create the Root micro adapter
    BtaMicroAdapter* microAdapter = new BtaRooMicroAdapter( *this );

    // Create the BtaMicroCand base:
    BtaCandBase* base= new BtaMicroCand( *fitParams, microAdapter ); 

    delete fitParams;
    result= new BtaCandidate( base );

  } 
  else {
    ErrMsg(error) << " Could not create transient BtaFitParams object"
		  << endmsg;
  }

  // Register this object and its corresponding transient:
  registerThis( result, reg );

  // return new BtaMicroCand
  return result;

}


// fillRefs and fillPointers:
Bool_t BtaMicroCandR::fillRefs( const BtaCandidate* trans, 
			      const RooEvtObjLocReg & reg ) {
  // don't do anything until you have persistent references
  return true;
}
Bool_t BtaMicroCandR::fillPointers( BtaCandidate* trans, 
				  const RooEvtObjLocReg & reg ) const {
  // don't do anything until you have persistent references
  return true;
}

*/
// Printing:
void BtaMicroCandR::print( std::ostream & ost ) const {
/*
  _fitParams.print( ost );
  if( _subObjStatus & ( 0x1 << 0 ) ) {
    BtaTrkQual* btq= _trkQual.transient();
    btq->print( ost );
    delete btq;
  }
  if( _subObjStatus & ( 0x1 << 1 ) ) {
    BtaCalQual* bcq= _calQual.transient();
    bcq->print( ost );
    delete bcq;
  }
  if( _subObjStatus & ( 0x1 << 2 ) ) {
    BtaPidQual* bpq= _pidQual.transient();
    bpq->print( ost );
    delete bpq;
  }
  if( _subObjStatus & ( 0x1 << 3 ) ) {
    BtaPidInfo* bpi= _pidInfo.transient();
    bpi->print( ost );
    delete bpi;
  }
  if( _subObjStatus & ( 0x1 << 4 ) ) {
    BtaIfrQual* biq= _ifrQual.transient();
    biq->print( ost );
    delete biq;
  }
*/
  return;

}
void BtaMicroCandR::Print( Option_t* option ) const {
/*
  cout << ClassName() << endl;
  print( cout );
  return;
*/
}



//		-----------------------------------------
// 		-- Private Function Member Definitions --
//		-----------------------------------------

//		-----------------------------------
// 		-- Internal Function Definitions --
//		-----------------------------------
