#ifndef TASSOCIATOR_H
#define TASSOCIATOR_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TAssociator								//
//                                                                      //
// Monte Carlo truth association class					//
//                                                                      //
// Author: Marcel Kunze, Bochum University, Dec. 99			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "TNamed.h"

class TEventManager;
class TCandList;
class TCandidate;
class VAbsTruth;
class VAbsMicroCandidate;
class TDatabasePDG;
class TPidQuality;

class TAssociator : public TNamed {
    
public:
    //Constructor
    TAssociator(TEventManager &evtmgr, const char* name = "VAbsTruth/VAbsMicroCandidate");
    TAssociator(TCandList& mcTruth, TCandList& reco, const char* name = "VAbsTruth/VAbsMicroCandidate");
    //Destructor
    virtual ~TAssociator();
    
    void SetRecoPid(TCandidate *c);
    void SetMcPid(TCandidate *c);
    void SetMcAttributes(VAbsMicroCandidate &micro, Int_t index, Double_t cons=0.0);
    virtual TCandidate* McFromReco(TCandidate *c);
    virtual TCandidate* RecoFromMc(TCandidate *c);
    virtual Int_t McIndexFromReco(TCandidate *c)=0;
    virtual Int_t RecoIndexFromMc(TCandidate *c)=0;
    Bool_t IsAParticleToMatch(Int_t code) const;
    Bool_t IsAParticleToMatch(TCandidate *c) const;
    Bool_t IsAParticleToMatch(VAbsTruth *c) const;
    Bool_t BelongsToAShower(TCandidate *c) const;
    Double_t GetMatchConsistency(void) const { return fMatchConsistency; }
    TCandList* GetRecoList() const { return fRecoList; }
    TCandList* GetMcTruthList() const { return fMcList; }
    Double_t GetAssociation(Int_t m, Int_t i) const;
    void Associate(TCandList &l);
    void PrintOn( std::ostream& o=std::cout );

protected:
    enum MatchMethod { UNASSIGNED=0, CHARGED=1, ELECTRON, NEUTRAL, UNSTABLE, TEST };
    MatchMethod GetMatchMethod(Int_t pid) const;
    MatchMethod GetMatchMethod(TCandidate *c) const;
    MatchMethod GetMatchMethod(VAbsTruth *c) const;
    void CheckList() const;
    TEventManager	*fEvtMgr;	//!The event manager
    TCandList		*fRecoList;	//!The reconstructed particles
    TCandList		*fMcList;	//!The MC particles
    Double_t		fMatchConsistency; //!The matching quality
private:
    Bool_t		fOwnLists;	//!Owner of the lists
public:
    ClassDef(TAssociator,1) //Associaton of tracks to MC-Truth
	
};      

// standalone print
std::ostream&  operator << (std::ostream& o, TAssociator&);

class TMicroAssociator : public TAssociator {
    
public:
    TMicroAssociator(TEventManager &evtmgr, const char* name = "VAbsTruth/VAbsMicroCandidate");
    TMicroAssociator(TCandList& mcTruth, TCandList& reco, const char* name = "VAbsTruth/VAbsMicroCandidate");
    virtual ~TMicroAssociator();
    virtual Int_t McIndexFromReco(TCandidate *c);
    virtual Int_t RecoIndexFromMc(TCandidate *c);
public:
    ClassDef(TMicroAssociator,1) //Associaton of tracks to MC-Truth from micro candidate/file	
};      


class TChi2Associator : public TAssociator {
    
public:
    TChi2Associator(TEventManager &evtmgr, const char* name = "VAbsTruth/VAbsMicroCandidate");
    TChi2Associator(TCandList& mcTruth, TCandList& reco, const char* name = "VAbsTruth/VAbsMicroCandidate");
    virtual ~TChi2Associator();
    virtual Int_t McIndexFromReco(TCandidate *c);
    virtual Int_t RecoIndexFromMc(TCandidate *c);
    void SetDiffLimit(Double_t limit) { fDiffLimit = limit; }
    Double_t GetDiffLimit(void) const { return fDiffLimit; }  
private:
    Double_t fDiffLimit; //!Limit for chi2
public:
    ClassDef(TChi2Associator,1) //Associaton of tracks to MC-Truth via momentum	
};      


class TMapAssociator : public TAssociator {
    
public:
    TMapAssociator(TEventManager &evtmgr, const char* name = "VAbsTruth/VAbsMicroCandidate");
    TMapAssociator(TCandList& mcTruth, TCandList& reco, const char* name = "VAbsTruth/VAbsMicroCandidate");
    virtual ~TMapAssociator();
    virtual Int_t McIndexFromReco(TCandidate *c);
    virtual Int_t RecoIndexFromMc(TCandidate *c);
    void SetMinimumConsistency(Double_t c) { fMinConsistency = c; }
    Double_t GetMinimumConsistency(void) const { return fMinConsistency; }    
private:
    Double_t fMinConsistency; //!Threshold
public:
    ClassDef(TMapAssociator,1) //Associaton of tracks to MC-Truth via maps	
};      

class TMcAssocAssociator : public TAssociator {
    
public:
    TMcAssocAssociator(TEventManager &evtmgr, const char* name = "VAbsTruth/VAbsMicroCandidate");
    TMcAssocAssociator(TCandList& mcTruth, TCandList& reco, const char* name = "VAbsTruth/VAbsMicroCandidate");
    virtual ~TMcAssocAssociator();
    virtual Int_t McIndexFromReco(TCandidate *c);
    virtual Int_t RecoIndexFromMc(TCandidate *c);
    void SetMinimumConsistency(Double_t c) { fMinConsistency = c; }
    Double_t GetMinimumConsistency(void) const { return fMinConsistency; }    
private:
    Double_t fMinConsistency; //!Threshold
public:
    ClassDef(TMcAssocAssociator,1) //Associaton of tracks to MC-Truth via maps	
};      

#endif
