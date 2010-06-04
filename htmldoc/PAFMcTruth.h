#ifndef PAFMCTRUTH_H
#define PAFMCTRUTH_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFMcTruth								//
//                                                                      //
// Monte Carlo truth class in PAF(Pico Analysis Framework)		//
//									//
// Author List:								//
// Marcel Kunze, Bochum University, Aug. 99				//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

// std
#include <iosfwd>
// ROOT stuff
#include "TObject.h"
#include "TObjArray.h"
// PAF
#include "PAFSchema/PAFMcBase.h"
#include "TLorentzVector.h"

#include "RhoBase/VAbsTruth.h"

class TDatabasePDG;
class TParticlePDG;

class PAFMcTruth : public VAbsTruth {
    
public:
    //Constructor
    PAFMcTruth();
    PAFMcTruth( PAFMcBase& );
    //Destructor
    virtual ~PAFMcTruth();
    void PrintOn(std::ostream& o=std::cout) const;

    // Setter
    void SetMother(PAFMcTruth* mom) { fMother = mom; }
    void AddDaughter(PAFMcTruth* d) { fDaughters->Add(d); }   
    
    // Getter
    PAFMcBase*    GetMcBase()        const { return fMyBase; }   
    VAbsTruth*    GetMother()        const { return fMother; }
    // To loop over all daughers, you can either create a
    // >>   TObjArrayIter daughters( GetDaughterArray() );
    // or do a classic loop  i<GetNumberOfDaughters  { GetDaughter(i) }
    TObjArray*    GetDaughterArray() const { return fDaughters; }
    VAbsTruth*    GetDaughter( const Int_t i )  const { return ( (VAbsTruth*)(*fDaughters)[i] ); };
    int           GetNumberOfDaughters() const { return ( fDaughters->GetEntriesFast() ); };
    
    //  Interface Getter functions to the McBase
    //  If you really need to Set something, go get the McBase
    Int_t GetStatus()         const { return( fMyBase ? fMyBase->GetStatus()        : 0 ); }             // Particle status
    TParticlePDG* GetPdtEntry()   const; 
    Double_t GetCharge()      const;
    Double_t GetPx()          const { return( fMyBase ? fMyBase->GetPx()            : 0 ); }                 // X momentum
    Double_t GetPy()          const { return( fMyBase ? fMyBase->GetPy()            : 0 ); }                 // y momentum
    Double_t GetPz()          const { return( fMyBase ? fMyBase->GetPz()            : 0 ); }                 // z momentum
    Double_t GetEnergy()      const { return( fMyBase ? fMyBase->GetEnergy()        : 0 ); }             // Track energy
    Double_t GetMass()        const { return( fMyBase ? fMyBase->GetMass()          : 0 ); }               // Particle mass
    TLorentzVector GetMomentum() const { return( fMyBase ? fMyBase->GetMomentum() : TLorentzVector() ); }
    Double_t GetStartVx()     const { return( fMyBase ? fMyBase->GetStartVx()       : 0 ); }            // x position of vertex
    Double_t GetStartVy()     const { return( fMyBase ? fMyBase->GetStartVy()       : 0 ); }            // y position of vertex
    Double_t GetStartVz()     const { return( fMyBase ? fMyBase->GetStartVz()       : 0 ); }            // z position of vertex
    TVector3 GetStartVertex() const { return( fMyBase ? fMyBase->GetStartVertex() : TVector3() ); }
    Double_t GetLifeTime()    const { return( fMyBase ? fMyBase->GetLifeTime()      : 0 ); }           // Production time
    Int_t GetAodMatchIndex()  const { return( fMyBase ? fMyBase->GetAodMatchIndex() : 0 ); }      // Track number of AOD Parter (if none: -1)
    Double_t GetAodMatchConsistency()  const { return( fMyBase ? fMyBase->GetAodMatchConsistency() : 0 ); }      // Track number of AOD Parter (if none: -1)
    
protected:
    PAFMcBase*    fMyBase;         // This is the PAFMcBase for which I am te Interface.
    VAbsTruth*    fMother;         // My mom.
    TObjArray*    fDaughters;      // My kiddies. Of course I am a modern mom and do NOT own my descendants.
    
private:
    TDatabasePDG  *fPDG;	    //!Particle database
public:
    ClassDef(PAFMcTruth,1) //Monte-Carlo truth event
	
};      

// standalone print
std::ostream&  operator << (std::ostream& o, const PAFMcTruth&);

#endif
