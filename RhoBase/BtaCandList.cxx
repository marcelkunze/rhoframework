//////////////////////////////////////////////////////////////////////////
//                                                                      //
// BtaCandList								//
//                                                                      //
// Container class for BtaCandidates					//
//									//
// Author List:								//
// Marcel Kunze,  RUB, Feb. 99						//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include <stdlib.h>

#include "RhoBase/BtaCandList.hh"
#include "RhoBase/BtaCandidateFactory.hh"
#include "RhoBase/BtaCandidate.hh"
//#include "RhoBase/VAbsPidSelector.h"
//#include "RhoBase/VAbsVertexSelector.h"

ClassImp(BtaCandList)

#include <iostream>
using namespace std;

BtaCandList::BtaCandList(const char *name, UInt_t capacity) : 
TNamed(name,name), fFast(kFALSE)
{
    fOwnList = new TObjArray(capacity);
}


// Perform a deep copy
BtaCandList::BtaCandList(BtaCandList& l) 
{
    fFast = l.fFast;
    
    cleanup();
    const Int_t n = l.getNumberOfTracks();
    for (int i=0;i<n;++i) {
	put(*l.get(i));
    }
}

//--------------
// Destructor --
//--------------
// Deletes the list and the owned objects 
BtaCandList::~BtaCandList( ) 
{
    delete fOwnList;
}

//----------------------
//-- public Functions --
//----------------------

void BtaCandList::cleanup( ) 
{
    fOwnList->Clear();
}

Int_t BtaCandList::getNumberOfTracks() const 
{
    return fOwnList->GetLast()+1;
}

void BtaCandList::put(const BtaCandidate& c, Int_t i )  
{
    BtaCandidate *newCand = BtaCandidateFactory::instance()->newCandidate(c);
    if (i<0)
	fOwnList->Add(newCand);
    else
	(*fOwnList)[i] = newCand;
}        

void BtaCandList::insertAt(Int_t i, const BtaCandidate& c)
{
    fOwnList->AddAtAndExpand((TObject*)&c,i);
    put(c,i);
}        


BtaCandidate* BtaCandList::get(Int_t i)  
{
    if (i >= getNumberOfTracks()) return 0;
    return (BtaCandidate*) (fOwnList->UncheckedAt(i));
}

BtaCandidate* BtaCandList::getConst(Int_t i) const
{
    if (i >= getNumberOfTracks()) return 0;
    return (BtaCandidate*) (fOwnList->UncheckedAt(i));
}

// Compare the marker and remove corresponding entry (MK,12/99)
// This allows to remove objects in several lists
Int_t BtaCandList::remove(BtaCandidate& c )  {
    Int_t nRemoved = 0;
    Int_t n = getNumberOfTracks();
    for (Int_t i=0;i<n;++i) {
	BtaCandidate *b = get(i);
	if (b->equals(c)) {
	    fOwnList->RemoveAt(i);
	    nRemoved++; 
	}
    }
    fOwnList->Compress();
    return nRemoved;
}        

Int_t BtaCandList::removeFamily(BtaCandidate& c )  
{
    Int_t nRemoved = 0;
    Int_t n = getNumberOfTracks();
    for (Int_t i=0;i<n;++i) {
	BtaCandidate *b = get(i);
	if (b->overlaps(c)) {
	    fOwnList->RemoveAt(i);
	    nRemoved++; 
	}
    }
    fOwnList->Compress();
    return nRemoved;
}        

Int_t BtaCandList::removeClones()  
{
    Int_t nRemoved = 0;
    Int_t n = getNumberOfTracks();
    for (Int_t i=0;i<n-1;++i) {
	BtaCandidate *b = get(i);
	if (b==0) continue;
	for (Int_t j=i+1;j<n;++j) {
	    BtaCandidate *c = get(j);
	    if (c==0) continue;
	    if (b->equals(*c)) {
		fOwnList->RemoveAt(j);
		nRemoved++; 
	    }
	}
    }
    fOwnList->Compress();
    return nRemoved;
}        

Int_t BtaCandList::occurrencesOf(BtaCandidate& c )  {
    Int_t nCand = 0;
    const Int_t n = getNumberOfTracks();
    for (Int_t i=0;i<n;++i) {
	BtaCandidate *b = get(i);
	if (b->equals(c)) nCand++; 
    }
    return nCand;
}        

Double_t BtaCandList::getTotalEnergy(Double_t emin)  
{
    Double_t e = 0.0;
    const Int_t n = getNumberOfTracks();
    for (Int_t i=0;i<n;++i) {
	BtaCandidate *c = get(i);
	Double_t energy = c->energy();
	if (energy > emin) e += energy;
    }
    return e;
}        

TVector3 BtaCandList::getTotalMomentum(Double_t pmin)  
{
    TVector3 p(0.0,0.0,0.0);
    const Int_t n = getNumberOfTracks();
    for (Int_t i=0;i<n;++i) {
	BtaCandidate *c = get(i);
	TVector3 p3 = c->p3();
	if (p3.Mag() > pmin) p = p + p3;
    }
    return p;
}        

void BtaCandList::boost(const TVector3& p )  
{
    const Int_t n = getNumberOfTracks();
    for (Int_t i=0;i<n;++i) {
        get(i)->boost(p);
    }
}        

void BtaCandList::printOn(std::ostream& o) const 
{
    const Int_t n = getNumberOfTracks();
    o << "content: " << n << " entries." << endl;
    if ( n!=0 ) {
	for (Int_t i=0;i<n;i++) {
	    const BtaCandidate *c = getConst(i);
	    o << "[" << i << "] " << *c;
	    o << endl;
	}
    }
}        

