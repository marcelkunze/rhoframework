//----------*- Mode: c++ -*---------------------------------------------------
// File and Version Information:
// 	$Id: BtaCandListR.h,v 1.1.1.1 2001-05-15 07:01:31 marcel Exp $
//
// Description:
//	Class BtaCandListR
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
#ifndef BTACANDLISTR_HH
#define BTACANDLISTR_HH

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
#include "TObjArray.h"


//------------------------
// Forward declarations --
//------------------------
class BtaCandList;
class BtaCandPtrR;
class RooEvtObjLocReg;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class BtaCandListR : public RooEvtObj<BtaCandList> {
public:
  //
  // Public interface
  //

  // Constructor:
  BtaCandListR();
  //BtaCandListR( const BtaCandList*, RooEvtObjLocReg & );

  // Destructor:
  virtual ~BtaCandListR();

  // Make transient:
  //virtual BtaCandList* transient( RooEvtObjLocReg & ) const;

private:

  // Data members:
  TObjString _key;        // List key
  TObjArray _array;       // Array of persistent BtaCandPtrR

  // disallowed:
  BtaCandListR( const BtaCandListR & );
  BtaCandListR & operator = ( const BtaCandListR & );

public:
  TString GetKey() { return _key.GetString(); }
  UInt_t GetNumberOfCandidates() { return _array.GetEntries(); }
  BtaCandPtrR* GetCandPtr(UInt_t i) { return (BtaCandPtrR *) _array.At(i); }

  ClassDef(BtaCandListR,1) //Persistent list of BetaCandidates

};


#endif
