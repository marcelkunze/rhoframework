//----------*- Mode: c++ -*---------------------------------------------------
// File and Version Information:
// 	$Id: BtaCandAssocMapR.h,v 1.1.1.1 2001-05-15 07:00:47 marcel Exp $
//
// Description:
//	Class BtaCandAssocMapR
//
//      ROOT persistent class for BtaCandAssocMap
//         see TruthTools/BtaCandAssocMap.h
// 
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author Map:
//     Stefan Kluth
//     Rolf Dubitzky                     adaption
//
// Copyright Information:
//
//------------------------------------------------------------------------
#ifndef BTACANDASSOCMAPR_HH
#define BTACANDASSOCMAPR_HH

//----------------------
// Base Class Headers --
//----------------------
#include "KangaSchema/RooEvtObj.h"

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "KangaSchema/BtaCandAssocMap.h"

//------------------------
// Forward declarations --
//------------------------
class RooEvtObjLocReg;


//		---------------------
// 		-- Class Interface --
//		---------------------
 
class BtaCandAssocMapR : public RooEvtObj<BtaCandAssocMap> {
public:
  //
  // Public interface
  //

  // Constructor:
  BtaCandAssocMapR();

  // Destructor:
  virtual ~BtaCandAssocMapR();

  // Make transient:
  virtual BtaCandAssocMap* transient() const;

  int getMicro() const { return _microIndex; }
  int getMcTru() const { return _mcTruIndex; }
  float getConsistency() const { return _consistency; }

private:

  // Data members:
  Short_t _microIndex;
  Short_t _mcTruIndex;
  Float_t _consistency;


public:
  BtaCandAssocMapR( const BtaCandAssocMapR & );
  BtaCandAssocMapR & operator = ( const BtaCandAssocMapR & );

  ClassDef(BtaCandAssocMapR,1)
};


#endif
