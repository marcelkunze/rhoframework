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

// The associators create a match index into the Reco/MCTruth list.
// The base class methods McFromReco and RecoFromMc can be used to translate 
// the indices into TCandidates.

#include "TDatabasePDG.h"

#include "RhoManager/TAssociator.h"
#include "PAFSchema/PAFMcAssocBase.h"
#include "RhoManager/TEventManager.h"
#include "RhoBase/VAbsReader.h"
#include "RhoBase/VAbsTruth.h"
#include "RhoBase/TRho.h"
#include "RhoBase/TCandidate.h"
#include "RhoBase/TCandList.h"
#include "RhoBase/TCandListIterator.h"
#include "RhoBase/VAbsMicroCandidate.h"

ClassImp(TAssociator)

#include <iostream>
using namespace std;

TAssociator::TAssociator(TEventManager &evtmgr, const char* name) :
fEvtMgr(&evtmgr), TNamed(name,name),fMatchConsistency(0.0)
{
    fRecoList = new TCandList();
    fMcList = new TCandList();
    fOwnLists = kTRUE;
}

TAssociator::TAssociator(TCandList& mcTruth, TCandList& reco, const char* name) :
fEvtMgr(0), TNamed(name,name),fMatchConsistency(0.0)
{
    fRecoList = &reco;
    fMcList = &mcTruth;
    fOwnLists = kFALSE;
}


TAssociator::~TAssociator() 
{
    if (fOwnLists) {
	delete fRecoList;
	delete fMcList;
    }
}

void TAssociator::Associate(TCandList &l) 
{
    Int_t nmicro = fRecoList->GetNumberOfTracks();
    Float_t *bestMatch = new float[nmicro];
    for (int n=0;n<nmicro;n++)  bestMatch[n] = 0.0;

    Int_t nl = l.GetLength();
    for (Int_t i=0;i<nl;++i) {
	TCandidate *cand = &l[i];
	Int_t index = RecoIndexFromMc(cand);
	Float_t consistency = GetMatchConsistency();

	if (index>=0 && index<nmicro) {
	    VAbsMicroCandidate &micro = TRho::Instance()->GetEventManager()->GetMicroCandidate(index);
	    if (&micro == 0) {
		cerr << "TAssociator::Associate: Non-existent micro #" << i << endl;
		continue;
	    }
	    if (consistency>bestMatch[index]) { // Found a better match
	      bestMatch[index] = consistency;
	      if (cand->PdtEntry()!=0)
	        micro.SetMcPid(cand->PdtEntry()->PdgCode());
	      else
		cerr << "TAssociator::Associate: Can not set type for #" << i << endl;
	      micro.SetMcConsistency(consistency);
	      micro.SetMcIndex(i);
	      cand->SetMicroCandidate(micro);
	    }
	}
    }

    delete bestMatch;
}

TCandidate* TAssociator::McFromReco(TCandidate *c)
{
#ifdef DEBUG
    cout << "TAssociator::McFromReco: Uses " << this->ClassName() << endl;
#endif
    if (c == 0) return 0; // Nothing to do
    VAbsMicroCandidate &micro = c->GetMicroCandidate();
    if (&micro == 0) return 0; // This is not a MicroCandidate
    
    Int_t index = McIndexFromReco(c);
    
    if (index>=0 && index<fMcList->GetNumberOfTracks()) {
	SetMcAttributes(micro,index,GetMatchConsistency()); // Note the matching results
	return fMcList->Get(index);
    }
    
    return 0;
}

TCandidate* TAssociator::RecoFromMc(TCandidate *c)
{
#ifdef DEBUG
    cout << "TAssociator::RecoFromMc: Uses " << this->ClassName() << endl;
#endif
    TCandidate *result = 0;
    if (c == 0) return result; // Nothing to do
    
    Int_t index = RecoIndexFromMc(c);
    if (index>=0 && index<fRecoList->GetNumberOfTracks()) {
	result = fRecoList->Get(index);
	VAbsMicroCandidate &micro = result->GetMicroCandidate();
	if (&micro == 0) return 0;			    // This is not a MicroCandidate
	SetMcAttributes(micro,c->Uid(),GetMatchConsistency());  // Note the matching results
    }
    
    return result;
}

