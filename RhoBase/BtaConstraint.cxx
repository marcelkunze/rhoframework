//////////////////////////////////////////////////////////////////////////
//                                                                      //
// BtaConstraint							//
//                                                                      //
// Set kinematics and geometrical constraints for BtaCandidates		//
//									//
// Author List:								//
// Marcel Kunze,  RUB, Aug.99						//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "THashList.h"
#include "TString.h"

#include "RhoBase/TRho.h"
#include "RhoBase/BtaEventInfo.hh"
#include "RhoBase/BtaConstraint.hh"
#include "RhoBase/BtaFitParams.hh"
#include "RhoBase/BtaCandidate.hh"
#include "RhoBase/BtaCandList.hh"
#include "RhoBase/BtaCandListIterator.hh"
#include "RhoBase/BtaAbsVertex.hh"
#include "RhoMath/TDoubleErr.h"
#include "RhoMath/TPointErr.h"
#include "PDT/PdtEntry.hh"

#include <assert.h>
#include <stddef.h>

ClassImp(BtaConstraint)

#include <iostream>
using namespace std;

BtaConstraint::BtaConstraint( BtaConstraint::Type type )
: fType(type)
, fParams( 0 )
, fParamsBtaCandidate( 0 )
{
}

BtaConstraint::BtaConstraint(const BtaConstraint& b )
: fType(b.fType)
, fParams( 0 )
, fParamsBtaCandidate( 0 )
{  
    if ( b.fParams != 0 ) {
	fParams = new THashList(b.fParams);
    }
    if ( b.fParamsBtaCandidate != 0 ) {
	fParamsBtaCandidate = 
	    new THashList(b.fParamsBtaCandidate);
    }
}

//--------------
// Destructor --
//--------------

BtaConstraint::~BtaConstraint( )
{
    if (fParams!=0) { fParams->Delete(); delete fParams; }
    if (fParamsBtaCandidate!=0) { fParamsBtaCandidate->Delete();delete fParamsBtaCandidate; }
}

//--------------------
// Member Functions --
//--------------------


BtaConstraint::Type 
BtaConstraint::type() const 
{ 
    return fType; 
}

void
BtaConstraint::addNewParm( const TString& parmName, Double_t parm )
{
    if ( fParams == 0 ) {
	fParams = new THashList();
    }
    fParams->Add( new BtaDoubleParm(parmName, parm) );
}

void
BtaConstraint::addNewParm( const TString& parmName, BtaCandidate* parm )
{
    if ( fParamsBtaCandidate == 0 ) {
	fParamsBtaCandidate = new THashList();
    }
    fParamsBtaCandidate->Add( new BtaCandidateParm(parmName, parm) );
}

Bool_t
BtaConstraint::getParmValue( const TString& parmName, Double_t& value ) const
{
    if ( fParams == 0 ) return kFALSE;
    BtaDoubleParm *parm = (BtaDoubleParm *)  fParams->FindObject(parmName);
    if (parm!=0) value = parm->getValue();
    return (parm!=0);
}

Bool_t
BtaConstraint::getParmValue( const TString& parmName, BtaCandidate* value ) const
{
    if ( fParamsBtaCandidate == 0 ) return kFALSE;
    BtaCandidateParm *parm = (BtaCandidateParm *)  fParamsBtaCandidate->FindObject(parmName);
    if (parm!=0) value = parm->getValue();
    return (parm!=0);
}


void 
BtaConstraint::printOn(std::ostream& o) const
{
    o << "Constraint Type " << type() << endl;
    if ( fParams == 0 ) return;
    TIter iter(fParams);
    for(;iter.Next();)
    {
	BtaDoubleParm &p = (BtaDoubleParm &) iter;
	o << "/t" << p.GetName() << "/t" << p.getValue() << endl;
    }
}

//
// Global functions
//

// Constrain the mass of the candidate to the given mass
void 
setMassConstraint( BtaCandidate& cand, Double_t mass )
{
    BtaConstraint& c = cand.addConstraint( BtaConstraint::Mass );
    c.addNewParm( "mass", mass );
}

// Constrain the mass of the candidate to its type mass
void 
setMassConstraint( BtaCandidate& cand )
{
    const PdtEntry* pdt = cand.pdtEntry();
    Double_t mass = pdt>0 ? pdt->mass() : 0.;
    setMassConstraint( cand, mass );
}

// Constrain the lifetime of the candidate (c*tau, in centimeters)
void 
setLifetimeConstraint( BtaCandidate& cand, Double_t ctau )
{
    BtaConstraint& c = cand.addConstraint( BtaConstraint::Life );
    c.addNewParm( "ctau", ctau );  
}

