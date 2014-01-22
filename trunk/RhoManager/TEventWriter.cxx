//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TEventWriter								//
//                                                                      //
// Write data class to dump TAG and AOD					//
//                                                                      //
// Author: Marcel Kunze, RUB, Oct. 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////
#define DEBUG

#define KTAG      "TagTree"
#define SUFFTAG	  "Tag"
#define KAOD      "AodTree"
#define SUFFAOD	  "Aod"
#define KMAP      "CompTree"
#define SUFFMAP	  "Aod"
#define KMCT      "MctTree"
#define SUFFMCT	  "Mct"

#define TAGBRANCH  "TagList"
#define AODCBRANCH "AodListC"
#define AODNBRANCH "AodListN"
#define MCTBRANCH  "MctList"
#define ASSBRANCH  "AssList"
#define MAPBRANCH  "BtaCandMaps"

// Root includes

#include <stdlib.h>
#include "THashList.h"
#include "TClonesArray.h"

#include "RhoManager/TEventWriter.h"
#include "RhoManager/TEventCollection.h"
#include "RhoManager/TEventManager.h"
#include "RhoManager/TParameterManager.h"

#include "RhoBase/TRho.h"
#include "RhoBase/TCandidate.h"
#include "RhoBase/TCandList.h"
#include "RhoBase/TCandListIterator.h"
#include "RhoBase/VAbsTag.h"
#include "RhoBase/VAbsMicroCandidate.h"

#include "PAFSchema/PAFMcBase.h"
#include "PAFSchema/PAFStream.h"
#include "PAFSchema/PAFReader.h"
#include "PAFSchema/PAFMcAssocBase.h"
#include "PAFSchema/PAFChargedAodBase.h"
#include "PAFSchema/PAFNeutralAodBase.h"
#include "PAFSchema/PAFListBase.h"
#include "PAFSchema/PAFCompAodBase.h"
#include "PAFSchema/PAFEventBase.h"
#include "PAFSchema/PAFBitBase.h"

//#include "KangaSchema/BtaCandAssocMapR.h"

ClassImp(TEventWriter)

#include <iostream>
using namespace std;

TEventWriter::TEventWriter() : VAbsWriter("TEventWriter"),
fTagStream( 0 ),
fAodStream( 0 ),
fMctStream( 0 ),
fStreams( new THashList() ),
fEventNumber( 0 ),
fNumMcTracks( 0 ),
fTag(kTRUE),
fAod(kTRUE),
fMap(kFALSE),
fMct(kFALSE)
{
    fWriter = kFALSE;
    fWriteCollection = kFALSE;
    fWriteEvents = 0;
    fFirstEventWritten = kFALSE;
    fChargedAod  = new PAFChargedAodBase();
    fNeutralAod  = new PAFNeutralAodBase();
    fMcTruth     = new PAFMcBase();
    fMcAssoc     = new PAFMcAssocBase();
    fBtaAssoc    = 0; //new BtaCandAssocMapR();
    fGarbageBin  = new THashList();
    fTrackList   = new TCandList();
    fBits        = new PAFBitBase();
    fContent     = new PAFEventBase();
}

TEventWriter::TEventWriter(const char *file)
: VAbsWriter(file),
fTagStream( 0 ),
fAodStream( 0 ),
fMctStream( 0 ),
fStreams( new THashList() ),
fEventNumber( 0 ),
fNumMcTracks( 0 ),
fTag(kTRUE),
fAod(kTRUE),
fMap(kFALSE),
fMct(kFALSE)
{
    fWriter = kFALSE;
    fWriteCollection = kFALSE;
    fWriteEvents = 0;
    fFirstEventWritten = kFALSE;
    fChargedAod  = new PAFChargedAodBase();
    fNeutralAod  = new PAFNeutralAodBase();
    fMcTruth     = new PAFMcBase();
    fMcAssoc     = new PAFMcAssocBase();
    fBtaAssoc    = 0; //new BtaCandAssocMapR();
    fGarbageBin  = new THashList();
    fTrackList   = new TCandList();
    fBits        = new PAFBitBase();
    fContent     = new PAFEventBase();
    
    fWriter = kTRUE;
    TParameterManager *mgr = TRho::Instance()->GetParameterManager();
    SetStreamStatus("tag",mgr->GetBoolParm("tag"));
    SetStreamStatus("aod",mgr->GetBoolParm("aod"));
    SetStreamStatus("mct",mgr->GetBoolParm("mct"));
    SetStreamStatus("map",mgr->GetBoolParm("map"));
    InitWrite(file);
} 

TEventWriter::TEventWriter(VAbsCollection *collection)
: VAbsWriter(collection),
fTagStream( 0 ),
fAodStream( 0 ),
fMctStream( 0 ),
fStreams( 0 ),
fEventNumber( 0 ),
fNumMcTracks( 0 ),
fTag(kFALSE),
fAod(kFALSE),
fMap(kFALSE),
fMct(kFALSE)
{
    fWriter = kFALSE;
    fWriteCollection = kFALSE;
    fWriteEvents = 0;
    fFirstEventWritten = kFALSE;
    
    fWriteCollection = kTRUE;
    TString name(collection->ClassName());
    if (name!="TEventCollection") {
	cerr << "TEventWriter: Request to write to " << (const char *) collection->ClassName() << endl;
	cerr << "TEventWriter: Only supports TEventCollection" << endl;
	exit(0);
    }
    fCollection = collection;
} 

TEventWriter::~TEventWriter() 
{  
    if (fWriteCollection) {
	delete fCollection;
    }
    else {
	delete fChargedAod;
	delete fNeutralAod;
	delete fTrackList;
	delete fMcTruth;
	delete fMcAssoc;
	//delete fBtaAssoc;
	delete fBits;
	delete fContent;
    
	// Close all connected files
	TIter iter(fStreams);
	PAFStream *s = 0;
	while ( (s = (PAFStream *) iter.Next()) ) {
	    if (s->GetFile()) s->GetFile()->Close();
	}
    
	fStreams->Delete(); delete fStreams;
	fGarbageBin->Delete(); delete fGarbageBin;
    }
}

