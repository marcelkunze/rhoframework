// Filter macro for twoprong events in the PAF based on the TAG
// see: http://www.slac.stanford.edu/~mazzoni/babar/filters.html
//
// Author: Marcel Kunze, Bochum University, June '99
// 
// Parameters which can be modified (on the command line):
// 
// Boolean
// -------
// -            tag                      true	(Process TAG data)
// -          babar                      true	(Read TAG data a la BaBar)
// -            aod                     false	(Process AOD data)
// -            mct                     false	(Process MCT data)
// -        verbose                     false	(Dump events)
// -           mumu                      true	(Select dimuon events)
// 
// Integer
// -------
// -            nev                   1000000	(Number of events)
// -          nchar                         2	(Number of prongs)
// 
// Double
// ------
// -           dphi                        11	(Alignment of tracks in phi)
// -         dcosth                       0.1	(Alignment of tracks in theta)
// -         ppelow                        -1	(p1Mag + p2Mag - ECMS)
// -        ppehigh                       0.5	(p1Mag + p2Mag - ECMS)
// -           ecut                         1	(Cut on p12EmcCandEnergy)
// 
// String
// ------
// -           file  system/1999/6/r6096_6170	(Name of input file)
// -     collection                     	(Read a collection)
//

#ifndef __CINT__
#include "TStopwatch.h"
#include "RhoBase/TRho.h"
#include "RhoManager/TEventManager.h"
#include "RhoManager/TDirectoryManager.h"
#include "RhoManager/TParameterManager.h"
#include "RhoManager/TEventCollection.h"
#include "RhoConditions/TConditions.h"
#include "RhoConditions/TBeams.h"
#include "RhoBase/VAbsTag.h"

#include <iostream>
using namespace std;

Int_t twoprong(int argc, char**argv);

Int_t main(int argc, char**argv) { return twoprong(argc, argv); }

#endif

