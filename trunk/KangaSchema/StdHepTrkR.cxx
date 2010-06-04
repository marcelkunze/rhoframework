//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: StdHepTrkR.cxx,v 1.2 2001-12-17 17:46:28 Marcel Exp $
//
// Description:
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Gautier Hamel de Monchenault
//
// Copyright Information:
//	Copyright (C) 1999       Saclay
//
//------------------------------------------------------------------------

//-----------------------
// This Class's Header --
//-----------------------
#include "KangaSchema/StdHepTrkR.h"


//-------------
// C Headers --
//-------------
#include <math.h>

//---------------
// C++ Headers --
//---------------


//-------------------------------
// Collaborating Class Headers --
//-------------------------------
//#include "StdHepData/StdHepTrk.h"


//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------
ClassImp(StdHepTrkR)

#include <iostream>
using namespace std;

//----------------
// Constructors --
//----------------
StdHepTrkR::StdHepTrkR() : 
  RooEvtObj<StdHepTrk>(),
  _isthep( 0 ),
  _idhep( 0 ),
  _trkno( 0 )
{
  _jmohep[0]=_jmohep[1]=0;
  _jdahep[0]=_jdahep[1]=0;
  _phep[0]=_phep[1]=_phep[2]=_phep[3]=_phep[4]=0;
  _vhep[0]=_vhep[1]=_vhep[2]=_vhep[3]=0;
}
/*
StdHepTrkR::StdHepTrkR( const StdHepTrk* g,  RooEvtObjLocReg & reg  ) :  
  RooEvtObj<StdHepTrk>(),
  _isthep( g->getStatus() ),
  _idhep( g->getId() ),
  _trkno( g->getTrackNumber() )
{
  // Register this object and its corresponding transient:
  registerThis( g, reg );
  
  // Fill arrays:
  _jmohep[0] = g->getFirstMother();
  _jmohep[1] = g->getSecondMother();
  _jdahep[0] = g->getFirstDaughter();
  _jdahep[1] = g->getLastDaughter();
  _phep[0]   = g->getPx();
  _phep[1]   = g->getPy();
  _phep[2]   = g->getPz();
  _phep[3]   = g->getEnergy();
  _phep[4]   = g->getMass();
  _vhep[0]   = g->getVx();
  _vhep[1]   = g->getVy();
  _vhep[2]   = g->getVz();
  _vhep[3]   = g->getTime();
}
*/
//--------------
// Destructor --
//--------------
StdHepTrkR::~StdHepTrkR() {}

//--------------
// Operations --
//--------------
/*
StdHepTrk* StdHepTrkR::transient( RooEvtObjLocReg & reg ) const {

  // Create the transient object:
  StdHepTrk* theStdTrk= new StdHepTrk();

  // Fill it with data:
  theStdTrk->setStatus( _isthep );
  theStdTrk->setId( _idhep );
  theStdTrk->setFirstMother( _jmohep[0] );
  theStdTrk->setSecondMother( _jmohep[1] );
  theStdTrk->setFirstDaughter( _jdahep[0] );
  theStdTrk->setLastDaughter( _jdahep[1] );
  theStdTrk->setPx( _phep[0] );
  theStdTrk->setPy( _phep[1] );
  theStdTrk->setPz( _phep[2] );
  theStdTrk->setEnergy( _phep[3] );
  theStdTrk->setMass( _phep[4] );
  theStdTrk->setVx( _vhep[0] );
  theStdTrk->setVy( _vhep[1] );
  theStdTrk->setVz( _vhep[2] );
  theStdTrk->setTime( _vhep[3] );
  theStdTrk->setTrackNumber( _trkno );

  // Register this object and its corresponding transient:
  registerThis( theStdTrk, reg );

  // The End:
  return theStdTrk;

}
*/

// Printing:
void StdHepTrkR::print( std::ostream & ost ) const {

  return;

}

void StdHepTrkR::Print( Option_t* option ) const {

  cout << ClassName() << endl;
  print( cout );
  return;

}

