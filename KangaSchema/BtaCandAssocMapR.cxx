//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaCandAssocMapR.cxx,v 1.1.1.1 2001-05-15 07:01:45 marcel Exp $
//
// Description:
//      see BtaCandAssocMapR.h
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Rolf Dubitzky
//
// Copyright Information:
//
//------------------------------------------------------------------------

//-----------------------
// This Class's Header --
//-----------------------
#include "KangaSchema/BtaCandAssocMapR.h"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------
ClassImp(BtaCandAssocMapR)


//----------------
// Constructors --
//----------------
BtaCandAssocMapR::BtaCandAssocMapR() :
  RooEvtObj<BtaCandAssocMap>(),
  _microIndex(0),
  _mcTruIndex(0),
  _consistency(0)
{}


//--------------
// Destructor --
//--------------
BtaCandAssocMapR::~BtaCandAssocMapR() 
{}


//--------------
// Operations --
//--------------
BtaCandAssocMap* BtaCandAssocMapR::transient() const {

    BtaCandAssocMap* transient= new BtaCandAssocMap( _microIndex, _mcTruIndex, _consistency );
    return transient;

}


BtaCandAssocMapR::BtaCandAssocMapR( const BtaCandAssocMapR &other ) {
  _microIndex=other._microIndex;
  _mcTruIndex=other._mcTruIndex;
  _consistency=other._consistency;
}

BtaCandAssocMapR & BtaCandAssocMapR::operator = ( const BtaCandAssocMapR &other ) {
  _microIndex=other._microIndex;
  _mcTruIndex=other._mcTruIndex;
  _consistency=other._consistency;
  return *this;
}

//              -------------------------------------------
//              -- Protected Function Member Definitions --
//              -------------------------------------------
