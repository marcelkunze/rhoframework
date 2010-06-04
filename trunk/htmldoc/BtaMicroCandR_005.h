//  	-*- Mode: c++ -*-
//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaMicroCandR_005.h,v 1.1.1.1 2002-12-20 15:05:29 marcel Exp $
//
// Description:
//      Class BtaMicroCandR - The persistent version of the BtaMicroCand class.
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
#ifndef BTAMICROCANDR005_HH
#define BTAMICROCANDR005_HH

//-------------
// C Headers --
//-------------
// extern "C" {}

//---------------
// C++ Headers --
//---------------
#include <iosfwd>

//-----------------------------------------
// Registered Package Class Declarations --
//-----------------------------------------

//----------------------
// Base Class Headers --
//----------------------
#include "KangaSchema/RooEvtObj.h"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "KangaSchema/TBtaFitParamsR_001.h"
#include "KangaSchema/TBtaTrkQualR.h"
#include "KangaSchema/TBtaCalQualR.h"
#include "KangaSchema/TBtaPidQualR.h"
#include "KangaSchema/TBtaPidInfoR.h"
#include "KangaSchema/TBtaIfrQualR.h"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class BtaCandidate;
//class RooEvtObjLocReg;

//		---------------------
// 		-- Class Interface --
//		---------------------

class BtaMicroCandR_005 : public RooEvtObj<BtaCandidate> {
//--------------------
// Instance Members --
//--------------------

public:

  //
  // Constructors
  // aVertexFactory and aTruthFactory arguments are for compatibility with 
  // other subclasses of BtaCandBaseP and are ignored
  //
  BtaMicroCandR_005();
  //BtaMicroCandR( const BtaCandidate*, RooEvtObjLocReg & reg );

  // Destructor
  virtual ~BtaMicroCandR_005();

  // Operators
    
  // Selectors (const)
  //virtual BtaCandidate* transient( RooEvtObjLocReg & ) const;
  
  virtual void print( ostream & ost ) const;
  virtual void Print( Option_t* option="" ) const;

  // Modifiers:
  //virtual Bool_t fillRefs( const BtaCandidate*, const RooEvtObjLocReg & );
  //virtual Bool_t fillPointers( BtaCandidate*, const RooEvtObjLocReg & ) const;

private:

  // Data members:
  TBtaFitParamsR_001 _fitParams;  // Fit parameters
  UChar_t _subObjStatus;     // Status flags for contained objects
  TBtaTrkQualR _trkQual;      // Track quality
  TBtaCalQualR _calQual;      // Calorimeter quality 
  TBtaPidQualR _pidQual;      // PID quality
  TBtaPidInfoR _pidInfo;      // PID info
  TBtaIfrQualR _ifrQual;      // Ifr quality

  // disallowed:
  BtaMicroCandR_005( const BtaMicroCandR_005 & );
  const BtaMicroCandR_005 & operator = ( const BtaMicroCandR_005 & );
  
  friend class BtaRooMicroAdapter;

  ClassDef(BtaMicroCandR_005,1) //Persistent Kanga MicroCandidate

};

#endif
