//////////////////////////////////////////////////////////////////////////
// Analysis
//
// Sample analysis program to create spectra of K0s, JPsi and B
// (inspired by BetaSampleAnalysis)
//
// Author: Sascha Berger and Marcel Kunze, Bochum University
// March '99
//////////////////////////////////////////////////////////////////////////

// Read the candidates from a predefined list
#undef PERSISTENTLIST

// Use original BABAR particle selectors or PAF implementation
#undef BETAPID
#undef PAFPID

#include "RhoExamples/Analysis.h"

#include <stdlib.h>

#include "TROOT.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "RhoBase/TRho.h"
#include "RhoBase/TCandidate.h"
#include "RhoBase/TCandList.h"
#include "RhoBase/TCandListIterator.h"
#include "RhoHistogram/THistogram.h"
#include "RhoSelector/TPidSelector.h"
#include "RhoSelector/TPidNNOSelector.h"
#include "RhoSelector/TPidTruthSelector.h"
#include "RhoSelector/TSimpleVertexSelector.h"
#include "RhoTools/TBVariables.h"
#include "RhoConditions/TConditions.h"


#include "RhoManager/TAnalysis.h"
#include "RhoManager/TEventManager.h"
#include "RhoManager/TCandListManager.h"
#include "RhoManager/TPersistenceManager.h"
#include "RhoManager/TParameterManager.h"

#include <iostream>
using namespace std;

enum selectors{smicro,snno,struth,sbeta}; // Selectors to choose

TROOT root("Analysis","Rho Analysis Example");
TApplication *theApp = new TApplication("Rho Analysis",0,0);

// Main program to set parameters and drive the analysis

int main (int argc, char *argv[])
{
    // Create a named Framework and register services
    TRho Rho("Sample Analysis");
    
    // Instantiate the analysis and add a sample module
    // Parameter presets can be overriden from file or command line
    TAnalysis *analysis = new TAnalysis(argc,argv);    
    JpsiKs *theModule = new JpsiKs("JpsiKs","Sample Analysis Module");
    analysis->Add(theModule);
    analysis->SetParm("file","$RHO/Data/test/runjpsiks310"); // Name of the input file
    analysis->SetParm("model",snno);	// Selector model to use
    analysis->SetParm("tagcut","");	// Apply a cut on the tag attributes
    analysis->SetParm("graphics",kTRUE);// Produce live updating histograms
    
    // Register a manager to handle I/O of objects (histos)
    // The "SetPersistenceManager()" method does not take ownership of the stored objects
    Rho.SetPersistenceManager(new TPersistenceManager()); 
    
    // Register a manager to handle the candidate lists
    Rho.RegisterService(new TCandListManager()); 
    
    // Register a manager to handle conditions DB (for RhoTools)
    Rho.RegisterService(new TConditions("$RHO/Data/CondDB/BaBarConditions.root")); 
    
    // Register the analysis as a service and execute it
    Rho.RegisterService(analysis);    
    Rho.GetAnalysis()->Run();
    
    return 0;
}

// Analysis class inplementation