void TEventWriter::DumpList(TCandList *list)
{
    TCandListIterator iterTrk(*list);
    TCandidate* trk;
    while ( 0 != ( trk = iterTrk.Next()) ) {
	
	// Should we skip this entry ?
	for (int i=0;i<fGarbageBin->GetSize();i++) {
	    TCandidate *skip = (TCandidate *) fGarbageBin->At(i);
	    if (trk->Overlaps(*skip)) continue;
	}
	
	VAbsMicroCandidate &c = trk->GetMicroCandidate();
	if (c.GetCharge() == 0) {
	    ConvertCandidate(fNeutralAod,&c);
	    Fill(fNeutralAod);
	}
	else {
	    ConvertCandidate(fChargedAod,&c);
	    Fill(fChargedAod);
	}
    }
}

void TEventWriter::Exclude(TCandidate *c)
{
    static Int_t oldEvent = -1;
    if (GetEventNumber()!=oldEvent) {
	oldEvent = GetEventNumber();
	fGarbageBin->Delete();
    }

    fGarbageBin->Add((TObject*)c);
}

// __________________________ VAbsMicroCandidate -> PAFChargedAod ___________________________

void
TEventWriter::ConvertCandidate(PAFChargedAodBase *aod,VAbsMicroCandidate *cand)
{
    int i;
    
    aod->SetEmcLateralShape(cand->GetEmcLateralShape());
    aod->SetEmcSecondMoment(cand->GetEmcSecondMoment());
    aod->SetEmcZernikeMoment20(cand->GetEmcZernikeMoment20());
    aod->SetEmcZernikeMoment42(cand->GetEmcZernikeMoment42());
    aod->SetEmcS9S25(cand->GetEmcS9S25());
    aod->SetEmcS1S9(cand->GetEmcS1S9());
    aod->SetEmcRawEnergy(cand->GetEmcRawEnergy());
    aod->SetEmcCalEnergy(cand->GetEmcCalEnergy()); // New in 8.6.x
    aod->SetEmcStatus(cand->GetEmcStatus()); // New in 8.6.x
    aod->SetEmcNumberOfCrystals(cand->GetEmcNumberOfCrystals());
    aod->SetNeutralPidHypo(cand->GetNeutralPidHypo());
    aod->SetEmcNumberOfBumps(cand->GetEmcNumberOfBumps());
    aod->SetEmcConsistencyValue(cand->GetEmcConsistencyValue());
    aod->SetEmcCentroid(cand->GetEmcCentroid());
    //aod->SetEmcCovarianceEnergy(cand->GetEmcCovarianceEnergy());
    //aod->SetEmcCovariancePhi(cand->GetEmcCovariancePhi());
    //aod->SetEmcCovarianceTheta(cand->GetEmcCovarianceTheta());
    //aod->SetEmcCovarianceRho(cand->GetEmcCovarianceRho());
    
    aod->SetIfrPattern(cand->GetIfrPattern());
    aod->SetIfrMeasuredInteractionLengths(cand->GetIfrMeasuredInteractionLengths());
    aod->SetIfrExpectedInteractionLengths(cand->GetIfrExpectedInteractionLengths());
    aod->SetIfrInteractionLengthsBeforeIron(cand->GetIfrInteractionLengthsBeforeIron());
    aod->SetIfrTrackMatchConsistency(cand->GetIfrTrackMatchConsistency());
    aod->SetIfrEmcMatchConsistency(cand->GetIfrEmcMatchConsistency());
    aod->SetIfrFirstHit(cand->GetIfrFirstHit());
    aod->SetIfrLastHit(cand->GetIfrLastHit());
    aod->SetIfrNumberOfStrips(cand->GetIfrNumberOfStrips());
    for (i=0;i<20;i++) {
	aod->SetIfrNumberOfStrips(cand->GetIfrNumberOfStrips(i),i);
    }
    aod->SetIfrLastBarrel(cand->GetIfrLastBarrel());
    aod->SetIfrClusfitChi2(cand->GetIfrClusterFit()); // New in 8.6.x
    aod->SetIfrRpcMuProb(cand->GetIfrRpcMuonProbability()); // New in 8.6.x
    aod->SetIfrHitLayers(cand->GetIfrHitLayers());
    
    aod->SetSvtHitPattern(cand->GetSvtHitPattern());
    aod->SetDchInnerLayer(cand->GetDchInnerLayer());
    aod->SetDchOuterLayer(cand->GetDchOuterLayer());
    aod->SetDegreesOfFreedom(cand->GetDegreesOfFreedom());
    aod->SetProbability(cand->GetProbability());
    aod->SetChiSquared(cand->GetChiSquared());
    aod->SetDchHits(cand->GetDchHits());
    aod->SetSvtHits(cand->GetSvtHits());
    aod->SetChargedPidHypo(cand->GetChargedPidHypo());
    aod->SetFitStatus(cand->GetFitStatus());
    aod->SetTrackLength(cand->GetTrackLength());
    aod->SetTrackStartFoundRange(cand->GetTrackStartFoundRange());
    
    aod->SetDchMeanDEdx(cand->GetDchMeanDEdx());
    aod->SetSvtMeanDEdx(cand->GetSvtMeanDEdx());
    aod->SetDchSamples(cand->GetDchSamples());
    aod->SetSvtSamples(cand->GetSvtSamples());
    aod->SetDrcNumberOfPhotons(cand->GetDrcNumberOfPhotons());
    aod->SetDrcNumberOfBackgroundPhotons(cand->GetDrcNumberOfBackgroundPhotons());
    for (i=0;i<5;i++)
	aod->SetDrcNumberOfExpectedPhotons(cand->GetDrcNumberOfExpectedPhotons(i),i);
    aod->SetDrcThetaC(cand->GetDrcThetaC());
    aod->SetDrcThetaCError(cand->GetDrcThetaCError());
    aod->SetDrcNumBar(cand->GetDrcInBar()); // New in 8.5.x
    aod->SetDrcExit(cand->GetDrcExitBar()); // New in 8.5.x 
    aod->SetDrcXpos(cand->GetDrcXPos()); // New in 8.5.x 
    aod->SetEmcInterPhi(cand->GetDrcPhiAtEmc()); // New in 8.6.x
    aod->SetEmcInterTheta(cand->GetDrcThetaAtEmc()); // New in 8.6.x
    aod->SetDrcDeltaMomentum(cand->GetDrcDeltaMomentum()); // New in 8.5.x
    aod->SetDchDeltaMomentum(cand->GetDchDeltaMomentum());
    
    // Copy MC-Truth (PAF)
    
    aod->SetMcPid(cand->GetMcPid());
    
    // Copy PID info
    
    PAFAbsPidInfo svt;
    for (i=0;i<5;i++) {
	Float_t sl = cand->GetSvtPidInfo().GetSignificance(i);
	Float_t lhood = cand->GetSvtPidInfo().GetLikelihood(i);
	Int_t status = cand->GetSvtPidInfo().GetStatus(i);
	Int_t sign = cand->GetSvtPidInfo().GetSign(i);
	svt.SetStats(i, sl,lhood, status, sign);
    }
    aod->SetSvtPidInfo(svt);
    
    PAFAbsPidInfo dch;
    for (i=0;i<5;i++) {
	Float_t sl = cand->GetDchPidInfo().GetSignificance(i);
	Float_t lhood = cand->GetDchPidInfo().GetLikelihood(i);
	Int_t status = cand->GetDchPidInfo().GetStatus(i);
	Int_t sign = cand->GetDchPidInfo().GetSign(i);
	dch.SetStats(i, sl,lhood, status, sign);
    }
    aod->SetDchPidInfo(dch);
    
    PAFAbsPidInfo drc;
    for (i=0;i<5;i++) {
	Float_t sl = cand->GetDrcPidInfo().GetSignificance(i);
	Float_t lhood = cand->GetDrcPidInfo().GetLikelihood(i);
	Int_t status = cand->GetDrcPidInfo().GetStatus(i);
	Int_t sign = cand->GetDrcPidInfo().GetSign(i);
	drc.SetStats(i, sl,lhood, status, sign);
    }
    aod->SetDrcPidInfo(drc);
    
    PAFAbsPidInfo emc;
    for (i=0;i<5;i++) {
	Float_t sl = cand->GetEmcPidInfo().GetSignificance(i);
	Float_t lhood = cand->GetEmcPidInfo().GetLikelihood(i);
	Int_t status = cand->GetEmcPidInfo().GetStatus(i);
	Int_t sign = cand->GetEmcPidInfo().GetSign(i);
	emc.SetStats(i, sl,lhood, status, sign);
    }
    aod->SetEmcPidInfo(emc);
    
    PAFAbsPidInfo ifr;
    for (i=0;i<5;i++) {
	Float_t sl = cand->GetIfrPidInfo().GetSignificance(i);
	Float_t lhood = cand->GetIfrPidInfo().GetLikelihood(i);
	Int_t status = cand->GetIfrPidInfo().GetStatus(i);
	Int_t sign = cand->GetIfrPidInfo().GetSign(i);
	ifr.SetStats(i, sl,lhood, status, sign);
    }
    aod->SetIfrPidInfo(ifr);
    
    // FitParams
    aod->SetPosition(cand->GetPosition());
    TLorentzVector x = cand->GetLorentzVector();
    aod->SetLorentzVector(x);
    aod->SetCharge(cand->GetCharge());
    
    // Error Matrix
    aod->SetErrorP7((Float_t*)cand->GetErrorP7());
}

