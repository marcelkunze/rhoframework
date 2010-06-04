//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: EidInfoR.cxx,v 1.2 2001-12-17 17:45:31 Marcel Exp $
//
// Description:
//      Class EidInfoR - The persistent version of the BtaMicroCand class.
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

// This class header:
#include "KangaSchema/EidInfoR.h"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------
ClassImp(EidInfoR)

#include <iostream>
using namespace std;
//		-----------------------------------------------
// 		-- Static Data & Function Member Definitions --
//		-----------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
EidInfoR::EidInfoR() : 
  RooEvtObj<AbsEventID>(),
  _run( 0 ),
  _configKey( 0 ),
  _platform( 0 ),
  _partitionMask( 0 ),
  _timeStamp_u( 0 ),
  _timeStamp_l( 0 ),
  _bdbTimeGmtSec( 0 ),
  _bdbTimeGmtNSec( 0 ) {}
/*
EidInfoR::EidInfoR( const AbsEventID* eventID, RooEvtObjLocReg & reg ) :
  RooEvtObj<AbsEventID>(),
  _run( 0 ),
  _configKey( 0 ),
  _platform( 0 ),
  _partitionMask( 0 ),
  _timeStamp_u( 0 ),
  _timeStamp_l( 0 ),
  _bdbTimeGmtSec( 0 ),
  _bdbTimeGmtNSec( 0 )

{

  // Register this object and its corresponding transient:
  // Doesn't work, AbsEventID is not an AbsEvtObj (don't ask, 
  // I don't know why)
  //  registerThis( eventID, reg );

  // Run # and configkey:
  _run= eventID->run();
  _configKey= eventID->configKey();

  // Stuff from EidEventTriplet:
  EidEventTriplet eidEvt= eventID->eventIdTriplet();
  _platform= eidEvt.platform();
  _partitionMask= eidEvt.partitionMask();
  odfTime timeStamp= eidEvt.timeStamp();
  _timeStamp_u= timeStamp.binary().upper;
  _timeStamp_l= timeStamp.binary().lower;

  // Stuff from EidCondKeyTriplet, also contains platform and 
  // partitionMask info, why this duplication? Anyway, just get
  // the BdbTime:
  BdbTime bdbTime= eventID->condKeyTriplet().key();
  _bdbTimeGmtSec= bdbTime.getGmtSec();
  _bdbTimeGmtNSec= bdbTime.getGmtNsec();

}
*/
//--------------
// Destructor --
//--------------
EidInfoR::~EidInfoR() {}

/*
// Create transient:
AbsEventID* EidInfoR::transient( RooEvtObjLocReg & reg ) const {

  // Create EidEventTriplet:
  odfTime timeStamp( _timeStamp_u, _timeStamp_l );
  EidEventTriplet eidEvt( _platform, _partitionMask, timeStamp );

  // Create EidCondKeyTriplet:
  BdbTime bdbTime( _bdbTimeGmtSec, _bdbTimeGmtNSec );
  EidCondKeyTriplet eidCond( _platform, _partitionMask, bdbTime );

  // Create transient AbsEventID as EidInfo object:
  AbsEventID* result= new EidInfo( eidEvt, eidCond, _configKey, _run );

  // Register this object and its corresponding transient:
  // Doesn't work, AbsEventID is not an AbsEvtObj (don't ask, I don't why)
  //  registerThis( result, reg );

  // The End:
  return result;

}

// FillRefs and FillPointers dummies:
Bool_t EidInfoR::fillRefs( const AbsEventID*, const RooEvtObjLocReg & ) {

  return true;

}
Bool_t EidInfoR::fillPointers( AbsEventID*, const RooEvtObjLocReg & ) const {

  return true;

}
*/
// Print object:
void EidInfoR::PrintOn( std::ostream & ost ) const {

  ost << "EidInfoR: contents: " << endl;
  ost << "Run: " << _run << endl;
  ost << "ConfigKey: " << _configKey << endl;
  ost << "Platform: " << _platform << endl;
  ost << "PartitionMask: " << _partitionMask << endl;
  ost << "Timestamps (upper,lower): " << _timeStamp_u << " " << _timeStamp_l 
      << endl;
  ost << "BdbTime (sec, nsec): " << _bdbTimeGmtSec << " " << _bdbTimeGmtNSec
      << endl;

}

