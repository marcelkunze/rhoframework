#ifndef BtaCandList_hh
#define BtaCandList_hh
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
#include <iostream>
#include "TNamed.h"
#include "TClonesArray.h"
#include "TObjArray.h"
#include "TVector.h"
#include "TVector3.h"

class BtaCandidate;
class VAbsPidSelector;
class VAbsVertexSelector;

class BtaCandList : public TNamed {
    
public:
    //Constructor
    BtaCandList(const char *name="BtaCandList",UInt_t capacity=512);
    BtaCandList(BtaCandList& b);
    //Destructor
    virtual ~BtaCandList();
    void cleanup();
    
    Int_t getNumberOfTracks() const;
    Int_t getNumberOfCandidates() const {return getNumberOfTracks();}
    Int_t getLength() const {return getNumberOfTracks();}
    void add(const BtaCandidate& c) { put(c); }
    void append(const BtaCandidate& c) { put(c); }
    //void append(BtaCandList& l, VAbsPidSelector* pidmgr);
    void put(const BtaCandidate&, Int_t i = -1 );
    void insertAt(Int_t i, const BtaCandidate& c);
    //void combine(BtaCandList& l1, BtaCandList& l2,VAbsVertexSelector* s=0);
    //void combineAndAppend( BtaCandList& l1, BtaCandList& l2,VAbsVertexSelector* s=0);
    //void select(VAbsPidSelector* pidmgr);
    //void select(BtaCandList& l, VAbsPidSelector* pidmgr);
    void select(BtaCandList& l, Bool_t (*selfunc)(BtaCandidate&) );
    Int_t occurrencesOf(BtaCandidate& );
    Int_t remove(BtaCandidate& ); // Returns #removed cands
    Int_t removeFamily(BtaCandidate& ); // Returns #removed cands
    Int_t removeClones(); // Returns #removed cands
    BtaCandidate* get(Int_t);
    BtaCandidate* getConst(Int_t) const;

    // Analysis functions

    void boost(const TVector3& );
    Double_t getTotalEnergy(Double_t emin=0.0);
    TVector3 getTotalMomentum(Double_t pmin=0.0);
    
    //operations which work on existing lists
    
    void printOn( std::ostream& o=std::cout ) const;
    void sort(int (*compfunc)(const BtaCandidate **, const BtaCandidate **) );
    
    void remainder(BtaCandList&);
    
    void operator =(const BtaCandList& );
    BtaCandidate& operator[](Int_t);
    
    void setFast(Bool_t yesno=kTRUE) { fFast = yesno; }

private:
    TObjArray*   fOwnList;	// This holds the candidates        
    Bool_t	 fFast;		// Fast mode = no error matrices

    ClassDef(BtaCandList,1) //Container for BtaCandidates

};      

// standalone print
std::ostream&  operator << (std::ostream& o, const BtaCandList&);

#endif