// __________________________ VAbsMicroCandidate -> PAFNeutralAod ___________________________

void
TEventWriter::ConvertCandidate(PAFNeutralAodBase *aod,VAbsMicroCandidate *cand)
{
    int i;
    
    aod->SetEmcLateralShape(cand->GetEmcLateralShape());
    aod->SetEmcSecondMoment(cand->GetEmcSecondMoment());
    aod->SetEmcZernikeMoment20(cand->GetEmcZernikeMoment20());
    aod->SetEmcZernikeMoment42(cand->GetEmcZernikeMoment42());
    aod->SetEmcS9S25(cand->GetEmcS9S25());
    aod->SetEmcS1S9(cand->GetEmcS1S9());
    aod->SetEmcRawEnergy(cand->GetEmcRawEnergy());
    aod->SetEmcCalEnergy(cand->GetEmcCalEnergy()); // New in 8.6.x
    aod->SetEmcStatus(cand->GetEmcStatus()); // New in 8.6.x
    aod->SetEmcNumberOfCrystals(cand->GetEmcNumberOfCrystals());
    aod->SetNeutralPidHypo(cand->GetNeutralPidHypo());
    aod->SetEmcNumberOfBumps(cand->GetEmcNumberOfBumps());
    aod->SetEmcConsistencyValue(cand->GetEmcConsistencyValue());
    aod->SetEmcCentroid(cand->GetEmcCentroid());
    //aod->SetEmcCovarianceEnergy(cand->GetEmcCovarianceEnergy());
    //aod->SetEmcCovariancePhi(cand->GetEmcCovariancePhi());
    //aod->SetEmcCovarianceTheta(cand->GetEmcCovarianceTheta());
    //aod->SetEmcCovarianceRho(cand->GetEmcCovarianceRho());
    
    aod->SetIfrPattern(cand->GetIfrPattern());
    aod->SetIfrMeasuredInteractionLengths(cand->GetIfrMeasuredInteractionLengths());
    aod->SetIfrExpectedInteractionLengths(cand->GetIfrExpectedInteractionLengths());
    aod->SetIfrInteractionLengthsBeforeIron(cand->GetIfrInteractionLengthsBeforeIron());
    aod->SetIfrTrackMatchConsistency(cand->GetIfrTrackMatchConsistency());
    aod->SetIfrEmcMatchConsistency(cand->GetIfrEmcMatchConsistency());
    aod->SetIfrFirstHit(cand->GetIfrFirstHit());
    aod->SetIfrLastHit(cand->GetIfrLastHit());
    aod->SetIfrNumberOfStrips(cand->GetIfrNumberOfStrips());
    for (i=0;i<20;i++) {
	aod->SetIfrNumberOfStrips(cand->GetIfrNumberOfStrips(i),i);
    }
    aod->SetIfrLastBarrel(cand->GetIfrLastBarrel());
    aod->SetIfrClusfitChi2(cand->GetIfrClusterFit()); // New in 8.6.x
    aod->SetIfrRpcMuProb(cand->GetIfrRpcMuonProbability()); // New in 8.6.x
    aod->SetIfrHitLayers(cand->GetIfrHitLayers());
    
    // Copy MC-Truth (PAF)
    
    aod->SetMcPid(cand->GetMcPid());
    
    // Copy PID info
    
    PAFAbsPidInfo emc;
    for (i=0;i<5;i++) {
	Float_t sl = cand->GetEmcPidInfo().GetSignificance(i);
	Float_t lhood = cand->GetEmcPidInfo().GetLikelihood(i);
	Int_t status = cand->GetEmcPidInfo().GetStatus(i);
	Int_t sign = cand->GetEmcPidInfo().GetSign(i);
	emc.SetStats(i, sl,lhood, status, sign);
    }
    aod->SetEmcPidInfo(emc);
    
    PAFAbsPidInfo ifr;
    for (i=0;i<5;i++) {
	Float_t sl = cand->GetIfrPidInfo().GetSignificance(i);
	Float_t lhood = cand->GetIfrPidInfo().GetLikelihood(i);
	Int_t status = cand->GetIfrPidInfo().GetStatus(i);
	Int_t sign = cand->GetIfrPidInfo().GetSign(i);
	ifr.SetStats(i, sl,lhood, status, sign);
    }
    aod->SetIfrPidInfo(ifr);
    
    // FitParams
    aod->SetPosition(cand->GetPosition());
    TLorentzVector x(cand->GetLorentzVector());
    aod->SetLorentzVector(x);
    aod->SetCharge(cand->GetCharge());
    
    // Error Matrix
    aod->SetErrorP7((Float_t*)cand->GetErrorP7());
}

