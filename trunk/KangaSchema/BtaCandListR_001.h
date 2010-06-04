//----------*- Mode: c++ -*---------------------------------------------------
// File and Version Information:
// 	$Id: BtaCandListR_001.h,v 1.1.1.1 2001-05-15 07:01:55 marcel Exp $
//
// Description:
//	Class BtaCandListR_001
//
//      ROOT persistent class for BtaMicroCand maps used by
//      BtaMicroDispatch
// 
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//     Stefan Kluth   
//
// Copyright Information:
//	Copyright (C) 1999                LBNL
//
//------------------------------------------------------------------------
#ifndef BTACANDLISTR_001_HH
#define BTACANDLISTR_001_HH

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
#include "TObjString.h"
#include "TArrayC.h"
#include "TArrayS.h"


//------------------------
// Forward declarations --
//------------------------
class BtaCandList;
class BtaCandPtrR;
class RooEvtObjLocReg;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class BtaCandListR_001 : public RooEvtObj<BtaCandList> {
public:
  //
  // Public interface
  //

  // Constructor:
  BtaCandListR_001();
  //BtaCandListR_001( const BtaCandList*, RooEvtObjLocReg & );

  // Destructor:
  virtual ~BtaCandListR_001();

  // Make transient:
  //virtual BtaCandList* transient( RooEvtObjLocReg & ) const;

private:

  // Data members:
  TObjString _key;        // List key
  TArrayC    _indices;    //
  TArrayS    _lundIds;    //
  Bool_t     _oneToMany;  //

  // disallowed:
  BtaCandListR_001( const BtaCandListR_001 & );
  BtaCandListR_001 & operator = ( const BtaCandListR_001 & );

public:
  TString GetKey() { return _key.GetString(); }
  UInt_t GetNumberOfCandidates() { return _lundIds.GetSize(); }
  BtaCandPtrR* GetCandPtr(UInt_t i);

  ClassDef(BtaCandListR_001,1) //Persistent list of BetaCandidates

};


#endif