void TAssociator::PrintOn(std::ostream& o) {
    o << "TAssociator " <<  GetName() << endl;
    CheckList(); // Force reading the lists...
    TCandListIterator iter(*fRecoList);
    TCandidate *c=0;
    Int_t n = 1;
    while ( (c = iter.Next()) ) {	
	TCandidate *mc = McFromReco(c);
	if (c->PdtEntry() == 0) SetRecoPid(c);
	cout << "Track #" << n++ << '\t' << c->PdtEntry()->GetName() << '\t';
	if (mc != 0) {
	    cout << "matched to MC-track #" << c->GetMicroCandidate().GetMcIndex() 
		<< '\t' << mc->PdtEntry()->GetName() << " (" << GetMatchConsistency() << ")" << endl;
	}
	else
	    cout << "not matched" << endl;
    }
    
}        

void TAssociator::CheckList() const
{
    if (!fOwnLists) return;
    
    static Int_t oldEvent = -1;
    if (fEvtMgr != 0 && fEvtMgr->GetEventNumber()!=oldEvent) {
	fEvtMgr->Fill(fRecoList);
	fEvtMgr->FillMcTruthList(fMcList);
	oldEvent = fEvtMgr->GetEventNumber();
    }
}

Bool_t TAssociator::IsAParticleToMatch(TCandidate *c) const
{
    return IsAParticleToMatch(TMath::Abs(c->PdtEntry()->PdgCode()));
}

Bool_t TAssociator::IsAParticleToMatch(VAbsTruth *c) const
{
    return IsAParticleToMatch(TMath::Abs(c->GetPdtEntry()->PdgCode()));
}

Bool_t TAssociator::IsAParticleToMatch(Int_t pid) const
{
    if (pid==11||pid==13||pid==211||pid==321||pid==2212|| // e,mu,pi,K,p
	pid==22||pid==130||pid==2112) return kTRUE; // gamma, KL, n
    return kFALSE;
}

Bool_t TAssociator::BelongsToAShower(TCandidate *c) const
{
    // Check if a particle belongs to a hadronic shower
    // Compare the sum of daughter energies to the mother
    if (c==0) return kFALSE;
    VAbsTruth *mc = (VAbsTruth *) c->McTruth();
    if (mc==0) return kFALSE;
    if (mc->GetStartVertex().Mag()>100.0) return kTRUE; // Too far outside

    VAbsTruth *mother = mc->GetMother();
    if (mother==0) return kFALSE;
    if (!IsAParticleToMatch(mother)) return kFALSE; // Only check the stable particles

    if (mother->GetNumberOfDaughters()>4) {
#ifdef DEBUG
	cout << mc->GetPdtEntry()->GetName() 
	    << " belongs to a shower from " << mother->GetPdtEntry()->GetName() << 
	    " with " << mother->GetNumberOfDaughters() << " particles: " << endl;
	for (int i=0;i<mother->GetNumberOfDaughters();i++) {
	    VAbsTruth *daughter = (VAbsTruth *) mother->GetDaughter(i);
	    cout << daughter->GetPdtEntry()->GetName() << " ";
	}
	cout << endl;
#endif	
	return kTRUE;
    }
    
    return kFALSE;
}

void TAssociator::SetRecoPid(TCandidate *c)
{
    Int_t pdgCode = 0; // Rootino
    Int_t charge = (Int_t) c->GetCharge();
    if (charge != 0) {
	switch(c->GetMicroCandidate().GetChargedPidHypo()) { // From Reco
	case 0: pdgCode = charge*11; break; // Electron
	case 1: pdgCode = charge*13; break; // Muon
	case 2: pdgCode = charge*211; break; // Pion
	case 3: pdgCode = charge*321; break; // Kaon
	case 4: pdgCode = charge*2212; break; // Proton
	}
    }
    else
	pdgCode = 22;  // Gamma
    
    c->SetType(TRho::Instance()->GetPDG()->GetParticle(pdgCode));
}

void TAssociator::SetMcPid(TCandidate *c)
{
    Int_t pdgCode = 0; // Rootino
    Int_t charge = (Int_t) c->GetCharge();
    
    TCandidate *mc;
    if ( (mc = McFromReco(c)) )
	pdgCode = mc->PdtEntry()->PdgCode();
    
    c->SetType(TRho::Instance()->GetPDG()->GetParticle(pdgCode));
    
    VAbsMicroCandidate &micro = c->GetMicroCandidate();
    
    if (charge != 0) {
	switch(pdgCode) { // From Reco
	case 11: case -11: micro.SetMcPid(0); break;	    // Electron
	case 13: case -13: micro.SetMcPid(1); break;	    // Muon
	case 211: case -211: micro.SetMcPid(2); break;	    // Pion
	case 321: case -321: micro.SetMcPid(3); break;	    // Kaon
	case 2212: case -2212: micro.SetMcPid(4); break;    // Proton
	}
    }
}