// From PAFEvent

void TEventWriter::InitWrite(const char *fname) {
    Int_t BuffSize = 32000;
    Int_t splitlevel = 1;
    TString fFile(fname);
    
    InitializeStreams();
    
    // Authorize special writer account or default
    if (getenv("RHOUSERW")!=0)  {
	const char *user = getenv("RHOUSERW");
	const char *passwd = 0;
	if (getenv("RHOPASSWDW")!=0) passwd = getenv("RHOPASSWDW");
	TRho::Instance()->Authorize(user,passwd); 
    }
    else if (getenv("RHOUSER")!=0)
	TRho::Instance()->Authorize();
    
    TIter iter(fStreams);
    PAFStream *s = 0;
    while ( (s = (PAFStream *) iter.Next()) ) {
	
	s->SetFile(TFile::Open(fFile+s->GetFileNameBase()+".root","recreate"));
	cout << "TEventWriter::InitWrite open " << (const char *) (fFile+s->GetFileNameBase()+".root") << endl;
	s->GetFile()->SetCompressionLevel(2);
	TString comment("A Root TTree for "); 
	comment+= s->GetFileNameBase();
	s->SetTree(new TTree(s->GetTreeName(),comment));
	
	// iterate over all stream members and add corresponding branches to the tree: 
	for (int j=0; j < s->GetMembers()->GetEntriesFast(); j++) {
	    const PAFStreamMember *m= s->GetMember(j);
	    const char *name = m->fBranchName;
	    const char *classname =m->fClassName;
	    TTree *tree = s->GetTree();
	    void *address = m->fBranchAddress;
	    switch (m->fMemberType) {
	    case PAFStreamMember::ClonesArray: 
		tree->Branch(name,classname,address,BuffSize,splitlevel); break;
	    case PAFStreamMember::Leaflist:    
		tree->Branch(name,address,classname); break;
	    case PAFStreamMember::Object:      
		tree->Branch(name,classname,address,BuffSize,splitlevel); break;
	    }
	}
	
    }

    if (fAod && fAodStream) {
	fAodListsComp->SetMode(1); // Set to write - mode
	fAodStream->GetFile()->cd();
	fAodListsComp->SetTree(new TTree(KMAP,"Tree of comp. AOD - objects"));
    }
    
    if (fTag && fTagStream) fBits->SetTree(fTagStream->GetTree());
}	


void TEventWriter::InitUpdate(const char *fname) {
    // Open the files in update mode to append data
    TString fFile(fname);
    
    InitializeStreams();
    if (getenv("RHOUSER")!=0) TRho::Instance()->Authorize();
    
    TIter iter(fStreams);
    PAFStream *s = 0;
    while ( (s = (PAFStream *) iter.Next()) ) {
	
	if (s->IsActive()) {
	    
	    //s->SetFile( new TFile(fFile+s->GetFileNameBase()+".root","update") );
	    s->SetFile( TFile::Open(fFile+s->GetFileNameBase()+".root","update") );
	    s->SetTree( (TTree*)s->GetFile()->Get(s->GetTreeName()) );
	    
	    // iterate over all stream members and set the branch addresses: 
	    for (int j=0; j < s->GetMembers()->GetEntriesFast(); j++) {
		const PAFStreamMember *m= s->GetMember(j);
		s->GetTree()->GetBranch(m->fBranchName)->SetAddress(m->fBranchAddress);
	    }
	    
	}
    }
    
    fBits->SetTree(fTagStream->GetTree());
    
    fEventNumber= (UInt_t) fTagStream->GetTree()->GetEntries();
    cout<<"compression"<<fAodStream->GetFile()->GetCompressionLevel()<<endl;
}

void TEventWriter::NewEvent() {
    static int total = 1;
    // Prepare to store a new event
    fContent->SetTime(total++);
    fContent->NewEvent();
}

