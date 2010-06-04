//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFNeutralAodBase							//
//                                                                      //
// Definition of the Analysis Object Data database (neutral part)	//
//                                                                      //
// Author: T. Brandt, TUD, June 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "TTree.h"
#include "TBranch.h"
#include "TBranchClones.h"

#include "PAFSchema/PAFNeutralAodBase.h"

ClassImp(PAFNeutralAodBase)

#include <iostream>
using namespace std;

PAFNeutralAodBase::PAFNeutralAodBase() :
PAFAodBase(),
PAFAodCluster()
{
    _charge = 0;

    for (Int_t i=0; i<4; i++) {
	_PIDconsistency[i]=0;
	_PIDlikelihood[i]=0;
    }
    dummy =  new TBranch();  dummy->ResetBit(kDoNotProcess);
    theLHOODbranch = dummy;
    theSLbranch = dummy;
}


PAFNeutralAodBase::PAFNeutralAodBase(PAFNeutralAodBase &other)
  :PAFAodBase(other),
   PAFAodCluster(other)
{
  // copy Branches
  //dummy = new TBranch();   dummy->ResetBit(kDoNotProcess);
  dummy=NULL;
  if (other.theLHOODbranch!=other.dummy) {
    theLHOODbranch = other.theLHOODbranch;
  } else {
    theLHOODbranch = dummy;
  }
  if (other.theSLbranch!=other.dummy) {
    theSLbranch = other.theSLbranch;
  } else {
    theSLbranch = dummy;
  }

  // copy data
  for (Int_t i=0; i<5; i++) {
    _PIDconsistency[i]=other._PIDconsistency[i];
    _PIDlikelihood[i]=other._PIDlikelihood[i];
  }

}


Float_t
PAFNeutralAodBase::GetSignificanceLevel(Int_t hypo) {
  if (theSLbranch->TestBit(kDoNotProcess)) {
    theSLbranch->ResetBit(kDoNotProcess);
    //theSLbranch->GetTree()->UpdateActiveBranches();
    theSLbranch->GetTree()->GetEntry(theSLbranch->GetTree()->GetReadEntry());
  }
  if ((hypo>=0) && (hypo<=4)) {
    return _PIDconsistency[hypo];
  } else {
    return 0;
  }
}

void
PAFNeutralAodBase::SetSignificanceLevel(Int_t hypo,Float_t val) 
{
    if ((hypo>=0) && (hypo<=4)) {
	_PIDconsistency[hypo]=val;
    } 
}


// likelihood 

void
PAFNeutralAodBase::SetLikelihood(Int_t hypo,Float_t val) 
{
    if ((hypo>=0) && (hypo<=4)) {
	_PIDlikelihood[hypo]=val;
    } 
}

Float_t
PAFNeutralAodBase::GetLikelihood(Int_t hypo) 
{
  if (theLHOODbranch->TestBit(kDoNotProcess)) {
    theLHOODbranch->ResetBit(kDoNotProcess);
    //theLHOODbranch->GetTree()->UpdateActiveBranches();
    theLHOODbranch->GetTree()->GetEntry(theLHOODbranch->GetTree()->GetReadEntry());
  }  
  if ((hypo>=0) && (hypo<=4)) {
    return _PIDlikelihood[hypo];
  } else {
    return 0;
  }
}


void
PAFNeutralAodBase::PrintOn(std::ostream &o) const 
{
    PAFAodBase::PrintOn(o);
    PAFAodCluster::PrintOn(o);
}


void PAFNeutralAodBase::SetTree(TTree *theTree)
{
  //cout << "Set Neutral Tree " << endl;
  TBranchClones *theNeutralBranch = (TBranchClones*) theTree->GetBranch("AodListN");
  if (theNeutralBranch==NULL) {
    cout << "ERROR ! Could not find Branch for charged Aod ! " << endl;
    return;
  }

  theSLbranch = theTree->GetBranch("AodListN._PIDconsistency[5]"); 
  theLHOODbranch = theTree->GetBranch("AodListN._PIDlikelihood[5]"); 
  if (theSLbranch==NULL) { theSLbranch = dummy; }
  if (theLHOODbranch==NULL) { theLHOODbranch = dummy; } 

}

void PAFNeutralAodBase::SetTree(PAFNeutralAodBase *other)
{
  theSLbranch = other->theSLbranch;
  theLHOODbranch = other->theLHOODbranch;
}


std::ostream&  operator << (std::ostream& o, const PAFNeutralAodBase& a) { a.PrintOn(o); return o; }