Bool_t JpsiKs::BeginJob(TEventManager *eventmgr)
{
    // Instantiate a vertex selector
    
    vertexSelector   = new TSimpleVertexSelector(); 
    vertexSelector->SetDistanceOfClosestApproach(2.0);	// DOCA of the two tracks
    vertexSelector->SetAngle(1.0);			// Opening of cone to primary vertex
    cout << *vertexSelector;
    
    // Instantiate particle selectors
    
    chargedSelector = new TPidChargedSelector(); 
    plusSelector    = new TPidPlusSelector(); 
    minusSelector   = new TPidMinusSelector(); 
    
    Int_t sel = GetParm("Framework::model");
    
    if (sel == smicro) {
#ifdef PAFPID
	cout << "Using PAF Microselectors" << endl;
	// Use standard selectors (Micro)
	electronSelector    = new TPidElectronSelector("electronSelector");
	muonSelector	    = new TPidMuonSelector("muonSelector");
	pionSelector	    = new TPidNNOPionSelector("pionSelector",kTRUE,kTRUE,kTRUE,kTRUE,kFALSE);
	kaonSelector	    = new TPidKaonSelector("kaonSelector");
	muonSelector->SetParm("criteria","veryLoose");
	pionSelector->SetParm("criteria","veryLoose");
#else
	cout << "Compiled w/o support for PAFPidSelectors." << endl;
#endif
    }
    else if (sel == sbeta) {
#ifdef BETAPID
	cout << "Using BetaPid Microselectors" << endl;
	// Use standard selectors (Micro)
	electronSelector    = new PAFPidElectronMicroSelector("electronSelector");
	muonSelector	    = new PAFPidMuonMicroSelector("muonSelector");
	pionSelector	    = new PAFPidRoyPionSelector("pionSelector");
	kaonSelector	    = new PAFPidRoyKaonSelector("kaonSelector");
	muonSelector->SetCriterion("veryLoose"); muonSelector->SetParms();
	pionSelector->SetCriterion("veryLoose"); pionSelector->SetParms();
#else
	cout << "Compiled w/o support for BetaPidSelectors." << endl;
#endif
    }
    else if (sel == snno) {
	cout << "Using NNOselectors" << endl;
	// Use neural net selectors and set criteria for particle selection
	electronSelector    = new TPidNNOElectronSelector("electronSelector",kTRUE,kTRUE,kTRUE,kTRUE,kFALSE); //(electron,muon,pion,kaon,proton)
	muonSelector	    = new TPidNNOMuonSelector("muonSelector",kTRUE,kTRUE,kTRUE,kTRUE,kFALSE); //(electron,muon,pion,kaon,proton)
	pionSelector	    = new TPidNNOPionSelector("pionSelector",kTRUE,kTRUE,kTRUE,kTRUE,kFALSE); //(electron,muon,pion,kaon,proton)
	kaonSelector	    = new TPidNNOKaonSelector("kaonSelector",kTRUE,kTRUE,kTRUE,kTRUE,kFALSE); //(electron,muon,pion,kaon,proton)
	muonSelector->SetParm("criteria","best");
	pionSelector->SetParm("criteria","best");
    }
    else {
	cout << "Using Truthselectors" << endl;
	SetParm("mct",kTRUE);
	// Use standard selectors (MC-Truth)
	electronSelector    = new TPidTruthElectronSelector();
	muonSelector	    = new TPidTruthMuonSelector();
	pionSelector	    = new TPidTruthPionSelector();
	kaonSelector	    = new TPidTruthKaonSelector();
    }
    
    // Define mass windows for BG reduction (Activated with USECUTS)
    
    ksMassSelector	    = new TPidMassSelector("KsMass",0.4977,0.150); 
    jpsiMassSelector	    = new TPidMassSelector("JPsiMass",3.0969,0.200); 
    
    // Set up the lists for event processing
    
    TCandListManager *listmgr = TRho::Instance()->GetCandListManager();
    trackList	= listmgr->Put(new TCandList("trackList"));
    chargedList	= listmgr->Put(new TCandList("chargedList"));
    plusList	= listmgr->Put(new TCandList("plusList"));
    minusList	= listmgr->Put(new TCandList("minusList"));
    compositeList   = listmgr->Put(new TCandList("compositeList"));
    pionList	= listmgr->Put(new TCandList("pionList"));
    muonList	= listmgr->Put(new TCandList("muonList"));
    ksList	= listmgr->Put(new TCandList("ksList"));
    jpsiRawList	= listmgr->Put(new TCandList("jpsiRawList"));
    jpsiList	= listmgr->Put(new TCandList("jpsiList"));
    bList	= listmgr->Put(new TCandList("bList"));
    
    TPersistenceManager *persmgr = TRho::Instance()->GetPersistenceManager();
    persmgr->SetDir("1D-Spectra");
    mTrkHist	    = persmgr->MassHistogram("mTrkHist","Mass of all Candidates in trkList",200,0,1);
    pTrkHist	    = persmgr->MomentumHistogram("pTrkHist","Momentum of all Candidates in trkList",200,0,5);
    mPionHist	    = persmgr->MassHistogram("mPionHist","Mass of all Candidates in pionList",200,0,1);
    mMuonHist	    = persmgr->MassHistogram("mMuonHist","Mass of all Candidates in muonList",200,0,1);
    mCombinedTrkHist    = persmgr->MassHistogram("mCombinedTrkHist","Mass of all Pair-Composites from trkList",200,0,4);
    mCombinedPionHist   = persmgr->MassHistogram("mCombinedPionHist","Mass of all PiPi-Composites from ksList (K0s->PiPi)",200,0,4);
    mCombinedPionHist->SetFillColor(42); // Paint it brown
    mCombinedMuonHist   = persmgr->MassHistogram("mCombinedMuonHist","Mass of all MuMu-Composites from jpsiList (J/Psi->MuMu)",200,0,4);
    mCombinedMuonHist->SetFillColor(46); // Paint it red
    pCombinedPionHist   = persmgr->MomentumHistogram("pCombinedPionHist","Momentum of all PiPi-Composites in ksList (K0s->PiPi)",200,0,4);
    pCombinedMuonHist   = persmgr->MomentumHistogram("pCombinedMuonHist","Momentum of all MuMu-Composites in jpsiList (J/Psi->MuMu)",200,0,4);
    mBHist		= persmgr->MassHistogram("mBHist","Mass of B-Candidates",200,4.5,5.5);
    pxBHist		= persmgr->MomentumHistogram("pxBHist","x-Momentum of B-Candidates",200,-5,5);
    pyBHist		= persmgr->MomentumHistogram("pyBHist","y-Momentum of B-Candidates",200,-5,5);
    pzBHist		= persmgr->MomentumHistogram("pzBHist","z-Momentum of B-Candidates",200,-5,5);
    
    // Set up 2d-histograms
    
    persmgr->SetDir("2D-Spectra");
    eoverpHist	= persmgr->EoverPHistogram("eoverpHist","E over P for all Candidates",100,0,4.5,100,0,4.5);
    deltavse	= persmgr->Histogram("DeltaE over E for all B Candidates in bList",100,4.5,5.5,100,-1.0,1.0);
    
    // Initialise live updating histograms
    // The upper pad holds combination of two prongs (raw and selector)
    // The lower pad shows the invariant mass of B-candidates
    
    if (GetParm("Framework::graphics")) {
	c1 = new TCanvas("c1","Rho Analysis Framework Example",200,10,600,800);
	c1->Clear();
	c1->Divide(1,3);
	c1->SetGrid();
	c1->cd(1);
	mCombinedTrkHist->Draw("e1p");
	mCombinedPionHist->Draw("same");
	mCombinedMuonHist->Draw("same");
	c1->cd(2);
	mBHist->Draw("e1p");
	c1->cd(3);
	deltavse->Draw();
    }
    
    return kTRUE;
}