void TEventWriter::NewChargedCandidate() {
    // Store a new charged track
    fContent->NewChargedTrack();
}

void TEventWriter::NewNeutralCandidate() {
    // Store a new neutral track
    fContent->NewNeutralTrack();
}

// _______ Fill TagBase Only ______________________________

void TEventWriter::Fill( PAFTagBase* c ) {
    // Fill the TAG
    fContent->AddTag(c); 
}

void TEventWriter::Fill( VAbsBit* b ) {
    // Fill the BIT
    VAbsBitBoolIterator boolIter(b);
    while (boolIter.Next()) {
	fBits->SetBool(boolIter.Key(),boolIter.Value());
    } 
}

void TEventWriter::Fill( VAbsTag* c) {
    // We have to fill the PAFTagBase as well
    PAFTagBase &tag = fContent->GetTag();
    tag.SetNumberOfTracks(c->GetNumberOfTracks());
    tag.SetNumberOfGoodTracksLoose(c->GetNumberOfGoodTracksLoose());
    tag.SetNumberOfGoodTracksTight(c->GetNumberOfGoodTracksTight());
    tag.SetTotalMomentum(c->GetTotalMomentum());
    tag.SetTotalEnergy(c->GetTotalEnergy());
    tag.SetTotalMass(c->GetTotalMass());
    tag.SetPrimaryTrackMomentum(c->GetPrimaryTrackMomentum());
    tag.SetPrimaryTrackShowerEnergy(c->GetPrimaryTrackShowerEnergy());
    tag.SetPrimaryTrackHypothesis(c->GetPrimaryTrackHypothesis());
    tag.SetSecondaryTrackMomentum(c->GetSecondaryTrackMomentum());
    tag.SetSecondaryTrackShowerEnergy(c->GetSecondaryTrackShowerEnergy());
    tag.SetSecondaryTrackHypothesis(c->GetSecondaryTrackHypothesis());
    tag.SetTotalChargeLoose(c->GetTotalChargeLoose());
    tag.SetTotalChargeTight(c->GetTotalChargeTight());
    // Lepton Information
    //tag.SetPrimaryLeptonHypothesisLoose(c->GetPrimaryLeptonHypothesisLoose());
    //tag.SetSecondaryLeptonHypothesisLoose(c->GetSecondaryLeptonHypothesisLoose());
    //tag.SetPrimaryLeptonHypothesisTight(c->GetPrimaryLeptonHypothesisTight());
    //tag.SetSecondaryLeptonHypothesisTight(c->GetSecondaryLeptonHypothesisTight());
    //tag.SetPrimaryLeptonChargeLoose(c->GetPrimaryLeptonChargeLoose());
    //tag.SetSecondaryLeptonChargeLoose(c->GetSecondaryLeptonChargeLoose());
    //tag.SetPrimaryLeptonChargeTight(c->GetPrimaryLeptonChargeTight());
    //tag.SetSecondaryLeptonChargeTight(c->GetSecondaryLeptonChargeTight());
    //tag.SetPrimaryLeptonMomentumLoose(c->GetPrimaryLeptonMomentumLoose());
    //tag.SetSecondaryLeptonMomentumLoose(c->GetSecondaryLeptonMomentumLoose());
    //tag.SetPrimaryLeptonMomentumTight(c->GetPrimaryLeptonMomentumTight());
    //tag.SetSecondaryLeptonMomentumTight(c->GetSecondaryLeptonMomentumTight());
    // Calorimeter information
    tag.SetTotalNeutralEnergy(c->GetTotalNeutralEnergy());
    tag.SetHighestEnergyShower(c->GetHighestEnergyShower());
    tag.SetSecondHighestEnergyShower(c->GetSecondHighestEnergyShower());
    // Vertex and shape
    tag.SetPrimaryVertex(c->GetPrimaryVertex());
    tag.SetPrimaryVertexProbability(c->GetPrimaryVertexProbability());
    tag.SetThrust(c->GetThrust());
    tag.SetR2(c->GetR2());   
    // Particle numbers
    //tag.SetLeptonsLoose(c->GetLeptonsLoose());
    tag.SetElectronsLoose(c->GetElectronsLoose());
    tag.SetMuonsLoose(c->GetMuonsLoose());
    //tag.SetPionsLoose(c->GetPionsLoose());
    tag.SetKaonsLoose(c->GetKaonsLoose());
    tag.SetProtonsLoose(c->GetProtonsLoose());
    tag.SetGammasLoose(c->GetGammasLoose());
    tag.SetMergedPi0Loose(c->GetMergedPi0Loose());
    //tag.SetPi0Loose(c->GetPi0Loose());
    tag.SetKsLoose(c->GetKsLoose());
    //tag.SetLeptonsTight(c->GetLeptonsTight());
    tag.SetElectronsTight(c->GetElectronsTight());
    tag.SetMuonsTight(c->GetMuonsTight());
    //tag.SetPionsTight(c->GetPionsTight());
    tag.SetKaonsTight(c->GetKaonsTight());
    tag.SetProtonsTight(c->GetProtonsTight());
    tag.SetGammasTight(c->GetGammasTight());
    tag.SetMergedPi0Tight(c->GetMergedPi0Tight());
    //tag.SetPi0Tight(c->GetPi0Tight());
    tag.SetKsTight(c->GetKsTight());
    // New items for 8.8.0
    tag.SetBbqqDiscr(c->GetBbqqDiscr()); 
    tag.SetBGFR2(c->GetBGFR2());
    tag.SetBunchT0DeltaDircTrk(c->GetBunchT0DeltaDircTrk());
    tag.SetBunchT0DeltaFF(c->GetBunchT0DeltaFF()); 
    tag.SetBunchT0FinderErr(c->GetBunchT0FinderErr()); 
    tag.SetBunchT0FitterErr(c->GetBunchT0FitterErr());
    tag.SetECharShow(c->GetECharShow()); 
    tag.SetElecLoose1cm(c->GetElecLoose1cm());
    tag.SetElecLoose1l(c->GetElecLoose1l());
    tag.SetElecLoose2cm(c->GetElecLoose2cm()); 
    tag.SetElecLoose2l(c->GetElecLoose2l()); 
    tag.SetElecTight1cm(c->GetElecTight1cm());
    tag.SetElecTight1l(c->GetElecTight1l()); 
    tag.SetElecTight2cm(c->GetElecTight2cm()); 
    tag.SetElecTight2l(c->GetElecTight2l());
    tag.SetENeutShow(c->GetENeutShow());
    tag.SetL3OutBhabhaFlatOprValue(c->GetL3OutBhabhaFlatOprValue()); 
    tag.SetL3OutBhabhaFlatValue(c->GetL3OutBhabhaFlatValue());
    tag.SetMuonLoose1cm(c->GetMuonLoose1cm()); 
    tag.SetMuonLoose1l(c->GetMuonLoose1l()); 
    tag.SetMuonLoose2cm(c->GetMuonLoose2cm()); 
    tag.SetMuonLoose2l(c->GetMuonLoose2l()); 
    tag.SetMuonTight1cm(c->GetMuonTight1cm()); 
    tag.SetMuonTight1l(c->GetMuonTight1l()); 
    tag.SetMuonTight2cm(c->GetMuonTight2cm()); 
    tag.SetMuonTight2l(c->GetMuonTight2l());
    tag.SetPTotalScalar(c->GetPTotalScalar());
    tag.SetR2All(c->GetR2All());
    tag.SetSphericityAll(c->GetSphericityAll()); 
    tag.SetTagLooseProton_maxCmsP(c->GetTagLooseProton_maxCmsP()); 
    tag.SetTagTightProton_maxCmsP(c->GetTagTightProton_maxCmsP());
    tag.SetThrustCosThAll(c->GetThrustCosThAll()); 
    tag.SetThrustMagAll(c->GetThrustMagAll()); 
    tag.SetThrustPhiAll(c->GetThrustPhiAll());
    tag.SetD3PS(c->GetD3PS());
    tag.SetDmeson0(c->GetDmeson0()); 
    tag.SetDsmeson0(c->GetDsmeson0()); 
    tag.SetDstar0(c->GetDstar0());
    tag.SetDstar1(c->GetDstar1()); 
    //tag.SetLumiBhabhaWeight(c->GetLumiBhabhaWeight());
    tag.SetNumberOfNeutralCands(c->GetNumberOfNeutralCands());
    tag.SetNumberOfNeutralCndLoose(c->GetNumberOfNeutralCndLoose());
    tag.SetNumberOfNeutralCndTight(c->GetNumberOfNeutralCndTight());
    tag.SetTagEtaPeppTwoBody_maxCmsPmaxCosT(c->GetTagEtaPeppTwoBody_maxCmsPmaxCosT());
    tag.SetTagEtaPrhogTwoBody_maxCmsPmaxCosT(c->GetTagEtaPrhogTwoBody_maxCmsPmaxCosT());
    tag.SetTagEtaTwoBody_maxCmsPmaxCosT(c->GetTagEtaTwoBody_maxCmsPmaxCosT());
    tag.SetTagKstpTwoBody_maxCmsPmaxCosT(c->GetTagKstpTwoBody_maxCmsPmaxCosT()); 
    tag.SetTagKstTwoBody_maxCmsPmaxCosT(c->GetTagKstTwoBody_maxCmsPmaxCosT());
    tag.SetTagKTwoBody_maxCmsPmaxCosT(c->GetTagKTwoBody_maxCmsPmaxCosT());
    tag.SetTagK_S0TwoBody_maxCmsPmaxCosT(c->GetTagK_S0TwoBody_maxCmsPmaxCosT());
    tag.SetTagOmegaTwoBody_maxCmsPmaxCosT(c->GetTagOmegaTwoBody_maxCmsPmaxCosT()); 
    tag.SetTagPhiTwoBody_maxCmsPmaxCosT(c->GetTagPhiTwoBody_maxCmsPmaxCosT());
    tag.SetTagPi0TwoBody_maxCmsPmaxCosT(c->GetTagPi0TwoBody_maxCmsPmaxCosT());
    tag.SetTagRhopTwoBody_maxCmsPmaxCosT(c->GetTagRhopTwoBody_maxCmsPmaxCosT()); 
    tag.SetTagRhoTwoBody_maxCmsPmaxCosT(c->GetTagRhoTwoBody_maxCmsPmaxCosT());
    Fill(&tag);
}