Int_t twoprong(int argc=0,char* argv[]=0)
{
#ifdef __CINT__
    Double_t const kPI    = 3.14159265358979323846;
    gROOT->Macro("$PAF/PAFMacros/LoadLibs.C"); // Load PAF
#endif
    Double_t const raddeg = 180. / TMath::Pi();
        
    TRho PAF("Twoprong filter");	// Instantiate a named framework
    cout<<PAF;

    // Authorize this client to access the bochum server at SLAC
    PAF.Authorize();

    TParameterManager *parMgr = PAF.RegisterService(new TParameterManager);

    // Define the input

    parMgr->SetParm("file","test/runjpsiks310","Name of input file");

    // Define parameters (Use typed version for CINT)

    parMgr->SetIntParm("nev",1000000,"Number of events");
    parMgr->SetBoolParm("tag",kTRUE,"Process TAG data");
    parMgr->SetBoolParm("babar",kTRUE,"Read TAG data a la BaBar");
    parMgr->SetBoolParm("aod",kFALSE,"Process AOD data");
    parMgr->SetBoolParm("mct",kFALSE,"Process MCT data");
    parMgr->SetStringParm("collection","","Read a collection");
    parMgr->SetBoolParm("verbose",kFALSE,"Dump events");
    parMgr->SetBoolParm("mumu",kTRUE,"Select dimuon events");

    // Define rough cuts to be applied at the TAG level
    // see: http://babar-hn.slac.stanford.edu:5090/HyperNews/get/physAnal/223.html

    parMgr->SetIntParm("nchar",2,"Number of prongs");
    parMgr->SetDoubleParm("dphi",11.0,"Alignment of tracks in phi");
    parMgr->SetDoubleParm("dcosth",0.1,"Alignment of tracks in theta");
    parMgr->SetDoubleParm("ppelow",-1.0,"p1Mag + p2Mag - ECMS");
    parMgr->SetDoubleParm("ppehigh",0.5,"p1Mag + p2Mag - ECMS");
    parMgr->SetDoubleParm("ecut",1.0,"Cut on p12EmcCandEnergy");

    parMgr->ScanCommandLine(argc,argv);
    parMgr->PrintAllParm();

    // Cache the parameters in order to gain speed

    Bool_t   USETAG    = parMgr->GetBool("tag");
    Bool_t   USEAOD    = parMgr->GetBool("aod");
    Bool_t   VERBOSE   = parMgr->GetBool("verbose");
    Int_t    MAXEVENT  = parMgr->GetInt("nev");

    Int_t    NCHAR   = parMgr->GetInt("nchar");
    Double_t DPHI    = parMgr->GetDouble("dphi");
    Double_t DCOSTH  = parMgr->GetDouble("dcosth");
    Double_t PPELOW  = parMgr->GetDouble("ppelow");
    Double_t PPEHIGH = parMgr->GetDouble("ppehigh");
    Double_t ECUT    = parMgr->GetDouble("ecut");
    
    // Initialise a conditions database

    TConditions *theConditions = PAF.RegisterService(new TConditions);

    // Allocate the collection for accepted events

    TEventCollection *collection = 0;	// Collect the interesting events

    // Initialise reading of the event database and activate branches
    // Add the datafiles to be processed
    
    TEventManager* eventmgr = PAF.RegisterService(new TEventManager());

    // Define the event input
    // Run either from a single file or from an event collection
    if (parMgr->GetString("collection")=="") {
	eventmgr->InitRead(parMgr->GetString("file"));
	collection = new TEventCollection("twoprongs.root");
    }
    else
	eventmgr->UseCollection(parMgr->GetString("collection"));

    TStopwatch timer;				// Measure the execution time
    timer.Start();
    
    //____________________________________________________________________________
    
    Int_t counter=0, acceptedTag=0, acceptedAod=0;
    
    while ( eventmgr->NextIter() ) {
	
	UInt_t rNumber = eventmgr->GetRunNumber();
	UInt_t eNumber = eventmgr->GetEventNumber();
	if (eNumber > MAXEVENT) break;

	++counter;
		
	if (USETAG) {
	    VAbsTag &tag = eventmgr->GetTag();	// Shortcut to access TAG
	    Float_t p1Phi, p2Phi, p1CosTh, p2CosTh, p1Mag, p2Mag, 
		    p1EmcCandEnergy, p2EmcCandEnergy;

	    if (parMgr->GetBool("babar")) {
	      tag.getFloat(p1Phi,"p1Phi");
	      tag.getFloat(p2Phi,"p2Phi");
	      tag.getFloat(p1CosTh,"p1CosTheta"); 
	      tag.getFloat(p2CosTh,"p2CosTheta"); 
	      tag.getFloat(p1Mag,"p1Mag"); 
	      tag.getFloat(p2Mag,"p2Mag"); 
	      tag.getFloat(p1EmcCandEnergy,"p1EmcCandEnergy"); 
	      tag.getFloat(p2EmcCandEnergy,"p2EmcCandEnergy"); 
	    }
	    else {
	      TVector3 t1 = tag.GetPrimaryTrackMomentum();    // CosTheta, Phi, Magnitude
	      TVector3 t2 = tag.GetSecondaryTrackMomentum();  // CosTheta, Phi, Magnitude
	      p1CosTh = t1.X();
	      p2CosTh = t2.X();
	      p1Phi = t1.Y();
	      p2Phi = t2.Y();
	      p1Mag = t1.Z();
	      p2Mag = t2.Z();
	      p1EmcCandEnergy = tag.GetPrimaryTrackShowerEnergy();
	      p2EmcCandEnergy = tag.GetSecondaryTrackShowerEnergy();
	    }

	    // Preselect the interesting events 
	    // according to energy, mass and angular constraints

	    if (tag.GetNumberOfGoodTracksLoose() < NCHAR) continue;

	    // Preselect the interesting events in CMS 
	    // according to angular constraints

	    Double_t dcosth = TMath::Abs(p1CosTh + p2CosTh);	    // Ideally cancels to zero
	    if (dcosth > DCOSTH) continue;			    // Does it match ?

	    Double_t dphi = TMath::Abs(p1Phi - p2Phi);
	    dphi = TMath::Abs(dphi - TMath::Pi());			    // Those are close to 180 deg.
	    if (dphi*raddeg > DPHI) continue;			    // Does it match ?

	    // Select the event type

	    if (parMgr->GetBool("mumu")) {
              TRhoTime theTime(eventmgr->GetTime());
	      theConditions->At(theTime);		    // Read the beam conditions DB
	      TBeams *beams = theConditions->GetBeams();
	      Double_t ecms = beams->EnergyCM();
	      if (p1EmcCandEnergy >= ECUT  || p2EmcCandEnergy >= ECUT) continue; 
	      Float_t ppe = p1Mag + p2Mag - ecms;
	      if (ppe <= PPELOW  || ppe >= PPEHIGH) continue; 
	    }

	    cout << "Run #" << rNumber;
	    cout << ": Accepted event TAG #" << eNumber << endl;
	    acceptedTag++;

	    if (collection != 0) eventmgr->AddEventToCollection(collection);	// Collect the event

	}
		
    }
    
    //____________________________________________________________________________
    
    timer.Stop();

    delete collection;

    cout<<endl<<"FilterApplication: Processed "<<counter<<" events; accepted "<< acceptedTag << " TAGs, "<< acceptedAod << " AODs" <<endl<<endl;
    
    cout<<" ----- Realtime:   "<<timer.RealTime()<<"sec"<<endl;
    cout<<" ----- Cputime:    "<<timer.CpuTime()<<"sec"<<endl;
    cout<<" ----- Time/Event: "<<timer.CpuTime()/counter<<"sec"<<endl;
    cout<<" ----- Speed:      "<<counter/timer.CpuTime()<<"Hz"<<endl;   

    return 0;
}

