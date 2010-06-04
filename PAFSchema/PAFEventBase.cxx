//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFEventBase								//
//                                                                      //
// Event header class in PAF(Pico Analysis Framework)			//
//                                                                      //
// Author: Sascha Berger, Bochum University, Feb. 99			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "TTree.h"
#include "TBranch.h"
#include "TBranchObject.h"

#include "PAFSchema/PAFEventBase.h"

ClassImp(PAFEventBase)

#include <iostream>
using namespace std;

PAFEventBase::PAFEventBase() : 
_runno(0), 
_eventno(0), 
_majorID(0),
_minorID(0), 
_nChargedTracks(0), 
_nNeutralTracks(0), 
_tag()
{
    fBranches[0] = new TBranch();  fBranches[0]->ResetBit(kDoNotProcess);
    for (Int_t i=1; i<NELEM_EVENTBASE; i++) {
	fBranches[i]=fBranches[0];
    }
}

PAFEventBase::~PAFEventBase()
{
    Reset();
    delete fBranches[0];
}

void PAFEventBase::AddTag( PAFTagBase* c) {
    _tag=*c;
}

void PAFEventBase::AddNeutralTrack() { 
    _nNeutralTracks++;
}

void PAFEventBase::AddChargedTrack() { 
    _nChargedTracks++;
}

void PAFEventBase::Reset() {
}

void PAFEventBase::NewEvent()  {
    _nChargedTracks = 0;
    _nNeutralTracks = 0;
    ++_eventno;   
}

void PAFEventBase::NewChargedTrack() {
    ++_nChargedTracks;
}

void PAFEventBase::NewNeutralTrack() {
    ++_nNeutralTracks;
}

void PAFEventBase::SetRunNumber(UInt_t r) {
    _runno=r;
}

void PAFEventBase::SetEventNumber(Int_t e) {
    _eventno=e;
}

void PAFEventBase::SetNumberOfChargedTracks(UInt_t n) {
    _nChargedTracks = n; 
}

void PAFEventBase::SetNumberOfNeutralTracks(UInt_t n) {
    _nNeutralTracks = n; 
}

UInt_t   PAFEventBase::GetRunNumber() const {
    return GetUInt(fBranches[1],_runno);
}

Int_t  PAFEventBase::GetEventNumber() const { 
    return GetInt(fBranches[2],_eventno); 
}

Bool_t  PAFEventBase::Eof() { 
    SetEventNumber(-1); // Reset to check EoF
    return (GetEventNumber() == -1); 
}

UInt_t	PAFEventBase::GetMajorID() const { return GetUInt(fBranches[3],_majorID); }
UInt_t	PAFEventBase::GetMinorID() const { return GetUInt(fBranches[4],_minorID); }

UInt_t   PAFEventBase::GetNumberOfTracks() const {
    return GetUShort(fBranches[5],_nChargedTracks) + GetUShort(fBranches[6],_nNeutralTracks);
}

UInt_t   PAFEventBase::GetNumberOfChargedTracks() const {
    return GetUShort(fBranches[5],_nChargedTracks);
}

UInt_t   PAFEventBase::GetNumberOfNeutralTracks() const {
    return  GetUShort(fBranches[6],_nNeutralTracks);
}

TVector3 PAFEventBase::GetBoost() const {
    TVector3 upsiboost(0.,0.,0.486976); // Upsilon boost
    TLorentzVector v = _tag.GetCMFrame();
    if (v.T() <= 0) return upsiboost; // Upsilon boost
    return TVector3(v.X()/v.T(), v.Y()/v.T(), v.Z()/v.T());
}

PAFTagBase&  PAFEventBase::GetTag() {
    return _tag;
}

void PAFEventBase::PrintOn(std::ostream& o) const {
    o << endl;
    o << "Run number     : " << GetRunNumber() << endl;
    o << "Event number   : " << GetEventNumber() << endl;
    o << "Time (GMT)     : " << GetTime() << endl;
    o << "Candidates     : " << GetNumberOfTracks();
    o << ", Charged: " << GetNumberOfChargedTracks();
    o << ", Neutral: " << GetNumberOfNeutralTracks() << endl << endl;
    o << _tag<<endl;
}


TRhoTime  PAFEventBase::GetTime() const { return TRhoTime(GetUInt(fBranches[3],_majorID)); }


void
PAFEventBase::SetTree(TTree *theTree)
{
    TBranch *subBranch;
    TObjArray *list1 = theTree->GetListOfBranches();
    TBranchObject *objBranch = (TBranchObject*) (*list1)[0];
    _tag.Init(objBranch);
    TObjArray *list = objBranch->GetListOfBranches();
    Int_t listLen = list->GetEntries();
    for (Int_t i=0; i<listLen; i++) {
	subBranch = (TBranch*) (*list)[i];
	TString t(subBranch->GetName());
	if (t == "_runno") { fBranches[1] = subBranch; }
	if (t == "_eventno") { fBranches[2] = subBranch; }
	if (t == "_majorID") { fBranches[3] = subBranch; }
	if (t == "_minorID") { fBranches[4] = subBranch; }
	if (t == "_nChargedTracks") { fBranches[5] = subBranch; }
	if (t == "_nNeutralTracks") { fBranches[6] = subBranch; }
    }
    //test dump
    //for (Int_t i=0; i<NELEM_EVENTBASE; i++) {
    //  cout << "EventBase - Branch #" << i << " has Address " << fBranches[i] << endl;
    //}
}


void PAFEventBase::SetOtherEventNumber(UInt_t i)
{
    fEventNumber=i;
    _tag.SetEventNumber(i);
}

UInt_t PAFEventBase::GetUInt(TBranch *theBranch, const UInt_t &value) const
{
    theBranch->GetEntry(fEventNumber);
    return value;
}

Int_t PAFEventBase::GetInt(TBranch *theBranch, const Int_t &value) const
{
    theBranch->GetEntry(fEventNumber);
    return value;
}

UShort_t PAFEventBase::GetUShort(TBranch *theBranch, const UShort_t &value) const
{
    theBranch->GetEntry(fEventNumber);
    return value;
}




// --------------------
// non-member functions
// --------------------

std::ostream&  operator << (std::ostream& o, const PAFEventBase& a) {a.PrintOn(o); return o;}