// _______ Fill AodBase Only _______________________________

Int_t TEventWriter::Fill( PAFAodBase* c ) {
    // Fill the AOD (either charged or neutral)
    if (c->GetCharge()==0) {
	fContent->AddNeutralTrack();
	PAFNeutralAodBase *cn = (PAFNeutralAodBase*) c;
	new ((*fAodListNeutral)[fContent->GetNumberOfNeutralTracks()-1]) PAFNeutralAodBase(*cn);
	return fContent->GetNumberOfNeutralTracks()-1;
    } else {
	fContent->AddChargedTrack();
	PAFChargedAodBase *cc = (PAFChargedAodBase*) c;
	new ((*fAodListCharged)[fContent->GetNumberOfChargedTracks()-1]) PAFChargedAodBase(*cc);
	return fContent->GetNumberOfChargedTracks()-1;
    }
}

void TEventWriter::Fill( PAFMcBase* mc ) 
{
    if (!mc) {
	cout << "TEventWriter::Fill( PAFMcBase* ) : error (nil)" << endl;
	return;
    }
    new (  (*fMctList)[fNumMcTracks++] ) PAFMcBase( *mc );
}

void TEventWriter::Fill( PAFMcAssocBase* mc ) 
{
    if (!mc) {
	cout << "TEventWriter::Fill( PAFMcAssocBase* ) : error (nil)" << endl;
	return;
    }
    new (  (*fAssList)[fNumMcAssoc++] ) PAFMcAssocBase( *mc );
}