// Constrain the lifetime of the candidate to its type lifetime
void 
setLifetimeConstraint( BtaCandidate& cand )
{
    const PdtEntry* pdt = cand.pdtEntry();
    Double_t ctau = pdt>0 ? pdt->lifetime() : 0.;
    setLifetimeConstraint( cand, ctau );  
}

// Constrain the energy of the candidate
void 
setEnergyConstraint( BtaCandidate& cand, TDoubleErr& energy, const TVector3& boost )
{
    BtaConstraint& c = cand.addConstraint( BtaConstraint::Energy );
    c.addNewParm( "energy", energy.Value() );
    c.addNewParm( "energyCov", energy.Covariance() );
    c.addNewParm( "boostX", boost.X() );    
    c.addNewParm( "boostY", boost.Y() );    
    c.addNewParm( "boostZ", boost.Z() );    
}


void 
setEnergyConstraint( BtaCandidate& cand, TVectorErr& eMinusMom, TVectorErr& ePlusMom, Double_t scale)
{
    BtaConstraint& c = cand.addConstraint( BtaConstraint::BeamEnergy );
    c.addNewParm( "eMinusPx", eMinusMom.X() );    
    c.addNewParm( "eMinusPy", eMinusMom.Y() );    
    c.addNewParm( "eMinusPz", eMinusMom.Z() );
    c.addNewParm( "eMinusCovPxPx", eMinusMom.CovMatrix()(0,0) );
    c.addNewParm( "eMinusCovPxPy", eMinusMom.CovMatrix()(0,1) );
    c.addNewParm( "eMinusCovPxPz", eMinusMom.CovMatrix()(0,2) );
    c.addNewParm( "eMinusCovPyPy", eMinusMom.CovMatrix()(1,1) );
    c.addNewParm( "eMinusCovPyPz", eMinusMom.CovMatrix()(1,2) );
    c.addNewParm( "eMinusCovPzPz", eMinusMom.CovMatrix()(2,2) );
    //    
    c.addNewParm( "ePlusPx", ePlusMom.X() );    
    c.addNewParm( "ePlusPy", ePlusMom.Y() );    
    c.addNewParm( "ePlusPz", ePlusMom.Z() );
    c.addNewParm( "ePlusCovPxPx", ePlusMom.CovMatrix()(0,0) );
    c.addNewParm( "ePlusCovPxPy", ePlusMom.CovMatrix()(0,1) );
    c.addNewParm( "ePlusCovPxPz", ePlusMom.CovMatrix()(0,2) );
    c.addNewParm( "ePlusCovPyPy", ePlusMom.CovMatrix()(1,1) );
    c.addNewParm( "ePlusCovPyPz", ePlusMom.CovMatrix()(1,2) );
    c.addNewParm( "ePlusCovPzPz", ePlusMom.CovMatrix()(2,2) );
    //
    c.addNewParm( "scale", scale );    // scale factor to apply to the total beam energy
}


void 
setEnergyConstraint( BtaCandidate& cand,const BtaEventInfo* ev, Double_t scale)
{
    // retreive BtaEventInfo pointer
    if( ev==0 )
    {
      cerr << "setEnergyConstraint BtaEventInfo* not available " << endl;
      assert(0);
    }
    // get the beam energies
    const BtaFitParams em = ev->electronBeam().getFitParams();
    TVectorErr eMinusMom(em.p3WErr());
    const BtaFitParams ep = ev->positronBeam().getFitParams();
    TVectorErr ePlusMom(ep.p3WErr());
    // impose constraint
    setEnergyConstraint( cand, eMinusMom, ePlusMom, scale);
}



// Constraint the momentum of the candidate
void 
setMomentumConstraint( BtaCandidate& cand )
{
    BtaConstraint& c = cand.addConstraint( BtaConstraint::PseudoMomentum );
}


void 
setMomentumConstraint( BtaCandidate& cand, const TVectorErr& momentum, const TVector3& boost)
{
    BtaConstraint& c = cand.addConstraint( BtaConstraint::Momentum );
    c.addNewParm( "px", momentum.X() );      
    c.addNewParm( "py", momentum.Y() );      
    c.addNewParm( "pz", momentum.Z() );      
    c.addNewParm( "spxx", momentum.CovMatrix()(0,0) );      
    c.addNewParm( "spyy", momentum.CovMatrix()(1,1) );      
    c.addNewParm( "spzz", momentum.CovMatrix()(2,2) );      
    c.addNewParm( "spxy", momentum.CovMatrix()(0,1) );      
    c.addNewParm( "spxz", momentum.CovMatrix()(0,2) );      
    c.addNewParm( "spyz", momentum.CovMatrix()(1,2) );
    c.addNewParm( "boostX", boost.X() );    
    c.addNewParm( "boostY", boost.Y() );    
    c.addNewParm( "boostZ", boost.Z() );
}

