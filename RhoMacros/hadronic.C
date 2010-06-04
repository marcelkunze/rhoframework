// Filter macro for hadronic events in the Rho framework
// see: http://www.slac.stanford.edu/~mazzoni/babar/filters.html
//
// Sample spectra are produced for E/P and neutrals for accepted events
//
// Author: Marcel Kunze, Bochum University, June '99
//
// Parameters which can be modified (on the command line):
// 
// Boolean
// -------
// -            tag                      true	(Process TAG data)
// -          babar                     false	(Read TAG data a la BaBar)
// -            aod                      true	(Process AOD data)
// -            mct                     false	(Process MCT data)
// -        verbose                     false	(Dump events)
// -             qc                     false	(Fill selector QC spectra)
// 
// Integer
// -------
// -            nev                   1000000	(Number of events)
// -          start                      5000	(First run)
// -            end                         0	(Last run)
// -       nchartag                        -1	(Total charge)
// -          nchar                         2	(Number of prongs)
// -        totchar                         4	(Total charge)
// -           ndch                        20	(Number of hits in the DCH)
// -           bins                       200	(Binning factor for histos)
// 
// Double
// ------
// -      etotalmin                         3	(Minimum total energy)
// -      etotalmax                    1e+008	(Maximum total energy)
// -     ptotalmass                         2	(Total mass)
// -    ptotalcosth                         0	(Total direction)
// -  eneutralcosth                         0	(Total direction)
// -            xpv                        10	(Cut on x position)
// -            zpv                        10	(Cut on z position)
// -           etot                       1.5	(Total energy)
// -           pmin                      0.05	(Min. momentum)
// -           pmax                        10	(Max. momentum)
// -         highpt                       0.5	(Max. perp. momentum)
// -           emin                       0.1	(Energy deposit in the EMC)
// -           absz                        10	(Distance of track in z)
// -           doca                       1.5	(Distance of closest approach)
// -             pt                       0.1	(Perp. momentum)
// -          d0min                         1	(Distance of outer tracks)
// -           dist                         1	(Distance of closest approach)
// -          vtxip                       0.2	(Angle of KS direction wrt interaction point)
// -          pkmin                         1	(Min. momentum)
// -         eopcut                       0.8	(E over P)
// -         egamma                       0.8	(Min. gamma energy)
// -         epimin                       0.8	(Min. pi0 energy)
// 
// String
// ------
// -           file                          	(Name of input file)
// -        cluster                      system	(Database cluster)
// -     collection                hadrons.root	(Read a collection)
//

#include <math.h>
#include <stdlib.h>

// We have to ommit these lines if the program is executed as a macro

#ifndef __CINT__
#include "TStopwatch.h"
#include "RhoBase/TRho.h"
#include "RhoBase/TCandList.h"
#include "RhoBase/TCandListIterator.h"
#include "RhoBase/TCandidate.h"
#include "RhoManager/TEventManager.h"
#include "RhoManager/TCandListManager.h"
#include "RhoManager/TPersistenceManager.h"
#include "RhoManager/TParameterManager.h"
#include "RhoManager/TEventCollection.h"
#include "RhoManager/TRunCollection.h"
#include "RhoManager/TRunDB.h"
#include "RhoConditions/TConditions.h"
#include "RhoConditions/TBeams.h"
#include "RhoBase/VAbsTag.h"
#include "RhoSelector/TPidSelector.h"

#include <iostream>
using namespace std;

Int_t hadronic(int argc, char**argv);

Int_t main(int argc, char**argv) { return hadronic(argc,argv); }

#endif