void TAssociator::SetMcAttributes(VAbsMicroCandidate &micro, Int_t index, Double_t cons)
{
    if (index >= 0 && index<fMcList->GetNumberOfTracks()) {
	TCandidate *mc = fMcList->Get(index);
	const VAbsMicroCandidate &m = mc->GetMicroCandidate();
	if (&m==0) mc->SetMicroCandidate(micro);    // Associate the measurement
	micro.SetMcPid(mc->PdtEntry()->PdgCode());  // Note the true type
	micro.SetMcIndex(index);		    // Note the index
	micro.SetMcConsistency(cons);		    // Note the match quality
    }
}

std::ostream&  operator << (std::ostream& o, TAssociator& a) { a.PrintOn(o); return o; }


// Implementation of a micro/file based Associator (Available from T files)

ClassImp(TMicroAssociator)

TMicroAssociator::TMicroAssociator(TEventManager &evtmgr, const char* name) :
TAssociator(evtmgr,name)
{}

TMicroAssociator::TMicroAssociator(TCandList& mcTruth, TCandList& reco, const char* name) :
TAssociator(mcTruth,reco,name)
{}


TMicroAssociator::~TMicroAssociator() 
{}

Int_t TMicroAssociator::RecoIndexFromMc(TCandidate *c)
{   
    fMatchConsistency = 0.0;
    if (c == 0) return -1; // Nothing to do
    // Not yet implemented
    return -1;
}

Int_t TMicroAssociator::McIndexFromReco(TCandidate *c)
{
#ifdef DEBUG
    cout << "TMicroAssociator::McIndexFromReco:" << endl;
#endif
    fMatchConsistency = 0.0;
    if (c == 0) return -1; // Nothing to do
    VAbsMicroCandidate &micro = c->GetMicroCandidate();
    if (&micro==0) return -1;
    fMatchConsistency = micro.GetMcConsistency();
    return micro.GetMcIndex();
}



// Implementation of a Chi2 based Associator (Compare 4-momenta)

ClassImp(TChi2Associator)

TChi2Associator::TChi2Associator(TEventManager &evtmgr, const char* name) :
TAssociator(evtmgr,name)
{
    fDiffLimit = 100.;
}

TChi2Associator::TChi2Associator(TCandList& mcTruth, TCandList& reco, const char* name) :
TAssociator(mcTruth,reco,name)
{
    fDiffLimit = 100.;
}


TChi2Associator::~TChi2Associator() 
{}

Int_t TChi2Associator::RecoIndexFromMc(TCandidate *c)
{
    Int_t index = -1;
    fMatchConsistency = 0.0;
    if (c == 0) return index; // Nothing to do
    if (!IsAParticleToMatch(c)) return index;
    
    CheckList();
    
    Int_t cmc = (Int_t) c->GetCharge();
    
    Double_t minDiff2 = fDiffLimit;
    
    if (index<=0) { // Try to associate by comparison of momenta
	Double_t chi2 = 0.0;
	TVector3 pm = c->P3();
	for (int i=0;i<fRecoList->GetNumberOfTracks();i++) {
	    TCandidate &reco = (*fRecoList)[i];
	    Int_t cmr = (Int_t) reco.GetCharge();
	    if (cmr == 0) { // Match neutral
		if (cmc != 0) continue;
		TVector3 mcP3 = reco.P3();
		const double dot = mcP3 * pm;
		const double cang = dot/(pm.Mag()*mcP3.Mag());
		const double ang = TMath::ACos(cang);
		const double deltaang = 0.001*1.414*(-2.17*log(mcP3.Mag())+4.51);  
		chi2 = ((ang*ang)/(deltaang*deltaang));
	    }
	    else { // Match charged
		if (cmc == 0) continue;
		TVector3 v = pm - reco.P3();
		chi2 = 100. * v.Mag();
	    }
	    
	    if (chi2<minDiff2&&!BelongsToAShower(c)) {
		minDiff2 = chi2;
		index = i;
	    }
	}
    }
    
    if (minDiff2>0.0) {
	fMatchConsistency = 1.0/minDiff2;
	if (fMatchConsistency>1.0) fMatchConsistency = 1.0;
    }
    
    return index;
}

