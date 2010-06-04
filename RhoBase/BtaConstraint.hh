#ifndef BtaConstraint_hh
#define BtaConstraint_hh
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// BtaConstraint	    						//
//                                                                      //
// Set kinematics and geometrical constraints for BtaCandidates		//
//									//
// Author List:								//
// Marcel Kunze,  RUB, Aug.99						//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "TObject.h"
#include "TNamed.h"
#include "TVector3.h"

class BtaCandidate;
class THashList;
class TString;

class BtaConstraint : public TObject
{
    
public:
    
    //
    //  possible types of constraints
    //
    
  enum Type
    { 
      Mass, 
      Momentum, 
      PseudoMomentum,
      Energy,
      BeamEnergy,
      Beam,
      Life,
      GeoXY,
      GeoZ,
      LineOfFlight,
      MissingMass,
// the following are 'pseudo-constraints', used in persisting composites
      VtxAlgorithm,
      VtxMode,
      FlyResonances,
      Pseudo,
      PseudoOrigin
    };
    
private:
    
    //--------------------
    // Instance Members --
    //--------------------
    
    // type
    Type fType;
    
    // parameters
    THashList* fParams;
    THashList* fParamsBtaCandidate;
    
public:
    
    //
    // Public interface
    //
    
    // the type must be specified
    BtaConstraint( Type type );
    
    // copy
    BtaConstraint(const BtaConstraint& );
    
    // operators
    Bool_t operator==(const BtaConstraint& o ) 
    { return fType==o.fType; }
    
    // Destructor
    virtual ~BtaConstraint();
    
    // modifiers
    void addNewParm( const TString& parmName, Double_t parm );
    void addNewParm( const TString& parmName, BtaCandidate* parm );
    
    // access
    BtaConstraint::Type type() const ;
    Bool_t getParmValue( const TString& parmName, Double_t& ) const;
    Bool_t getParmValue( const TString& parmName, BtaCandidate* ) const;
    Bool_t getParmValue( int& ) const { return 0; }
    Bool_t getParmValue( const BtaCandidate*&) const { return 0; }
   
    // printings
    void printOn( std::ostream& o=std::cout ) const;

private:
    
    // mutable trick
    BtaConstraint* myself_() const { return (BtaConstraint*) this; }
    
public:
    ClassDef(BtaConstraint,1) //Set fitter constraints
};

//
//  Global functions to make the setting of Constraints easier
//  Note : the list is not exhaustive
//

//class BtaCandidate;
class TVector3;
class TPointErr;
class TVectorErr;
class TDoubleErr;
class TEventInfo;

// Constrain the mass of the candidate to the given mass
void setMassConstraint( BtaCandidate& cand, Double_t mass );

// Constrain the mass of the candidate to its type mass
void setMassConstraint( BtaCandidate& cand );

// Constrain the lifetime of the candidate (c*tau, in centimeters)
void setLifetimeConstraint( BtaCandidate& cand, Double_t ctau );

// Constrain the lifetime of the candidate to its type lifetime
void setLifetimeConstraint( BtaCandidate& cand );

// Constrain the energy of the candidate
void setEnergyConstraint( BtaCandidate& cand, TDoubleErr& energy,const TVector3& boost=TVector3(0,0,0) );

void setEnergyConstraint( BtaCandidate& cand, TVectorErr& eMinusMom, TVectorErr& ePlusMom, Double_t scale=0.5);

void setEnergyConstraint( BtaCandidate& cand, const TEventInfo* ev, Double_t scale=0.5);


// Constraint the momentum of the candidate
void setMomentumConstraint( BtaCandidate& cand );

void setMomentumConstraint( BtaCandidate& cand, 
			   const TVectorErr& momentum, 
			   const TVector3& boost=TVector3(0,0,0) );



// Beam Constrain - 
// Constrain the head of a decay tree to come from a given vertex
void setBeamConstraint( BtaCandidate& c, 
		       const TPointErr& beam );

void setBeamConstraint( BtaCandidate& c, const TEventInfo* ev);


// Geometric Constraint -
// forces the daughters of a candidate to have a common vertex (only defined in Paul Avery approach)
void setGeoXYConstraint( BtaCandidate& c);
void setGeoZConstraint( BtaCandidate& c);
void setGeoConstraint( BtaCandidate& c);

// Line of flight constraint
void setLineOfFlighBtaConstraint( BtaCandidate& c, TVector3 dir, TVector3 point=TVector3(0,0,0));
void setLineOfFlighBtaConstraint( BtaCandidate& c, TVector3 dir, const TEventInfo* ev);
void setLineOfFlighBtaConstraint( BtaCandidate& c, BtaCandidate* cdir);

// Constrain the missing mass
void setMissingMassConstraint( BtaCandidate& cand, BtaCandidate* cMissing, Double_t missingMass );
void setMissingMassConstraint( BtaCandidate& cand, BtaCandidate* cMissing );

class BtaDoubleParm : public TNamed
{
public:
    BtaDoubleParm(const char* name, Double_t val) : TNamed(name,name)
    { fValue = val; }
    Double_t getValue() { return fValue; }
private:
    Double_t fValue;
};

class BtaCandidateParm : public TNamed
{
public:
    BtaCandidateParm(const char* name, BtaCandidate *val) : TNamed(name,name)
    { fValue = val; }
    BtaCandidate* getValue() { return fValue; }
private:
    BtaCandidate* fValue;
};

#endif