// Beam Constrain - 
// Constrain the head of a decay tree to come from a given vertex
void 
setBeamConstraint( BtaCandidate& cand, 
		  const TPointErr& beam )
{
    BtaConstraint& c = cand.addConstraint( BtaConstraint::Beam );
    c.addNewParm( "bx", beam.X() );      
    c.addNewParm( "by", beam.Y() );      
    c.addNewParm( "bz", beam.Z() );      
    c.addNewParm( "sxx", beam.CovMatrix()(0,0) );      
    c.addNewParm( "syy", beam.CovMatrix()(1,1) );      
    c.addNewParm( "szz", beam.CovMatrix()(2,2) );      
    c.addNewParm( "sxy", beam.CovMatrix()(0,1) );      
    c.addNewParm( "sxz", beam.CovMatrix()(0,2) );      
    c.addNewParm( "syz", beam.CovMatrix()(1,2) );      
}


void 
setBeamConstraint( BtaCandidate& cand,const BtaEventInfo* ev )
{
    // retreive BtaEventInfo pointer
    if( ev==0 )
    {
      cerr << "SetBeamConstraint BtaEventInfo* not available " << endl;
      assert(0);
    }
    // get the beam position
    TPointErr beamSpot(ev->beamSpot());
    // impose constraint
    setBeamConstraint( cand, beamSpot);
}


void 
setGeoConstraint( BtaCandidate& cand)
{
    setGeoXYConstraint(cand);
    setGeoZConstraint(cand);
    setMomentumConstraint(cand);
}

void 
setGeoXYConstraint( BtaCandidate& cand)
{
    BtaConstraint& c = cand.addConstraint( BtaConstraint::GeoXY );
}

void 
setGeoZConstraint( BtaCandidate& cand)
{
    BtaConstraint& c = cand.addConstraint( BtaConstraint::GeoZ );
}


void 
setLineOfFlighBtaConstraint( BtaCandidate& cand, TVector3 dir, TVector3 point)
{
    BtaConstraint& c = cand.addConstraint( BtaConstraint::LineOfFlight );
    c.addNewParm( "dirX", dir.Unit().X() );
    c.addNewParm( "dirY", dir.Unit().Y() );
    c.addNewParm( "dirZ", dir.Unit().Z() );
    c.addNewParm( "pointX", point.X() );
    c.addNewParm( "pointY", point.Y() );
    c.addNewParm( "pointZ", point.Z() );
}


void 
setLineOfFlighBtaConstraint( BtaCandidate& cand, TVector3 dir, const BtaEventInfo* ev)
{
    // retreive BtaEventInfo pointer
    if( ev==0 )
    {
       cerr << "BtaConstraint::SetLineOfFlighBtaConstraint: BtaEventInfo* not available " << endl;
    }
    setLineOfFlighBtaConstraint( cand, dir, ev->primaryVertex() );
}


void
setLineOfFlighBtaConstraint( BtaCandidate& cand, BtaCandidate* cdir)
{
    TVector3 point;
    if ( cdir->isComposite() && (cdir->decayVtx()!=0) && (cdir->decayVtx()->status()==BtaAbsVertex::Success) )
	setLineOfFlighBtaConstraint( cand, cdir->p3(), cdir->decayVtx()->point() );
    else
	setLineOfFlighBtaConstraint( cand, cdir->p3() );
}



void 
setMissingMassConstraint( BtaCandidate& cand, BtaCandidate* cMissing, Double_t missingMass )
{
    BtaConstraint& c = cand.addConstraint( BtaConstraint::MissingMass );
    BtaCandListIterator iterDau( cand.daughterIterator() );
    int iDau(0);
    BtaCandidate* b=0;
    const PdtEntry* pdt=0;
    while ( b=iterDau.next() ) {
	iDau++;
	if (b->uid()==cMissing->uid()) {
	    pdt = b->pdtEntry();
	    break;
	}
    }
    assert(iDau<=cand.nDaughters());
    assert(pdt==cMissing->pdtEntry());
    c.addNewParm( "iDaughter", iDau );
    c.addNewParm( "missingMass", missingMass );
}



void 
setMissingMassConstraint( BtaCandidate& cand, BtaCandidate* cMissing )
{
    const PdtEntry* pdt = cMissing->pdtEntry();
    Double_t missingMass = pdt>0 ? pdt->mass() : 0.;
    setMissingMassConstraint( cand, cMissing, missingMass);
}