Int_t TChi2Associator::McIndexFromReco(TCandidate *c)
{
#ifdef DEBUG
    cout << "TChi2Associator::McIndexFromReco:" << endl;
#endif
    Int_t index = -1;
    fMatchConsistency = 0.0;
    if (c == 0) return index; // Nothing to do
    
    CheckList();
    
    VAbsMicroCandidate &micro = c->GetMicroCandidate();
    if (&micro == 0) return -1;
    
    Int_t cm = micro.GetCharge();
    Double_t minDiff2 = fDiffLimit;
    
    if (index<=0||cm==0) { // Try to associate by comparison of momenta
	Double_t chi2 = 0.0;
	TVector3 pm = micro.GetMomentum();
	for (int i=0;i<fMcList->GetNumberOfTracks();i++) {
	    TCandidate *mc = fMcList->Get(i);
	    if (!IsAParticleToMatch(mc)) continue;
	    Int_t cmc = (Int_t) mc->GetCharge();
	    if (cm == 0) { // Match neutral
		if (cmc != 0) continue;
		TVector3 mcP3 = mc->P3();
		const double dot = mcP3 * pm;
		const double cang = dot/(pm.Mag()*mcP3.Mag());
		const double ang = TMath::ACos(cang);
		const double deltaang = 0.001*1.414*(-2.17*log(mcP3.Mag())+4.51);  
		chi2 = ((ang*ang)/(deltaang*deltaang));
	    }
	    else { // Match charged
		if (cmc == 0) continue;
		TVector3 v = pm - mc->P3();
		chi2 = 100. * v.Mag();
	    }
	    
	    if (chi2<minDiff2&&!BelongsToAShower(mc)) {
		minDiff2 = chi2;
		index = i;
	    }
	}
    }
    
    if (minDiff2>0.0) {
	fMatchConsistency = 1.0/minDiff2;
	if (fMatchConsistency>1.0) fMatchConsistency = 1.0;
    }
    
    return index;
}


// Implementation of a MCTruth based associator

ClassImp(TMapAssociator)

TMapAssociator::TMapAssociator(TEventManager &evtmgr, const char* name) :
TAssociator(evtmgr,name)
{
    fMinConsistency = 0.0;
}

TMapAssociator::TMapAssociator(TCandList& mcTruth, TCandList& reco, const char* name) :
TAssociator(mcTruth,reco,name)
{
    fMinConsistency = 0.0;
}


TMapAssociator::~TMapAssociator() 
{}

Int_t TMapAssociator::RecoIndexFromMc(TCandidate *c)
{
    Int_t index = -1;
    fMatchConsistency = 0.0;
    if (c == 0) return index; // Nothing to do
    //if (!IsAParticleToMatch(c)) return index;
    
    CheckList();
    
    Int_t cmc = (Int_t) c->GetCharge();
    Int_t trackNumber = c->GetTrackNumber();
    
    if (index<=0) {
	Double_t matchConsistency = 0.0;
	for (int i=0;i<fRecoList->GetNumberOfTracks();i++) {
	    TCandidate *reco = fRecoList->Get(i);
	    Int_t cmr = (Int_t) reco->GetCharge();
	    if (cmr == 0) { // Match neutral
		if (cmc != 0) continue;
		matchConsistency = GetAssociation(i,trackNumber);
	    }
	    else { // Match charged
		if (cmc == 0) continue;
		matchConsistency = GetAssociation(i,trackNumber);
	    }
	    
	    if (matchConsistency>fMatchConsistency) {
		fMatchConsistency = matchConsistency;
		index = i;
	    }
	}
	
	if (fMatchConsistency<fMinConsistency) index = -1; // No good match
    }
    
#ifdef DEBUG
    cout << "RecoIndexFromMc: MC track #" << c->uid() << " matches reco track #" << index << endl;
#endif
    
    return index;
}

