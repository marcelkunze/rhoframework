//----------*- Mode: c++ -*---------------------------------------------------
// File and Version Information:
// 	$Id: BtaCandPtrR.h,v 1.1.1.1 2001-05-15 07:01:29 marcel Exp $
//
// Description:
//	Class BtaCandPtrR
//
//      Contains list of indices to BtaMicroCands
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
#ifndef BTACANDPTRR_HH
#define BTACANDPTRR_HH


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
#include "TArrayS.h"


//------------------------
// Forward declarations --
//------------------------
class BtaCandPtr;
//class RooEvtObjLocReg;


//		---------------------
// 		-- Class Interface --
//		---------------------

class BtaCandPtrR : public RooEvtObj<BtaCandPtr> {
    
public:
    //
    // Public interface
    //
    
    // Constructor:
    BtaCandPtrR(Int_t size = 5);
    //BtaCandPtrR( const BtaCandPtr*, RooEvtObjLocReg & );
    
    // Destructor:
    virtual ~BtaCandPtrR();
    
    // Make transient:
    //virtual BtaCandPtr* transient( RooEvtObjLocReg & ) const;
    
private:
    
    // Data members:
    Bool_t _oneToMany;      // is it a composite or a cluster - bump match?
    Short_t _lundId;        // Pdt IDs
    TArrayS _indices;       // Array of indices
    
    // disallowed:
    BtaCandPtrR( const BtaCandPtrR & );
    BtaCandPtrR & operator = ( const BtaCandPtrR & );
    
public:
    Bool_t OneToMany() { return _oneToMany; }
    Int_t  GetLundId() { return _lundId; }
    UInt_t GetNumberOfDaughters();
    Int_t operator()(Int_t i);
    Bool_t SetMother(int im);
    Bool_t GetMother(int &im); 
    void AddDaughter(Int_t ind);
    
    ClassDef(BtaCandPtrR,1) //Persistent pointer to BetaCandidate
	
    friend class BtaCandListR_001;
    
};


#endif
