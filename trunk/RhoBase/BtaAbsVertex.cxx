//////////////////////////////////////////////////////////////////////////
//                                                                      //
// BtaAbsVertex								//
//                                                                      //
// Abstract Definition of the vertex					//
//                                                                      //
// Author: Marcel Kunze, RUB, Mar. 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "RhoBase/BtaAbsVertex.hh"
#include "RhoBase/BtaCandidate.hh"

ClassImp(BtaAbsVertex)

#include <iostream>
using namespace std;

BtaAbsVertex::BtaAbsVertex( )
: fStatus(UnFitted), fInComingCand(0)
{
    nOutGoingCands = 0;
    nResonances = 0;
}

BtaAbsVertex::BtaAbsVertex( const BtaAbsVertex& other )
: fStatus(other.fStatus), fInComingCand(0)
{
    nOutGoingCands = 0;
    nResonances = 0;
}

//--------------
// Destructor --
//--------------

BtaAbsVertex::~BtaAbsVertex( )
{
}

//--------------
// Operations --
//--------------

const BtaCandidate*
BtaAbsVertex::theMother() const
{
    // return the incoming candidate, if any
    if( fInComingCand!=0 ) return fInComingCand;
    
    // else, loop over the resonances
    BtaCandidate* mother=0;
    for( Int_t i=0; i<nResonances; i++ )
    {
	BtaCandidate* cand =  fResonances[i];
	if( cand->theMother()==0 )
	{
	    if( mother==0 )
	    {
		mother=cand;
	    }
	    else
	    {
		cout << "several candidates with no mother" 
		    << endl;
	    }
	}
    }
    if( mother==0 ) cout << "no candidate with a mother" 
	<< endl;
    return mother;
}

// 4-momentum of the incoming particle
TLorentzVector 
BtaAbsVertex::p4() const
{
    const BtaCandidate* mother = theMother();
    if (mother==0) return TLorentzVector(0.,0.,0.,0.);
    return mother->p4();
}

// mass of the incoming particle
double 
BtaAbsVertex::mass() const
{
    const BtaCandidate* mother = theMother();
    if (mother==0) return 0.;
    return mother->mass();
}

// momentum of the incoming particle
double 
BtaAbsVertex::p() const
{
    const BtaCandidate* mother = theMother();
    if (mother==0) return 0.;
    return mother->p();
}

void 
BtaAbsVertex::printOn(std::ostream& o) const 
{
    o << point().X() << '\t' << point().Y() << '\t' << point().Z() << endl;
}

std::ostream&  
operator << (std::ostream& o, const BtaAbsVertex& a) 
{
    a.printOn(o); return o;
}

TMatrixD& 
BtaAbsVertex::XXWeight() const 
{
    static TMatrixD x(3,3);
    return x;
}

TMatrixD&  
BtaAbsVertex::PPWeight() const 
{
    static TMatrixD x(3,3);
    return x;
}

TMatrixD&   
BtaAbsVertex::XPWeight() const 
{
    static TMatrixD x(3,3);
    return x;
}

void 
BtaAbsVertex::getAllWeights(TVector3& xv,
			  TVector3& pv,
			  TMatrixD&  xxWeight,
			  TMatrixD&  ppWeight,
			  TMatrixD&     xpWeight) const 
{
    int i,j;
    for(i=0;i<3;i++)
	for(j=i;j<3;j++) {
	    xxWeight(i,j)=0;
	    ppWeight(i,j)=0;
	    xpWeight(i,j)=0;
	    xpWeight(j,i)=0;
	}
	
	for(i=0;i<3;i++)
	{
	    xv[i]=0;
	    pv[i]=0;
	}
	
}

TMatrixD& 
BtaAbsVertex::XXCov() const 
{
    static TMatrixD x(3,3);
    return x;
}

TMatrixD& 
BtaAbsVertex::PPCov() const 
{
    static TMatrixD in(3,3), out(3,3);
    if( fInComingCand ) 
	in = fInComingCand->getFitParams().p4Err();
    out = in - out;
    return out;
}

TMatrixD&    
BtaAbsVertex::XPCov() const 
{
    static TMatrixD x(4,3);
    return x;
}

/*
*
*/
BtaAbsVertex::VtxStatus
BtaAbsVertex::status() const
{
    return fStatus;
}

BtaAbsVertex::VtxType
BtaAbsVertex::type() const
{
    return fType;
}


TVector3 
BtaAbsVertex::point() const 
{ 
    return TVector3(v4().X(), v4().Y(), v4().Z()); 
}

/*
* Former inline funcs
*/
void
BtaAbsVertex::setStatus(BtaAbsVertex::VtxStatus s) 
{
    fStatus=s;
}

void
BtaAbsVertex::setType(BtaAbsVertex::VtxType type) 
{
    fType=type;
}

// number of out going candidates
int 
BtaAbsVertex::nOutGoing() const
{
    return nOutGoingCands;
}

// number of resonances (states produced and decayed at that vertex)
int 
BtaAbsVertex::nResonance() const
{
    return nResonances;
}

const BtaCandidate* 
BtaAbsVertex::inComingCand() const
{
    return fInComingCand;
}