Int_t TMapAssociator::McIndexFromReco(TCandidate *c)
{
#ifdef DEBUG
    cout << "TMapAssociator::McIndexFromReco:" << endl;
#endif
    Int_t index = -1;
    fMatchConsistency = 0.0;
    if (c == 0) return index; // Nothing to do
    
    CheckList();
    
    Int_t trackNumber = c->GetTrackNumber();
    Int_t cm = (Int_t) c->GetCharge();
    
    if (index<=0||cm==0) {
	Double_t matchConsistency = 0.0;
	for (int i=0;i<fMcList->GetNumberOfTracks();i++) {
	    TCandidate *mc = fMcList->Get(i);
	    if (!IsAParticleToMatch(mc)) continue;
	    Int_t cmc = (Int_t) mc->GetCharge();
	    if (cm == 0) { // Match neutral
		if (cmc != 0) continue;
		matchConsistency = GetAssociation(trackNumber,i);
	    }
	    else { // Match charged
		if (cmc == 0) continue;
		matchConsistency = GetAssociation(trackNumber,i);
	    }
	    
	    if (matchConsistency>fMatchConsistency) {
		fMatchConsistency = matchConsistency;
		index = i;
	    }
	}
	
	if (fMatchConsistency<fMinConsistency) index = -1; // No good match
    }
    
#ifdef DEBUG
    cout << "McIndexFromReco: Reco track #" << c->uid() << " matches MC track #" << index << endl;
#endif
    
    return index;
}


Double_t TAssociator::GetAssociation(Int_t track, Int_t mc) const
{
    // Try to read the AssocMap
    // Look for the best corresponding entry and assign micro mapping
    
#define NEVT 128
#define NMCT 512
    
    TEventManager *evtMgr = TRho::Instance()->GetEventManager();
    
    // Fill the cache
    static Float_t mcAssoc[NEVT][NMCT];
    static Int_t oldEvent = -1;
    if (evtMgr != 0 && evtMgr->GetEventNumber()!=oldEvent) {
	UInt_t nevt = evtMgr->GetNumberOfTracks();
	UInt_t nmct = evtMgr->GetNumberOfMcTruthTracks();
#ifdef DEBUG
	cout << "Install McAssoc table [" << nevt << "][" << nmct << "]" << endl;
#endif
	if (nevt>NEVT) { cerr << "TMapAssociator::GetAssociation: NEVT<" << nevt << endl; nevt = NEVT; }
	if (nmct>NMCT) { cerr << "TMapAssociator::GetAssociation: NMCT<" << nmct << endl; nmct = NMCT; }
	for (int i=0;i<nevt;i++)
	    for (int j=0;j<nmct;j++) mcAssoc[i][j] = 0.0;
	    
        VAbsReader *reader = evtMgr->GetReader();
	if (reader==0) return 0.0;

	for (int j=0;j<reader->GetNumberOfCandAssocs();j++) {
	    PAFMcAssocBase &map = reader->GetMcAssocBase(j);
	    if (&map!=0) {
		Int_t mctru = map.GetMcTru();
		Int_t micro = map.GetMicro();
		Float_t cons = map.GetConsistency();
		if (micro>=NEVT) continue;
		if (mctru>=NMCT) continue;
		mcAssoc[micro][mctru] = cons;
#ifdef DEBUG
		cout << "Fill#" << j << " Micro#" << micro << "->MCBase#" << mctru << " " << cons << " ";
		cout << " " << (evtMgr->GetMcTruth(mctru)).GetPdtEntry()->GetName();
		cout << endl;
#endif
	    }
	}

	oldEvent = evtMgr->GetEventNumber();
    }
    
    // Return the assoc consisntency
#ifdef DEBUG
    if (mcAssoc[track][mc]>0.0) {
	cout << "Micro#" << track << "->MCBase#" << mc << " " << mcAssoc[track][mc] << " ";
	cout << (evtMgr->GetMcTruth(mc)).GetPdtEntry()->GetName() << endl;
    }
#endif
    
    return mcAssoc[track][mc];
}   


// Another implementation of a MCTruth based associator

ClassImp(TMcAssocAssociator)

TMcAssocAssociator::TMcAssocAssociator(TEventManager &evtmgr, const char* name) :
TAssociator(evtmgr,name)
{
    fMinConsistency = 0.0;
}

TMcAssocAssociator::TMcAssocAssociator(TCandList& mcTruth, TCandList& reco, const char* name) :
TAssociator(mcTruth,reco,name)
{
    fMinConsistency = 0.0;
}


TMcAssocAssociator::~TMcAssocAssociator() 
{}

