//      -*- Mode: c++ -*-
//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: StdHepTrkR.h,v 1.3 2002-02-01 03:47:50 marcel Exp $
//
// Description:
//	Class StdHepTrkR
//
//      a MC truth track
// 
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//     Gautier Hamel de Monchenault
//
// Copyright Information:
//	Copyright (C) 1999                LBNL
//
//------------------------------------------------------------------------
#ifndef StdHepTrkR_RDL
#define StdHepTrkR_RDL


//----------------------
// Base Class Headers --
//----------------------
#include "KangaSchema/RooEvtObj.h"


//---------------
// C++ Headers --
//---------------
#include <iosfwd>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
class StdHepTrk;
class RooEvtObjLocReg;


//		---------------------
// 		-- Class Interface --
//		---------------------

class StdHepTrkR  : public RooEvtObj<StdHepTrk> {
    
public:
    //
    // Public interface
    //
    
    // constructors
    StdHepTrkR();
    //StdHepTrkR( const StdHepTrk*, RooEvtObjLocReg & reg );
    
    // destructor
    virtual ~StdHepTrkR();
    
    // get transient 
    //virtual StdHepTrk* transient( RooEvtObjLocReg& ) const;
    int getStatus() const {return _isthep;}         //Particle status
    int getId() const {return _idhep;}              // Particel Id
    int getFirstMother() const {return _jmohep[0];}  // Positionn of first mother
    int getSecondMother() const {return _jmohep[1];} // Position of second mother
    int getFirstDaughter() const {return _jdahep[0];}// Position of first daughter
    int getLastDaughter() const {return _jdahep[1];} // Position of last daughter
    double getPx() const {return _phep[0];}          // X momentum
    double getPy() const {return _phep[1];}          // y momentum
    double getPz() const {return _phep[2];}          // z momentum
    double getEnergy() const {return _phep[3];}      // Track energy
    double getMass() const {return _phep[4];}        // Particle mass
    double getVx() const {return _vhep[0];}          // x position of vertex
    double getVy() const {return _vhep[1];}          // y position of vertex
    double getVz() const {return _vhep[2];}          // z position of vertex
    double getTime() const {return _vhep[3];}        // Production time
    int getTrackNumber() const {return _trkno;}     // Track number
    
    // print-outs
    virtual void print( ostream & ost ) const;
    virtual void Print( Option_t* option="" ) const;
    
    // modifiers
    
private:  
    
    // Data members:
    Short_t _isthep;         // status code 
    Int_t _idhep;            // The particle id 
    UShort_t _trkno;         // Track Number
    UShort_t _jmohep[2];     // The position of the mother particle in list 
    UShort_t _jdahep[2];     // Position of the first daughter... in list 
    Float_t _phep[5];        // 4-Momentum, mass 
    Float_t _vhep[4];        // Vertex information 
    
    // helper
    //  void fillDataMembers( const StdHepTrk* );
    
    // Disallowed:
    StdHepTrkR( const StdHepTrkR & );
    StdHepTrkR & operator = ( const StdHepTrkR & );
    
    // Special ROOT macro:
    ClassDef(StdHepTrkR,1) // persistent stdhep track
	
};

#endif
