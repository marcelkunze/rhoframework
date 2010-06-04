//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaMicroCandR_003.cxx,v 1.2 2001-12-17 17:43:37 Marcel Exp $
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
#include "KangaSchema/BtaMicroCandR_003.h"

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

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
//#include "Beta/BtaCandidate.h"
//#include "Beta/BtaMicroCand.h"
//#include "BetaMicroAdapter/BtaTrkQual.h"
//#include "BetaMicroAdapter/BtaCalQual.h"
//#include "BetaMicroAdapter/BtaPidQual.h"
//#include "BetaMicroAdapter/BtaPidInfo.h"
//#include "BetaMicroAdapter/BtaIfrQual.h"

//#include "ErrLogger/ErrLog.h"
//#include "RooUtils/RooEvtObjLocReg.h"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------
ClassImp(BtaMicroCandR_003)

//		-----------------------------------------------
// 		-- Static Data & Function Member Definitions --
//		-----------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
BtaMicroCandR_003::BtaMicroCandR_003() :
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
BtaMicroCandR_003::BtaMicroCandR_003( const BtaCandidate* aCandidate,
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
BtaMicroCandR_003::~BtaMicroCandR_003() {}

//-------------
// Operators --
//-------------

//-------------
// Selectors --
//-------------
/*
BtaCandidate* BtaMicroCandR_003::transient( RooEvtObjLocReg & reg ) const 
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
Bool_t BtaMicroCandR_003::fillRefs( const BtaCandidate* trans, 
			      const RooEvtObjLocReg & reg ) {
  // don't do anything until you have persistent references
  return true;
}
Bool_t BtaMicroCandR_003::fillPointers( BtaCandidate* trans, 
				  const RooEvtObjLocReg & reg ) const {
  // don't do anything until you have persistent references
  return true;
}

*/
// Printing:
void BtaMicroCandR_003::print( std::ostream & ost ) const {
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
void BtaMicroCandR_003::Print( Option_t* option ) const {
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