Int_t TMcAssocAssociator::RecoIndexFromMc(TCandidate *c)
{
    Int_t index = -1;
    fMatchConsistency = 0.0;
    if (c == 0) return index; // Nothing to do
    //if (!IsAParticleToMatch(c)) return index;
    
    CheckList();
    
#ifdef TMCASSOC
    TEventManager *evtMgr = TRho::Instance()->GetEventManager();
    static TMcAssoc *assoc = 0;
    static Int_t oldEvent = -1;
    if (evtMgr->GetEventNumber()!=oldEvent) {
	oldEvent = evtMgr->GetEventNumber();
        static TCandList canonicalList;
        evtMgr->GetReader()->FillCanonicalMcTruthList(&canonicalList);
        delete assoc; // Remove the old one and generate new
	assoc = evtMgr->GetProxyDict()->GetTMcAssoc(*fRecoList,canonicalList);
    }
    
    if (assoc==0) {
	cerr << "TMcAssocAssociator::RecoFromMc: Could not create TMcAssoc" << endl;
	return -1;
    }
    
    TCandidate *match = assoc->RecoFromMC(c);
    if (match==0) return index;
    index = match->GetTrackNumber();
    fMatchConsistency = GetAssociation(index,c->uid());
#else
    
    Int_t cmc = (Int_t) c->GetCharge();
    Int_t trackNumber = c->GetTrackNumber();
    
    if (index<=0) {
	Double_t matchConsistency = 0.0;
	for (int i=0;i<fRecoList->GetNumberOfTracks();i++) {
	    TCandidate *reco = fRecoList->Get(i);
	    Int_t cmr = (Int_t) reco->GetCharge();
	    if (cmr == 0) { // Match neutral
		if (cmc != 0) continue;
		matchConsistency = GetAssociation(i,trackNumber);
	    }
	    else { // Match charged
		if (cmc == 0) continue;
		matchConsistency = GetAssociation(i,trackNumber);
	    }
	    
	    if (matchConsistency>fMatchConsistency) {
		fMatchConsistency = matchConsistency;
		index = i;
	    }
	}
	
	if (fMatchConsistency<fMinConsistency) index = -1; // No good match
    }
#endif
    
#ifdef DEBUG
    cout << "RecoIndexFromMc: MC track #" << c->uid() << " matches reco track #" << index << endl;
#endif
    
    return index;
}

Int_t TMcAssocAssociator::McIndexFromReco(TCandidate *c)
{
#ifdef DEBUG
    cout << "TMcAssocAssociator::McIndexFromReco:" << endl;
#endif
    Int_t index = -1;
    fMatchConsistency = 0.0;
    if (c == 0) return index; // Nothing to do
    
    CheckList();
    
#ifdef TMCASSOC
    TEventManager *evtMgr = TRho::Instance()->GetEventManager();
    
    static TMcAssoc *assoc = 0;
    static Int_t oldEvent = -1;
    
    if (evtMgr->GetEventNumber()!=oldEvent) {
	oldEvent = evtMgr->GetEventNumber();
        static TCandList canonicalList;
        evtMgr->GetReader()->FillCanonicalMcTruthList(&canonicalList);
        delete assoc; // Remove the old one and generate new
        assoc = evtMgr->GetProxyDict()->GetTMcAssoc(*fRecoList,canonicalList);
    }
    
    if (assoc==0) {
	cerr << "TMcAssocAssociator::McFromReco: Could not create TMcAssoc" << endl;
	return 0;
    }
    
    TCandidate *match = assoc->McFromReco(c);
    if (match==0) return index;
    
    index = match->GetTrackNumber();
    fMatchConsistency = GetAssociation(c->uid(),index);
#else
    
    Int_t trackNumber = c->GetTrackNumber();
    Int_t cm = (Int_t) c->GetCharge();
    
    if (index<=0||cm==0) {
	Double_t matchConsistency = 0.0;
	for (int i=0;i<fMcList->GetNumberOfTracks();i++) {
	    TCandidate *mc = fMcList->Get(i);
	    if (!IsAParticleToMatch(mc)) continue;
	    Int_t cmc = (Int_t) mc->GetCharge();
	    if (cm == 0) { // Match neutral
		if (cmc != 0) continue;
		matchConsistency = GetAssociation(trackNumber,i);
	    }
	    else { // Match charged
		if (cmc == 0) continue;
		matchConsistency = GetAssociation(trackNumber,i);
	    }
	    
	    if (matchConsistency>fMatchConsistency) {
		fMatchConsistency = matchConsistency;
		index = i;
	    }
	}
	
	if (fMatchConsistency<fMinConsistency) index = -1; // No good match
    }
#endif
    
#ifdef DEBUG
    cout << "McIndexFromReco: Reco track #" << c->uid() << " matches MC track #" << index << endl;
#endif
    
    return index;
}


