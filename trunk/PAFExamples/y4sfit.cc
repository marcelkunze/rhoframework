//--------------------------------------------------------------------------
// Examine the scan run, count number of Bhabha and hadronic events.
// Perform a fit on the Y4S.
//
//
// Author: Marcel Kunze, Bochum University
//         Roland Waldi, TUD, Y4S-fit
//
// June '99     : Initial version (mk)
// November '99 : Inherit from TAnalysis, added PAFConditions support (mk)
// Jan. 2k      : Use TModule for scan and fit
//
//--------------------------------------------------------------------------


// We have to ommit these lines if the program is executed as a macro

#ifndef __CINT__
// ROOT headers

#include "TApplication.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TF1.h"


// PAF headers

#include "PAFExamples/y4s.hh"
#include "PAFExamples/y4sfit.hh"

#include "RhoBase/TRho.h"
#include "RhoManager/TPersistenceManager.h"
#include "RhoSelector/TEventSelector.h"
#include "RhoManager/TAnalysis.h"
#include "RhoConditions/TBeams.h"

#include <iostream>
using namespace std;

Int_t y4s(int argc, char**argv);
Int_t main(int argc, char**argv) { return y4s(argc,argv); }

#endif


//--------------------------------------------------------------------------

// Global data (this has to be changed)

UInt_t  nruns(0);
Float_t b[MAXRUN], h[MAXRUN], ecms[MAXRUN];


//--------------------------------------------------------------------------
// Constructors

y4scan::y4scan(const char* const theName, const char* const theDescription) : 
TModule(theName, theDescription),
oldrun(0), counter(0), totalEvents(0),
acceptedBhabhas(0), acceptedHadrons(0), nbhabhas(0),
nhadrons(0), cmsenergy(0), eTime(0)
{
}


// Destructors

y4scan::~y4scan() { }


// Function members

Bool_t y4scan::BeginJob(TEventManager*)
{   
    // The BeginJob is executed at the beginning.
    // We set up a persistence manager to handle histograms and
    // event selectors to count the Bhabha and hadronic events
    
    TPersistenceManager *persmgr = TRho::Instance()->GetPersistenceManager();
    cms      = persmgr->Histogram("CMS energy of run",400,5850,6250);
    bhabhas  = persmgr->Histogram("Number of Bhabha events",400,5850,6250);
    hadrons  = persmgr->Histogram("Number of Hadron events",400,5850,6250);
    
    TParameterManager *parmgr = TRho::Instance()->GetParameterManager();
    if (!parmgr->GetBoolParm("aod")) {
	bhabhaSelector = (TBhabhaTagSelector*) new TBhabhaTagSelector(); 
	hadronSelector = (THadronTagSelector*) new THadronTagSelector();
    }
    else {
	parmgr->SetBoolParm("aod",kTRUE);
	bhabhaSelector = (TBhabhaAodSelector*) new TBhabhaAodSelector(); 
	hadronSelector = (THadronAodSelector*) new THadronAodSelector(); 
    }
    
    cout << endl << "Event selectors:" << endl << endl;
    cout << *bhabhaSelector;
    cout << *hadronSelector;  
    
    return kTRUE;
}    


Bool_t y4scan::EndRun(TEventManager*)
{
    // The EndRun function is executed at the end of each run.
    // We ask the conditions object for the conditions of the current event.
    // Caution: Here, the first event of the new run is already present.
    // Therefore we have to gather statistics on the basis of the old run.
    // If the CMS does not change, do nothing.
    
    TBeams *beams = TRho::Instance()->GetConditions()->GetBeams();
    Double_t newcms = beams->EnergyCM();
    
    if (cmsenergy==0) cmsenergy = newcms;
    
    if (TMath::Abs(newcms-cmsenergy)<1.E-3) return kTRUE;
    
    Count();
    
    cmsenergy = newcms;
    oldrun = rNumber;
    
    cout << endl << "New conditions for run # " << rNumber << '\t' << newcms << " GeV" << endl;
    if (Verbose()) TRho::Instance()->GetConditions()->PrintOn(cout);
    
    return kTRUE;
}


void y4scan::Count()
{
    // This function counts the number of Bhabha and hadronic events
    // acquired for any period of equal CMS energy.
    
    bhabhas->Accumulate(oldrun,acceptedBhabhas);
    hadrons->Accumulate(oldrun,acceptedHadrons);
    b[nruns] = acceptedBhabhas;
    h[nruns] = acceptedHadrons;
    ecms[nruns++] = cmsenergy;
    
    cout << "Acquired statistics for range of runs #" << oldrun << "-" << rNumber-1 << endl;
    cout << "Accepted Bhabha events: " << acceptedBhabhas << endl;
    cout << "Accepted hadron events: " << acceptedHadrons << endl;
    cout << "of " << totalEvents << " at ";
    cout << cmsenergy << " GeV " << endl;
    
    cms->Accumulate(oldrun,cmsenergy);
    
    totalEvents = acceptedBhabhas = acceptedHadrons = 0;
}