Bool_t JpsiKs::EndJob(TEventManager *eventmgr) 
{
    // Save the histograms
    TRho::Instance()->GetPersistenceManager()->Store();

    return kTRUE;
}

Bool_t JpsiKs::Event(TEventManager *eventmgr)
{
    // The event loop implementation is called for each accepted
    // event (e.g. for events that pass through the "tagcut")

    cout<<"Accepted event "<<eventmgr->GetEventNumber()<<endl;
#ifdef PERSISTENTLIST
    eventmgr->FillFromList(trackList,"TaggingList");
#else
    eventmgr->Fill(trackList);
#endif
    if (Verbose()) cout<<*eventmgr<<endl;
    
    // Filter and accumulate lists according to particle species
    
    chargedSelector->Select(*trackList,*chargedList);        // Filter charged particles
    plusSelector->Select(*chargedList,*plusList);            // Filter positive particles
    minusSelector->Select(*chargedList,*minusList);          // Filter negative particles
    compositeList->Combine( *plusList, *minusList );        // Pair combo of all tracks
    
#ifdef USECUTS
    // Select the composites through mass windows
    ksMassSelector->Select(*compositeList,*ksList);
    jpsiMassSelector->Select(*compositeList,*jpsiList);
#else
    // Define the composites with PID selectors
    pionSelector->Select(*chargedList,*pionList);            // Filter pions
    muonSelector->Select(*chargedList,*muonList);            // Filter muons
    vertexSelector->Combine( *pionList,*pionList,*ksList);  // Go for K0s
    jpsiList->Combine( *muonList,*muonList);                // Go for JPsi
#endif
    bList->Combine( *ksList, *jpsiList );                   // Go for B
    
    //Fill mass and momentum histogramms
    
    if (GetParm("Framework::graphics")) c1->cd(1);
    *mTrkHist   << *chargedList;   // This is the original information
    *pTrkHist   << *chargedList;
    *mPionHist  << *pionList;
    *mMuonHist  << *muonList;
    *eoverpHist << *chargedList; // Produce an E/P plot for all candidates
    
    *mCombinedTrkHist  << *compositeList; // Information from composites
    *mCombinedPionHist << *ksList;
    *mCombinedMuonHist << *jpsiList;
    *pCombinedPionHist << *ksList;
    *pCombinedMuonHist << *jpsiList;
    
    if (GetParm("Framework::graphics")) c1->cd(2);
    *mBHist << *bList;                 // Sample the B-mass and momentum
    pxBHist->Accumulate(*bList,1);
    pyBHist->Accumulate(*bList,2);
    pzBHist->Accumulate(*bList,3);
    
    // Draw beam substituted mass
    if (GetParm("Framework::graphics")) c1->cd(3);
    TCandListIterator iter(*bList);
    TCandidate *c = 0;
    while (c = iter.Next()) { 
	TLorentzVectorErr myBCandLabP4 = c->P4();
	TBVariables bVars(myBCandLabP4);
	deltavse->Accumulate(bVars.M_ES(),bVars.DeltaE());
    }
    
    if (GetParm("Framework::graphics") && (eventmgr->GetEventNumber()%50) == 0) {
	c1->cd(1);
	gPad->Modified();
	gPad->Update();
	c1->cd(2);
	gPad->Modified();
	gPad->Update();
	c1->cd(3);
	gPad->Modified();
	gPad->Update();
    }
    
    return kTRUE;
}

