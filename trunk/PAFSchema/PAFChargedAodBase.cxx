//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFChargedAodBase							//
//                                                                      //
// Definition of the Analysis Object Data database (charged part)	//
//                                                                      //
// Author: T. Brandt, TUD, June 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "TTree.h"
#include "TBranch.h"
#include "TBranchClones.h"

#include "RhoBase/TSmartPointer.h"
#include "PAFSchema/PAFChargedAodBase.h"

ClassImp(PAFChargedAodBase)

#include <iostream>
using namespace std;

PAFChargedAodBase::PAFChargedAodBase() :
PAFAodBase(),
PAFAodCluster(),
PAFAodTrack()
{
    _charge = -1;
    
    for (int i=0; i<5;i++) {
	_PIDconsistency[i] = 0;
	_PIDlikelihood[i] = 0;
    }
}


PAFChargedAodBase::PAFChargedAodBase(PAFChargedAodBase &other)
:PAFAodBase(other),
PAFAodTrack(other),
PAFAodCluster(other)
{    
    // copy data
    _charge = other._charge;
    for (Int_t i=0; i<5; i++) {
	_PIDconsistency[i]=other._PIDconsistency[i];
	_PIDlikelihood[i]=other._PIDlikelihood[i];
    }
    
}

PAFChargedAodBase::~PAFChargedAodBase() 
{ 
}

// significance level

Float_t
PAFChargedAodBase::GetSignificanceLevel(Int_t hypo) {
    if ((hypo>=0) && (hypo<=4)) {
	return _PIDconsistency[hypo];
    } else {
	return 0;
    }
}

void
PAFChargedAodBase::SetSignificanceLevel(Int_t hypo,Float_t val) 
{
    if ((hypo>=0) && (hypo<=4)) {
	_PIDconsistency[hypo]=val;
    } 
}


// likelihood 

void
PAFChargedAodBase::SetLikelihood(Int_t hypo,Float_t val) 
{
    if ((hypo>=0) && (hypo<=4)) {
	_PIDlikelihood[hypo]=val;
    } 
}

Float_t
PAFChargedAodBase::GetLikelihood(Int_t hypo) {
    if ((hypo>=0) && (hypo<=4)) {
	return _PIDlikelihood[hypo];
    } else {
	return 0;
    }
}

void
PAFChargedAodBase::PrintOn(std::ostream &o) const 
{
    PAFAodBase::PrintOn(o);
    PAFAodTrack::PrintOn(o);
    PAFAodCluster::PrintOn(o);
}

std::ostream&  operator << (std::ostream& o, const PAFChargedAodBase& a) { a.PrintOn(o); return o; }