void TEventWriter::Fill( BtaCandAssocMapR* mc ) 
{
/*
    if (!mc) {
	cout << "TEventWriter::Fill( BtaCandAssocMapR* ) : error (nil)" << endl;
	return;
    }
    new (  (*fBtaList)[fNumBtaMcAssoc++] ) BtaCandAssocMapR( *mc );
*/
}

// _____ Fill Tree ___________________________________________

void TEventWriter::Fill() {
    // Fill all trees
    fTimeStampMajor = fContent->GetMajorID();
    fTimeStampMinor = fContent->GetMinorID();
    TIter iter(fStreams);
    PAFStream *s = 0;
    while ( (s = (PAFStream *) iter.Next()) ) {
	if (s->IsActive()) s->GetTree()->Fill();
    }
    if (fMap) fAodListsComp->Fill();
    fWriteEvents++;
}

void TEventWriter::InitializeStreams()
{
    // Allocate PAF streams
    cout << "TEventWriter: Allocate PAF streams" << endl;
    if (fTag) fTagStream = NewStream(SUFFTAG,KTAG); cout << "Add Tag-Stream" << endl; 
    if (fAod) fAodStream = NewStream(SUFFAOD,KAOD); cout << "Add Aod-Stream" << endl; 
    if (fMct) fMctStream = NewStream(SUFFMCT,KMCT); cout << "Add Mct-Stream" << endl;
    
    // initialize the "common blocks" (sorry for using fortran slang)
    
    fAodListsComp  = new PAFListBase();
    fAodListCharged = new TClonesArray("PAFChargedAodBase");
    fAodListNeutral = new TClonesArray("PAFNeutralAodBase");
    fMctList = new TClonesArray("PAFMcBase");
    fNumMcTracks = 0;
    fAssList = new TClonesArray("PAFMcAssocBase");
    fNumMcAssoc = 0;
    fBtaList = 0; //new TClonesArray("BtaCandAssocMapR");
    fNumBtaMcAssoc = 0;
    
    // Set up the default streams:

    if (fTagStream) {
	fTagStream->AddObject("",TAGBRANCH,&fContent,"PAFEventBase");
	fTagStream->AddLeaflist("","TStampMajor",&fTimeStampMajor,"i");
	fTagStream->AddLeaflist("","TStampMinor",&fTimeStampMinor,"i");
	fTagStream->Enable();
    }
    
    if (fAodStream) {
	fAodStream->AddClonesArray("",AODCBRANCH,&fAodListCharged,"PAFChargedAodBase");
	fAodStream->AddClonesArray("",AODNBRANCH,&fAodListNeutral,"PAFNeutralAodBase");
	fAodStream->AddLeaflist("","TStampMajor",&fTimeStampMajor,"i");
	fAodStream->AddLeaflist("","TStampMinor",&fTimeStampMinor,"i");
	fAodStream->Enable();
    }
    
    if (fMctStream) {
	fMctStream->AddClonesArray("",MCTBRANCH,&fMctList,"PAFMcBase");
	fMctStream->AddClonesArray("",ASSBRANCH,&fAssList,"PAFMcAssocBase");
	//fMctStream->AddClonesArray("",MAPBRANCH,&fBtaList,"BtaCandAssocMapR");
	fMctStream->AddLeaflist("","NumberOfTracks",&fNumMcTracks,"I");
	fMctStream->AddLeaflist("","TStampMajor",&fTimeStampMajor,"i");
	fMctStream->AddLeaflist("","TStampMinor",&fTimeStampMinor,"i");
	fMctStream->Enable();
    }

}

void TEventWriter::ClearBuffer() {
    // Reset all buffers
    if (fAodStream!=0 && fAodStream->IsActive() ) 
    { 
	fAodListCharged->Clear(); fAodListNeutral->Clear();
	if (fMap)  fAodListsComp->Reset(); 
    }
    

    if ( fMctStream!=0 && fMctStream->IsActive() ) {
	fMctList->Clear();
	fNumMcTracks = 0;
	fAssList->Clear();
	fNumMcAssoc = 0;
	//fBtaList->Clear();
	fNumBtaMcAssoc = 0;
    }
}

void TEventWriter::Store() {
    if (fWriter) {
	// Write the trees to file
	cout<<"Writing "<<endl;
	//write files
	TIter iter(fStreams);
	PAFStream *s = 0;
	while ( (s = (PAFStream *) iter.Next()) ) {
	    if (s->IsActive() && s->GetFile() ) {
		cout << s->GetFileNameBase();
		s->GetFile()->Write();
		s->GetFile()->Close();
	    }
	}
	cout << endl;
	cout<<"Wrote "<<fWriteEvents<<" events"<<endl;
    }
    if (fWriteCollection) {
	delete fCollection;
	cout<<"Collected "<<fWriteEvents<<" events"<<endl;
    }
}

PAFStream* TEventWriter::NewStream(const char* AFileNameBase, const char* ATreeName)
{
    PAFStream *s= new PAFStream(AFileNameBase,ATreeName);  
    fStreams->Add(s);
    return s;
}

UInt_t TEventWriter::GetRunNumber() const {
    // Return the actual run number
    return fContent->GetRunNumber();
}

UInt_t TEventWriter::GetEventNumber() const {
    // Return the actual event number
    return fContent->GetEventNumber();
}

// Access to object handles

PAFEventBase& TEventWriter::GetHeader() const {
    // Return a handle to the event header
    return *fContent;
}    

VAbsTag& TEventWriter::GetTag() const {
    // Return a handle to the TAG
    return *fBits;
}

VAbsBit& TEventWriter::GetBit() const {
    // Return a handle to the BIT
    return *fBits;
}    

PAFListBase& TEventWriter::GetListBase() const {
    if (fAodListsComp->GetTree()==0) {
	fAodListsComp->SetMode(1); // Set to write - mode
	//fAodStream->GetFile()->cd();
	fAodListsComp->SetTree(new TTree("CompTree","Tree of comp. AOD - objects"));
    }
    return *fAodListsComp;
}

