//      -*- Mode: c++ -*-
//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: StdHepR.h,v 1.1.1.1 2002-12-20 15:05:29 marcel Exp $
//
// Description:
//	Class StdHepR
//
// 
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//     Gautier Hamel de Monchenault
//
// Copyright Information:
//	Copyright (C) 1999               Saclay
//
//------------------------------------------------------------------------
#ifndef StdHepR_RDL
#define StdHepR_RDL


//----------------------
// Base Class Headers --
//----------------------
#include "KangaSchema/RooClonesClassObj.h"


//---------------
// C++ Headers --
//---------------
#include <iosfwd>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "TClonesArray.h"
class StdHep;
class StdHepTrk;
class RooEvtObjLocReg;


//		---------------------
// 		-- Class Interface --
//		---------------------

class StdHepR  : public RooClonesClassObj< StdHep > {
    
public:
    //
    // Public interface
    //
    
    // constructors
    StdHepR();
    //StdHepR( const StdHep*, RooEvtObjLocReg & reg );
    
    // destructor
    virtual ~StdHepR();
    
    // Accessors:
    //virtual StdHep* transient( RooEvtObjLocReg & ) const;
    virtual TClonesArray* getArray() const { return _array; }
    int getEvent() const {return _eventNumber;}          // Event number 
    
    // print-outs
    virtual void print( ostream & ost ) const;
    virtual void Print( Option_t* option="" ) const;
    
    // modifiers
    
private:  
    
    // Helper:
    virtual void initialiseArray();
    
    // Data members
    Int_t _eventNumber;   // Event number from simulation
    
    // Disallowed:
    StdHepR( const StdHepR & );
    StdHepR & operator = ( const StdHepR & );
    
    // Special ROOT macro:
    ClassDef(StdHepR,1) // persistent stdhep structure
	
};

#endif
