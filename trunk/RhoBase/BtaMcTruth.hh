#ifndef BtaMcTruth_hh
#define BtaMcTruth_hh
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// BtaMcTruth								//
//                                                                      //
// Monte Carlo truth class						//
//									//
// Author List:								//
// Marcel Kunze, Bochum University, Aug. 99				//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "TObject.h"
#include "TObjArray.h"
#include "TLorentzVector.h"

class PdtEntry;

class BtaMcTruth : public TObject {
    
public:
    BtaMcTruth();
    virtual ~BtaMcTruth();
    virtual void printOn( std::ostream& o=std::cout ) const = 0;

    virtual BtaMcTruth*   getMother()        const = 0;
    virtual TObjArray*   getDaughterArray() const = 0;
    virtual BtaMcTruth*   getDaughter( const Int_t i ) const = 0;
    virtual Int_t        getNumberOfDaughters() const = 0;
    
    virtual Int_t getStatus()         const = 0;    // Particle status
    virtual PdtEntry* getPdtEntry()   const = 0; 
    virtual Double_t getCharge()      const = 0;
    virtual Double_t getPx()          const = 0;    // X momentum
    virtual Double_t getPy()          const = 0;    // y momentum
    virtual Double_t getPz()          const = 0;    // z momentum
    virtual Double_t getEnergy()      const = 0;    // Track energy
    virtual Double_t getMass()        const = 0;    // Particle mass
    virtual TLorentzVector getMomentum() const = 0;
    virtual Double_t getStartVx()     const = 0;    // x position of vertex
    virtual Double_t getStartVy()     const = 0;    // y position of vertex
    virtual Double_t getStartVz()     const = 0;    // z position of vertex
    virtual TVector3 getStartVertex() const = 0;
    virtual Double_t getLifeTime()    const = 0;    // Production time
    virtual Int_t getAodMatchIndex()  const = 0;    // Track number of AOD Parter (if none: -1)
    virtual Double_t getAodMatchConsistency()  const = 0;   // Track number of AOD Parter (if none: -1)
    
public:
    ClassDef(BtaMcTruth,1) //Monte-Carlo truth
	
};      

// standalone print
std::ostream&  operator << (std::ostream& o, const BtaMcTruth&);

#endif