Bool_t y4scan::Event(TEventManager *eventmgr) 
{
    // Read the beam conditions database for each event (using timestamp).
    // Classify the events (Bhabha or hadronic) using the event selectors.
    
    counter++;
    
    // Get event information from header
    
    rNumber = eventmgr->GetRunNumber();
    eNumber = eventmgr->GetEventNumber();
    eTime   = eventmgr->GetMajorID();		    // Event time in secs since 1901
    
    TRho::Instance()->GetConditions()->At(eTime);			    // Read the beam conditions DB
    
    if (oldrun == 0) oldrun = rNumber;		    // Set the first run
    
    // Make use of event selectors to count event types
    
    Bool_t isBhabha = kTRUE;
    Bool_t isHadron = kTRUE;
    
    isBhabha = isBhabha && bhabhaSelector->Accept(*eventmgr); 
    isHadron = isHadron && hadronSelector->Accept(*eventmgr);
    
    if (!isBhabha && !isHadron) return kFALSE;	      // Not interesting...
    
    if (isBhabha) { 
	nbhabhas++;
	acceptedBhabhas++; 
    }
    
    if (isHadron) { 
	nhadrons++;
	acceptedHadrons++; 
    }
    
    totalEvents++;
    
    return kTRUE;
}


Bool_t y4scan::EndJob(TEventManager*)
{
    // At the end, perform a Y(4S) resonance shape fit.
    
    Count(); // Get the last point
    
    cout<<"y4scan accepted "<<nbhabhas<<" Bhabha, "<<nhadrons<<" hadronic"<<endl<<endl;
    
    if (nbhabhas == 0 || nhadrons == 0) exit(0);
    
    TRho::Instance()->GetPersistenceManager()->Store();    // Make analysis objects persistent    
    
    return kTRUE;
}

//--------------------------------------------------------------------------

y4sfit::y4sfit(const char* const theName, const char* const theDescription) : 
TModule(theName, theDescription)
{
}

y4sfit::~y4sfit() { }

Bool_t y4sfit::EndJob(TEventManager*)
{
    // At the end, perform a Y(4S) resonance shape fit.
    
    // Acquire data points and errors
    
    cout << endl << "Fit points:" << endl;
    
    Float_t xx[MAXRUN], yy[MAXRUN], dx[MAXRUN], dy[MAXRUN];
    Int_t good = 0;
    
    Int_t i;
    for (i=0; i<nruns; i++) {
	
	if (b[i] <= 0.0) continue;
	if (h[i] <= 0.0) continue;
	
	Float_t rate = 10. * h[i] / b[i]; // Times arbitrary scale factor
	Float_t err = rate * TMath::Sqrt(1./h[i] + 1./b[i]);
	cout << good << '\t' << ecms[i] << '\t' << rate << "+-" << err << endl;
	
	xx[good] = ecms[i];
	yy[good] = rate;
	dx[i] = 0.001;
	dy[i] = err;
	good++;
    }
    
    // Show a canvas with the results of the selection
    
    R__EXTERN TStyle  *gStyle;
    gStyle->SetOptFit();    // Show the fit parameters
    
    TCanvas *c = new TCanvas("c", "Y4S Fit Program", 640, 480);
    TGraphErrors *sigmas = new TGraphErrors(good,xx,yy,dx,dy);
    sigmas->SetTitle("BABAR Y(4S) Scan (preliminary)");
    
    for (i=0;i<good;i++) {
	sigmas->SetPoint(i,xx[i],yy[i]);
	sigmas->SetPointError(i,dx[i],dy[i]);
    }
    
    sigmas->Draw("AP");
    c->Update();
    
    // Now, perform the Y4S fit
    
    cout << endl << "Fit results:" << endl;
    
    Y4S y4s;
    
    TF1 *func = new TF1("sigma",Y4S::resofu,10.50,10.70,5);
    
    func->SetParameters(190.0,0.58,0.01,0.005,1.5);
    func->SetParNames("Conti","m-10","Gamma","EcmsSpread","Bee*10^5");
    func->SetParLimits(1,0.1,1.0);
    func->SetParLimits(2,0.001,0.1);
    func->SetParLimits(3,0.005,0.005); // Fixed
    func->SetParLimits(4,0.5,2.5);
    
    sigmas->Fit("sigma");
    
    c->Update();
    
    return kTRUE;
}

//--------------------------------------------------------------------------


// This little main program drives the Y(4S) analysis

Int_t y4s(int argc=0,char* argv[]=0)
{
    // Instantiate an application manager
    TRho PAF("Y(4S) fitting example");
    
    // Authorize this client to access the bochum server at SLAC
    PAF.Authorize();
    
    // Instantiate the analysis 
    TAnalysis *analysis = new TAnalysis(argc,argv);
    
    // Set up parameter manager
    TParameterManager *parMgr = PAF.RegisterService(new TParameterManager);
    
    // Define the range of runs to process
    // Parameter presets can be overriden from file or command line
    
    parMgr->SetStringParm("cluster","system");
    parMgr->SetIntParm("start",5866);	// The scan runs
    parMgr->SetIntParm("end",5937);
    parMgr->SetStringParm("tagcut","_tag._nTracks>1");
    parMgr->SetBoolParm("tag",kTRUE,"Process TAG data");
    parMgr->SetBoolParm("aod",kFALSE,"Process AOD data");
    parMgr->SetBoolParm("mct",kFALSE,"Process MCT data");
    parMgr->SetParm("RootConditionsFile","$BFROOT/kanga/CondDB/BaBarConditions.root");
    
    // Add modules and start the event processing
    
    analysis->Add(new y4scan("y4scan","Y4S scan"));	// Add the Y4S scan module
    analysis->Add(new y4sfit("y4sfit","Y4S fit"));	// Add the Y4S fit module
    
    PAF.RegisterService(analysis);			// The sample analysis
    PAF.RegisterService(new TPersistenceManager()); // Save the histograms
    
    cout << PAF;
    
    TApplication theApp("App", &argc, argv);
    
    PAF.GetAnalysis()->Run(); // Execute the analysis
    
    // Here we don't return from the eventloop. "Exit ROOT" will quit the app.
    theApp.Run();
    
    return 0;
}