Bool_t TEventWriter::SetStreamStatus(const char *stream,Bool_t status)
{
    TString s(stream);
    if (s=="tag")
	fTag = status;
    else if (s=="aod"||s=="micro")
	fAod = status;
    else if (s=="mct")
	fMct = status;
    else if (s=="map"||s=="readMaps")
	fMap = status;
    else {
	cerr << "PAFReader::SetStreamStatus: Stream not defined " << (const char *) stream << endl;
	return kFALSE;
    }

    TString stat("on");
    if (!status) stat = "off";
    cout << "TEventWriter::SetStreamStatus: " << (const char *) s << "=" << (const char *) stat << endl;

    return kTRUE;
}

void TEventWriter::WriteEvent(TEventManager *evtmgr, TCandList *list)
{
#ifdef DEBUG
    cout << "TEventWriter " << (const char *) GetName() << ": Write event #" << evtmgr->GetEventNumber() << endl;
#endif

    if (fWriteCollection) {
	VAbsCollection *collection = GetCollection();
	if (collection!=0) {
	    fWriteEvents++;
	    evtmgr->AddEventToCollection(collection);
	    //collection->Fill(GetNumberOfEventsProcessed(),GetMajorID(),GetMinorID());
	}
	return;
    }

    if (Map()) {
#ifdef DEBUG
	cout << "Lists in the event:" << evtmgr->GetNumberOfLists() << endl;
	for (int i=0;i<evtmgr->GetNumberOfLists();i++) 
	    cout << evtmgr->GetListName(i) << endl;
#endif
	// Set up the default lists
	static Bool_t initialized = kFALSE;
	if (!initialized) {
	    initialized = kTRUE;
	    TTree *originalListTree = evtmgr->GetListBase().GetTree();
	    if (originalListTree!=0) {
		GetListBase().AddList((char *)"TaggingList");
		GetListBase().AddList((char *)"NeutralHadEMCNeutral");
		GetListBase().AddList((char *)"CalorClusterNeutral");
		GetListBase().AddList((char *)"SingleBumpNeutralClusters");
		GetListBase().AddList((char *)"pi0MergedDefault");
		GetListBase().AddList((char *)"eLoose");
		GetListBase().AddList((char *)"eTight");
		GetListBase().AddList((char *)"eDefault");
		GetListBase().AddList((char *)"muLoose");
		GetListBase().AddList((char *)"muTight");
		GetListBase().AddList((char *)"muDefault");
		GetListBase().AddList((char *)"piLoose");
		GetListBase().AddList((char *)"piTight");
		GetListBase().AddList((char *)"piDefault");
		GetListBase().AddList((char *)"KLoose");
		GetListBase().AddList((char *)"KTight");
		GetListBase().AddList((char *)"KDefault");
		GetListBase().AddList((char *)"pLoose");
		GetListBase().AddList((char *)"pTight");
		GetListBase().AddList((char *)"pDefault");
		GetListBase().AddList((char *)"MergedPi0s");
		GetListBase().AddList((char *)"KsDefault");
		GetListBase().AddList((char *)"KsLoose");
		GetListBase().AddList((char *)"KsTight");
	    }
	}
    }
    
    NewEvent();
    
    GetHeader().SetMajorID(evtmgr->GetMajorID());
    GetHeader().SetMinorID(evtmgr->GetMinorID());
    GetHeader().SetRunNumber(evtmgr->GetRunNumber());
    
    // Copy Tag 
    //if (Tag()) Fill(&GetTag());
    if (Tag()) Fill(&GetBit());
    
    if (Aod()) {
	// Copy micro
	if (list != 0) { // Dump tracks in the list
	    DumpList(list);
	}
	else { // Dump all tracks in the event
	    static TCandList tracks;
	    evtmgr->Fill(&tracks);
	    DumpList(&tracks);
	}
    }
    
    // Copy Lists
    
    // The reading of the lists translates Kanga->PAF
    
    if (Map()) {
	
	GetListBase().Reset();
	PAFListBase& theListBase = evtmgr->GetListBase();
	theListBase.Reset();

	for (int i=0;i<evtmgr->GetNumberOfLists();i++) {
	    TCandList dummy;
	    TString name = evtmgr->GetListName(i);
	    evtmgr->FillFromList(&dummy,name); // Try to get the list from evm
#ifdef DEBUG
	    cout << "Read list " << name.Data() << ":" << dummy.GetNumberOfCandidates() << endl;
#endif
	}
	
	// Set up a copy of the lists in the writing event manager
	// Always add the lists, as new lists might show up
	TTree *originalListTree = evtmgr->GetListBase().GetTree();
	if (originalListTree!=0) {
	    TObjArray *branchList = originalListTree->GetListOfBranches();
	    for (Int_t k=0; k<branchList->GetEntriesFast(); k++) {
		TBranch *br = (TBranch*) ((*branchList)[k]);
		GetListBase().AddList((char*)br->GetName());
		GetListBase().SetActiveList(br->GetName());
		TClonesArray *originalList = evtmgr->GetListBase().GetList(br->GetName(),evtmgr->GetCurrentIndex());
		Int_t originalListLen = evtmgr->GetListBase().GetListLength(br->GetName(),evtmgr->GetCurrentIndex());
		for (Int_t l=0; l<originalListLen; l++) {
		    PAFCompAodBase *b1 = (PAFCompAodBase*) ((*originalList)[l]);
		    GetListBase().AddCompAodBase(*b1);
		}
#ifdef DEBUG
		cout << "Write list " << br->GetName() << ":" << originalListLen << endl;
#endif
	    }
	}
    }

    
    // Copy MC-truth
    
    if (Mct()) {
	for (int j=0;j<evtmgr->GetNumberOfMcTruthTracks();j++)
	    Fill(&evtmgr->GetReader()->GetMcBase(j));
	for (int k=0;k<evtmgr->GetReader()->GetNumberOfCandAssocs();k++) {
	    Fill(&evtmgr->GetReader()->GetMcAssocBase(k));
	    Fill(&evtmgr->GetReader()->GetCandAssocMap(k));
	}
    }
    
    // Dump all 
    Fill();
    ClearBuffer();
}