Int_t hadronic(int argc=0,char* argv[]=0)
{
#ifdef __CINT__
    gROOT->Macro("$RHO/RhoMacros/LoadLibs.C"); // Load Rho
#endif

    TRho Rho("Hadronic filter");                   // Instantiate a named framework
    Rho.PrintOn();

    // Authorize this client to access the bochum server at SLAC
    Rho.Authorize();

    // Get access to the conditions database

    TConditions *theConditions = Rho.RegisterService(new TConditions);

    // Set up parameter manager
    TParameterManager *parMgr = Rho.RegisterService(new TParameterManager);

    // Define parameters (Use typed version for CINT)

    parMgr->SetParm("file","","Name of input file");
    parMgr->SetIntParm("nev",1000000,"Number of events");
    parMgr->SetIntParm("start",5000,"First run");
    parMgr->SetIntParm("end",0,"Last run");
    parMgr->SetBoolParm("tag",kTRUE,"Process TAG data");
    parMgr->SetBoolParm("babar",kFALSE,"Read TAG data a la BaBar");
    parMgr->SetBoolParm("aod",kTRUE,"Process AOD data");
    parMgr->SetBoolParm("mct",kFALSE,"Process MCT data");
    parMgr->SetStringParm("cluster","system","Database cluster");
    parMgr->SetStringParm("collection","","Read a collection");
    parMgr->SetBoolParm("verbose",kFALSE,"Dump events");
    parMgr->SetBoolParm("analyze",kTRUE,"Analyze events");
    parMgr->SetBoolParm("qc",kFALSE,"Fill selector QC spectra");

    // Define rough cuts to be applied at the TAG level
    // see http://babar-hn.slac.stanford.edu:5090/HyperNews/get/physAnal/223.html
    //     http://babar-hn.slac.stanford.edu:5090/HyperNews/get/physAnal/261.html
    //     http://babar-hn.slac.stanford.edu:5090/HyperNews/get/physAnal/259.html

    parMgr->SetDoubleParm("etotalmin",3.0,"Minimum total energy");
    parMgr->SetDoubleParm("etotalmax",1.E8,"Maximum total energy");
    parMgr->SetDoubleParm("ptotalmass",2.0,"Total mass");
    parMgr->SetDoubleParm("ptotalcosth",0.0,"Total direction");
    parMgr->SetDoubleParm("eneutralcosth",0.0,"Total direction");
    parMgr->SetDoubleParm("xpv",10.0,"Cut on x position");
    parMgr->SetDoubleParm("zpv",10.0,"Cut on z position");
    parMgr->SetIntParm("nchartag",-1,"Total charge");

    // Define more refined cuts to be applied at the Micro level ("Rome selection")
    // see http://babar-hn.slac.stanford.edu:5090/HyperNews/get/physAnal/216.html

    parMgr->SetDoubleParm("etot",1.5,"Total energy");
    parMgr->SetDoubleParm("nchar",2,"Number of prongs");
    parMgr->SetDoubleParm("pmin",0.05,"Min. momentum");
    parMgr->SetDoubleParm("pmax",10.0,"Max. momentum");
    parMgr->SetDoubleParm("highpt",0.5,"Max. perp. momentum");
    parMgr->SetDoubleParm("emin",0.1,"Energy deposit in the EMC");
    parMgr->SetIntParm("totchar",4,"Total charge");

    // see http://babar-hn.slac.stanford.edu:5090/HyperNews/get/physAnal/260.html

    parMgr->SetDoubleParm("absz",10.0,"Distance of track in z");
    parMgr->SetDoubleParm("doca",1.5,"Distance of closest approach");
    parMgr->SetDoubleParm("ndch",20,"Number of hits in the DCH");
    parMgr->SetDoubleParm("pt",0.1,"Perp. momentum");

    // Define cuts for the analysis
    // see http://babar-hn.slac.stanford.edu:5090/HyperNews/get/physAnal/247.html

    // For K0s (BABAR physics book chapter 4.6):
    // Vertex two outer tracks > D0MIN within DIST and opening cone <VTXIP
    // The pions are identified with an E over P cut

    parMgr->SetDoubleParm("d0min",1.0,"Distance of outer tracks");
    parMgr->SetDoubleParm("dist",1.0,"Distance of closest approach");
    parMgr->SetDoubleParm("vtxip",0.2,"Angle of KS direction wrt interaction point");
    parMgr->SetDoubleParm("pkmin",1.0,"Min. momentum");
    parMgr->SetDoubleParm("eopcut",0.8,"E over P");

    // For pi0:

    parMgr->SetDoubleParm("egamma",0.8,"Min. gamma energy");
    parMgr->SetDoubleParm("epimin",0.8,"Min. pi0 energy");

    // Histogram parameters

    parMgr->SetIntParm("bins",200,"Binning factor for histos");

    // Override parms from the  command line

    parMgr->ScanCommandLine(argc,argv);
    parMgr->PrintAllParm();

    // Cache the parameters in order to gain speed

    Double_t ETOTALMIN     = parMgr->GetDouble("etotalmin");
    Double_t ETOTALMAX     = parMgr->GetDouble("etotalmax");
    Double_t PTOTALMASS    = parMgr->GetDouble("ptotalmass");
    Double_t PTOTALCOSTH   = parMgr->GetDouble("ptotalcosth");
    Double_t ENEUTRALCOSTH = parMgr->GetDouble("eneutralcosth");
    Double_t XPV           = parMgr->GetDouble("xpv");
    Double_t ZPV           = parMgr->GetDouble("zpv");
    Int_t    NCHARTAG      = parMgr->GetInt("nchartag");

    Double_t ETOT    = parMgr->GetDouble("etot");
    Int_t    NCHAR   = parMgr->GetInt("nchar");
    Double_t PMIN    = parMgr->GetDouble("pmin");
    Double_t PMAX    = parMgr->GetDouble("pmax");
    Double_t HIGHPT  = parMgr->GetDouble("highpt");
    Double_t EMIN    = parMgr->GetDouble("emin");
    Int_t    TOTCHAR = parMgr->GetInt("totchar");
    
    Double_t ABSZMAX     = parMgr->GetDouble("absz");
    Double_t DOCAMAX     = parMgr->GetDouble("doca");
    Int_t    NHITSDCHMIN = parMgr->GetInt("ndch");
    Double_t PTMIN       = parMgr->GetDouble("pt");

    Double_t D0MIN   = parMgr->GetDouble("d0min");
    Double_t DIST    = parMgr->GetDouble("dist");
    Double_t VTXIP   = parMgr->GetDouble("vtxip");
    Double_t PKMIN   = parMgr->GetDouble("pkmin");
    Double_t EOPCUT  = parMgr->GetDouble("eopcut");
    Double_t EGAMMA  = parMgr->GetDouble("egamma");
    Double_t EPIMIN  = parMgr->GetDouble("epimin");

    Int_t    BINS = parMgr->GetInt("bins");

     // Declare histograms and persist them
    // (Adding them explicitely yields reasonable names in the browser)

    // The persistence manager will take care of object conservation

    TPersistenceManager* persmgr = Rho.RegisterService(new TPersistenceManager());

    persmgr->SetDir("1D-Spectra");
    
    THistogram* xVertex  = persmgr->Histogram("Primary vertex x",BINS,-0.5,0.5);
    THistogram* yVertex  = persmgr->Histogram("Primary vertex y",BINS,-0.5,0.5);
    THistogram* zVertex  = persmgr->Histogram("Primary vertex z",BINS,-5.0,5.0);
    
    THistogram* thrust   = persmgr->Histogram("Thrust",BINS,0.5,1.0);
    THistogram* fwmom    = persmgr->Histogram("FW moment H2/H0",BINS,0.0,1.0);
    
    TEnergyHistogram* eTotalSpectrum    = persmgr->EnergyHistogram("eTotalSpectrum","Total energy of all candidates",BINS,0,50);
    TEnergyHistogram* eTotalSpectrumTag = persmgr->EnergyHistogram("eTotalSpectrumTag","Total energy of all candidates, selected by TAG",BINS,0,50);
    TEnergyHistogram* eTotalSpectrumAod = persmgr->EnergyHistogram("eTotalSpectrumAod","Total energy of all candidates, selected by AOD",BINS,0,50);
    
    persmgr->SetDir("2D-Spectra");
    TEoverPHistogram* svtdedx   = persmgr->EoverPHistogram("svtdedx","SVT dE/dx over P for all Candidates",200,0,4.5,200,0,20.);
    TEoverPHistogram* dchdedx   = persmgr->EoverPHistogram("dchdedx","DCH dE/dx over P for all Candidates",200,0,4.5,200,0,1000.);
    TEoverPHistogram* drcthetac = persmgr->EoverPHistogram("drcthetac","DRC thetaC over P for all Candidates",200,0,4.5,200,0,1.0);
    TEoverPHistogram* eoverp    = persmgr->EoverPHistogram("eoverp","E over P for all Candidates",200,0,4.5,200,0,4.5);
    
    // Set up ntuple
    
    persmgr->SetDir("Ntuples");
    TTuple* tuple = persmgr->Ntuple("PID");
    
    // Declare selectors
    
    TPidChargedSelector* chargedSelector  = new TPidChargedSelector(); 
    TPidNeutralSelector* neutralSelector  = new TPidNeutralSelector(); 

    // Initialize the list manager

    TCandListManager* listmgr = Rho.RegisterService(new TCandListManager());
    TCandList *trackList = listmgr->Add(new TCandList("tracks"));
    TCandList *chargedList = listmgr->Add(new TCandList("charged"));
    TCandList *neutralList = listmgr->Add(new TCandList("neutral"));

    // Initialise reading of the event database and activate branches

    TEventManager* eventmgr = Rho.RegisterService(new TEventManager());

    // Collect the interesting events in an event collection
    TEventCollection *collection = 0;

    // Collect interesting runs in a run collection
    TRunCollection *runCollection = 0;

    // Define the input to the skim
    // There are three different data sources:

    // 1) Do we run on a collection of events ? 
    if (parMgr->GetString("collection")!="") 
    {
	eventmgr->UseCollection(parMgr->GetString("collection")); // Run on preselected events
    }

    // 2) Do we run on a single file ?
    else if (parMgr->GetString("file") != "")
    {
	eventmgr->InitRead(parMgr->GetString("file"));
	collection = new TEventCollection("hadrons.root");
    }

    // 3) Do we run on a collection of runs ?
    else
    {
	runCollection = new TRunCollection("Collection"); 
	TRunDB runDB; 
        runDB.SetCluster(parMgr->GetString("cluster"));
        runDB.AddRunsToCollection(runCollection,parMgr->GetInt("start"),parMgr->GetInt("end"));
	eventmgr->UseCollection(runCollection);
	collection = new TEventCollection("hadrons.root");
    }
    
    // Use the stopwatch to see how fast it goes
    
    TStopwatch timer;                           // Measure the execution time
    timer.Start();
    
    //____________________________________________________________________________
    
    Int_t counter=0, acceptedTag=0, acceptedAod=0;
    Float_t etotal = 0.0;
    
    while ( eventmgr->NextIter() ) {
	
	++counter;
	UInt_t rNumber = eventmgr->GetRunNumber();
	UInt_t eNumber = eventmgr->GetEventNumber();
	if (eNumber > parMgr->GetInt("nev")) break;
	
	
	// Make a crude decision to accept the event based on the TAG
	// (Read the AOD for acceptable events only to speed up)
	
	if (parMgr->GetBool("tag")) {
	    VAbsTag &tag = eventmgr->GetTag();       // Shortcut to access TAG
	    Int_t nGoodTrkLoose = 0;
	    Float_t xv,yv,zv,th,r2;
	    Float_t ptotalmass,ptotalcosth,eneutralcosth;
	    if (parMgr->GetBool("babar")) {
	      tag.getFloat(xv,"xPrimaryVtx");
	      tag.getFloat(yv,"yPrimaryVtx");
	      tag.getFloat(zv,"zPrimaryVtx");
	      tag.getFloat(th,"thrustMag");
	      tag.getFloat(r2,"R2");
	      tag.getFloat(etotal,"eTotal");
	      tag.getFloat(ptotalmass,"pTotalMass"); 
	      tag.getFloat(ptotalcosth,"pTotalCosTh"); 
	      tag.getFloat(eneutralcosth,"eNeutralCosTh");
	      tag.getInt(nGoodTrkLoose,"nGoodTrkLoose");
	    }
	    else {
	      TVector3 v = tag.GetPrimaryVertex();
	      xv = (Float_t) v.X();
	      yv = (Float_t) v.Y();
	      zv = (Float_t) v.Z();
	      th = (Float_t) tag.GetThrust().Z();
	      r2 = tag.GetR2();
	      etotal = tag.GetTotalEnergy();
	      ptotalmass = tag.GetTotalMass();
	      ptotalcosth = (Float_t) tag.GetTotalMomentum().X();
	      eneutralcosth = (Float_t) tag.GetTotalNeutralEnergy().X();
	      nGoodTrkLoose = tag.GetNumberOfGoodTracksLoose();
	    }

	    // Fill some control histograms
	    
	    if (xv != 0) xVertex->Accumulate(xv);               // Vertex histograms                                    // Primary vertex
	    if (yv != 0) yVertex->Accumulate(yv);
	    if (zv != 0) zVertex->Accumulate(zv);
	    
	    eTotalSpectrum->Accumulate(etotal);                       // Total energy spectrum
	    
	    // Preselect the interesting events 
	    // according to energy, vertex, tracks, mass and angular constraints
	    
	    if (etotal <= ETOTALMIN || etotal >= ETOTALMAX) continue;
	    if (fabs(xv) > XPV || fabs(zv) > ZPV) continue;
	    if (nGoodTrkLoose <= NCHARTAG) continue;
	    if (ptotalmass <= PTOTALMASS) continue;
	    if (ptotalcosth <= PTOTALCOSTH && eneutralcosth <= ENEUTRALCOSTH) continue; 
	    
	    eTotalSpectrumTag->Accumulate(etotal);
	    
	    thrust->Accumulate(th);                             // Shape variables
	    fwmom->Accumulate(r2);
	    
	    if (parMgr->GetBool("verbose")) {
		cout << "Run #" << rNumber << endl;
		cout << ": Accepted event TAG #" << eNumber << endl;
	    }
	    
	}
	
	acceptedTag++;
	
	// If the event survived the raw cuts, make a more detailed inspection
	// based on the Micro-DST ("Rome selection")
	
	if (parMgr->GetBool("aod")) {
	    
	    Int_t nGoodTrks = 0, nCharge = 0;
	    Bool_t  highpt = kFALSE;
	    
	    // Provide a charged and neutral track list

	    eventmgr->Fill(trackList);
	    chargedList->Select(*trackList,chargedSelector);
	    neutralList->Select(*trackList,neutralSelector);

	    // Calculate total neutral energy for candidates > EMIN
	    
	    Float_t energy = neutralList->GetTotalEnergy(EMIN);

	    // Iterate over all charged tracks ("Rome selection")

	    TCandListIterator iter(*chargedList);
	    TCandidate *c;

	    while (c = iter.Next()) {
		
		VAbsMicroCandidate &cand = c->GetMicroCandidate();

		Float_t mom  = cand.GetMomentum().Mag(); // Get the momentum
		Float_t svt  = cand.GetSvtMeanDEdx();    // Get the energy loss
		Float_t dch  = cand.GetDchMeanDEdx();    // Get the energy loss
		Float_t drc  = cand.GetDrcThetaC();      // Get the Cerenkov angle
		Float_t emc  = cand.GetEmcRawEnergy();   // Get the energy deposit in EMC
		Float_t ifr  = cand.GetIfrHitLayers();   // Get the ifr info
		Int_t charge = cand.GetCharge();         // Get electrical charge
		
		// Check the EMC signal
		if (emc > EMIN) energy += emc;          // Accumulate energy deposit
		
		// Prepare the dE/dx and E/P plot and PID nTuple
		
		svtdedx->Accumulate(mom,svt);
		dchdedx->Accumulate(mom,dch);
		drcthetac->Accumulate(mom,drc);
		eoverp->Accumulate(mom,emc);
		
		tuple->Column("mom",mom,0.0f,"DOUBLE");
		tuple->Column("svt",svt,0.0f,"DOUBLE");
		tuple->Column("dch",dch,0.0f,"DOUBLE");
		tuple->Column("drc",drc,0.0f,"DOUBLE");
		tuple->Column("emc",emc,0.0f,"DOUBLE");
		tuple->Column("ifr",ifr,0.0f,"DOUBLE");
		tuple->DumpData();
		
		// Set up quantities for tracking cuts
		
		if (mom<PMIN || mom>PMAX) continue;             // Momentum is too large
		
		TVector3 p = cand.GetPosition();                // Get the starting point
		ULong_t theEventTime = eventmgr->GetMajorID();
		theConditions->At(theEventTime);      // Read the conditions DB
		TBeams *beams = theConditions->GetBeams();
		p = p - beams->Interaction3Point();
		
		Double_t pt   = cand.GetMomentum().Perp();      // Get the transverse momentum
		Double_t zpos = fabs(p.Z());                    // Get the position along the beam axis
		Double_t dist = p.Perp();                       // Get distance of closest approach
		UInt_t   ndch = cand.GetDchHits();              // Get the number of hits
		
		// Now, select the good tracks
		// NB. This could as well be done more elegantly
		// with the TPicoGoodTrackSelector. I leave the
		// alternative implementation as your homework...
		
		if (ndch <= NHITSDCHMIN) continue;              // Check number of hits
		if (pt   < PTMIN)        continue;              // Check track momentum
		if (emc  < EMIN)         continue;              // Is the track matched ?
		
                // Is the track near to the primary vertex ?

                if (zpos>=ABSZMAX || dist>=DOCAMAX) continue;

		// Prepare quantities for the final selection
		
		if (pt > HIGHPT) highpt = kTRUE;                 // Check track momentum
		
		nGoodTrks++;                                    // Count the good tracks
		nCharge += charge;                              // Accumulate charge
		
		if (parMgr->GetBool("verbose")) {
		    cout << "Accepted track #" << iter.Index();
		    cout << cand << endl;
		}

	    }
	    
	    // Make a final decision on number of tracks, charge and energy balance
	    
	    if (energy <= ETOT) continue;                       // Require minimum energy deposit
	    if (nGoodTrks <= NCHAR) continue;                   // Require NCHAR tracks
	    if (!highpt) continue;                              // Require Pt > HIGHPT (at least one track)
	    if (abs(nCharge) >= TOTCHAR) continue;              // Require charge balance
	    
	    eTotalSpectrumAod->Accumulate(etotal);
	    
	    acceptedAod++;
	    
	    if (parMgr->GetBool("verbose")) {
		cout << endl << "Run #" << rNumber;
		cout << ": Accepted event AOD #" << eNumber << endl;
		cout << " at " << eventmgr->GetTime() << "(GMT)" << endl;
		cout << "Properties:";
		cout << "  Tracks=" << nGoodTrks;
		cout << ", Charge=" << nCharge;
		cout << ", Energy=" << energy << " GeV" << endl;
	    }
	    
	    // Note the event in the event collection
	    if (collection!=0) eventmgr->AddEventToCollection(collection);

	}
	
    }
    
    //____________________________________________________________________________

    timer.Stop();
    
    cout<<endl<<"FilterApplication: Processed "<<counter<<" events; accepted "<< acceptedTag << " TAGs, "<< acceptedAod << " AODs" <<endl<<endl;
    
    cout<<" ----- Realtime:   "<<timer.RealTime()<<"sec"<<endl;
    cout<<" ----- Cputime:    "<<timer.CpuTime()<<"sec"<<endl;
    cout<<" ----- Time/Event: "<<timer.CpuTime()/counter<<"sec"<<endl;
    cout<<" ----- Speed:      "<<counter/timer.CpuTime()<<"Hz"<<endl;   
    
    return 0;
}