void BtaCandList::remainder(BtaCandList& l) 
{
    const Int_t n = l.getNumberOfTracks();
    for (int i=0;i<n;++i) {
	const BtaCandidate *c = l.get(i);
	if ( c->getMarker()!=0 ) fOwnList->RemoveAt(i);
    }
    fOwnList->Compress();
}

// Makes now a deep copy rather than just cpoying pointers (MK,12/99)
void BtaCandList::operator =(const BtaCandList& l ) 
{
    fFast = l.fFast;
    
    cleanup();
    const Int_t n = l.getNumberOfTracks();
    for (int i=0;i<n;++i) {
	put(*l.getConst(i));
    }
}                      

BtaCandidate& BtaCandList::operator[](Int_t i) 
{
    return *get(i);
}
/*
void BtaCandList::combine( BtaCandList& l1, BtaCandList& l2,VAbsVertexSelector* s)
{    
    cleanup();
    combineAndAppend(l1, l2, s);  
}

void BtaCandList::combineAndAppend( BtaCandList& l1, BtaCandList& l2,VAbsVertexSelector* selector) 
{
    TLorentzVector vl;
    Double_t charge;
    Bool_t nearby = kTRUE;

    const Int_t endpos1 = l1.getNumberOfTracks();
    const Int_t endpos2 = l2.getNumberOfTracks();
    
    if ( &l1==&l2 ) {
        //combination of a list with itself
        for (Int_t comb_i=0;comb_i<endpos1;++comb_i) {
            for (Int_t comb_k=comb_i+1;comb_k<endpos2;++comb_k) {
                if ( !l1[comb_i].overlaps( l2[comb_k] ) ) {
                    vl=l1[comb_i].p4()+l2[comb_k].p4();
                    charge=l1[comb_i].charge()+l2[comb_k].charge();
		    if (selector) nearby = selector->accept(l1[comb_i],l2[comb_k]);
                    if (nearby) {
                        //fill list with new candidate
                        BtaCandidate c(vl,charge);
			c.SetMarker(l1[comb_i].GetMarker(0)|l2[comb_k].getMarker(0),0);
			c.SetMarker(l1[comb_i].GetMarker(1)|l2[comb_k].getMarker(1),1);
			c.SetMarker(l1[comb_i].GetMarker(2)|l2[comb_k].getMarker(2),2);
			c.SetMarker(l1[comb_i].GetMarker(3)|l2[comb_k].getMarker(3),3);
                        if (selector!=0){
                          c.setPosition(selector->getVertex());
                          c.setVect(selector->getMomentum());
			  c.setEnergy(c.e());
                        }
			put(c);
		    }
                }
            }
        }     
    }
    else {
        //combination of 2 different lists
        for (Int_t comb_i=0;comb_i<endpos1;++comb_i) {
            for (Int_t comb_k=0;comb_k<endpos2;++comb_k) {
                if ( !l1[comb_i].overlaps( l2[comb_k] ) ) {
		    vl=l1[comb_i].p4()+l2[comb_k].p4();
		    charge=l1[comb_i].charge()+l2[comb_k].charge();
		    if (selector) nearby = selector->accept(l1[comb_i],l2[comb_k]);
                    if (nearby) {
                        //fill list with new candidate
                        BtaCandidate c(vl,charge);
			c.setMarker(l1[comb_i].getMarker(0)|l2[comb_k].getMarker(0),0);
			c.setMarker(l1[comb_i].getMarker(1)|l2[comb_k].getMarker(1),1);
			c.setMarker(l1[comb_i].getMarker(2)|l2[comb_k].getMarker(2),2);
			c.setMarker(l1[comb_i].getMarker(3)|l2[comb_k].getMarker(3),3);
                        if (selector!=0){
                          c.setPosition(selector->getVertex());
                          c.setVect(selector->getMomentum());
			  c.setEnergy(c.e());
                        }
			put(c);
		    }
                }
            }
        }
    } 
}        
*/
std::ostream&  operator << (std::ostream& o, const BtaCandList& a) { a.printOn(o); return o; }

//extern "C" void qsort(void *, size_t, size_t, int (const void *,const void *));

typedef int compare_function(const void*, const void*);

void BtaCandList::sort(int (*compfunc)(const BtaCandidate **, const BtaCandidate **) )
{
   qsort( fOwnList, getNumberOfTracks(), sizeof(void *), (compare_function *)compfunc);
}


void BtaCandList::select(BtaCandList& l, Bool_t (*selfunc)(BtaCandidate&) )
{
    cleanup();
    const Int_t n = l.getNumberOfTracks();
    for (Int_t i=0;i<n;++i) {
	BtaCandidate *c = get(i);
	if (selfunc(*c)) {
	    put(*c);
       	}
    }
}

// Destructive selection

/*

void BtaCandList::select(VAbsPidSelector* pidmgr) 
{
    const Int_t n = getNumberOfTracks();
    for (Int_t i=0;i<n;++i) {
	BtaCandidate *c = get(i);
	if (!pidmgr->accept(*c)) fOwnList->RemoveAt(i);
    }
    fOwnList->Compress();
}

// Non-destructive selection

void BtaCandList::select(BtaCandList& l, VAbsPidSelector* pidmgr) 
{
    Cleanup();
    const Int_t n = l.getNumberOfTracks();
    for (Int_t i=0;i<n;++i) {
	BtaCandidate *c = l.get(i);
	if (pidmgr->accept(*c)) {
	    put(*c);
       	}
    }
}

void BtaCandList::append(BtaCandList& l, VAbsPidSelector* pidmgr) 
{
    const Int_t n = l.getNumberOfTracks();
    for (Int_t i=0;i<n;++i) {
	BtaCandidate *c = l.get(i);
	if (pidmgr->accept(*c)) {
	    put(*c);
       	}
    }
}

*/