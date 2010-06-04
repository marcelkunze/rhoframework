//  	-*- Mode: c++ -*-
//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: EidInfoR.h,v 1.1.1.1 2002-12-20 15:05:29 marcel Exp $
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
#ifndef EIDINFOR_HH
#define EIDINFOR_HH

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

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class AbsEventID;
//class RooEvtObjLocReg;

//		---------------------
// 		-- Class Interface --
//		---------------------

class EidInfoR : public RooEvtObj<AbsEventID> {
    //--------------------
    // Instance Members --
    //--------------------
    
public:
    
    // Constructors:
    EidInfoR();
    //EidInfoR( const AbsEventID*, RooEvtObjLocReg & );
    
    // Destructor
    virtual ~EidInfoR();
    
    // Operators
    
    // Selectors (const)
    UInt_t GetRunNumber() { return _run; }
    UInt_t GetCongifKey() { return _configKey; }
    UInt_t GetPlatform() { return _platform; }
    UInt_t GetPartitionMask() { return _partitionMask; }
    UInt_t GetMajorOdf() { return _timeStamp_u; }
    UInt_t GetMinorOdf() { return _timeStamp_l; }
    UInt_t GetMajorID() { return _bdbTimeGmtSec; }
    UInt_t GetMinorID() { return _bdbTimeGmtNSec; }
    
    //virtual AbsEventID* transient( RooEvtObjLocReg & ) const;
    
    virtual void PrintOn( ostream & ost ) const;

    // Modifiers:
    //virtual Bool_t fillRefs( const AbsEventID*, const RooEvtObjLocReg & );
    //virtual Bool_t fillPointers( AbsEventID*, const RooEvtObjLocReg & ) const;
    
private:
    
    // Data members (In BaBar/BaBarODMGTypes.h d_ULong is typedef'ed to
    // unsigned int, this corresponds to UInt_t in ROOT):
    UInt_t _run;
    UInt_t _configKey;
    UInt_t _platform;
    UInt_t _partitionMask;
    UInt_t _timeStamp_u;
    UInt_t _timeStamp_l;
    UInt_t _bdbTimeGmtSec;
    UInt_t _bdbTimeGmtNSec;
    
    // disallowed:
    EidInfoR( const EidInfoR & );
    const EidInfoR & operator = ( const EidInfoR & );
    
    ClassDef(EidInfoR,1)  //Persistent Kanga event header
	
};

#endif
