// Filter macro for two photon events in the PAF
// Example for use of an out-of-the-box event selector
//
// Author: Marcel Kunze, Bochum University, June '99
// 
// Parameters which can be modified (on the command line):
// 
// Boolean
// -------
// -            tag                      true	(Process TAG data)
// -          babar                     false	(Read TAG data a la BaBar)
// -            aod                     false	(Process AOD data)
// -            mct                     false	(Process MCT data)
// -        verbose                     false	(Dump events)
// 
// Integer
// -------
// -            nev                   1000000	(Number of events)
// 
// Double
// ------
// -          e1mag                         4	(Energy cut on primary particle)
// -          e2mag                         2	(Energy cut on secondary particle)
// -       thetalow                       2.8	(Min. angle between tracks)
// -      thetahigh                       3.5	(Max. angle between tracks)
// 
// String
// ------
// -           file  system/1999/6/r6096_6170	(Name of input file)
// -     collection                     	(Read a collection)
//

// We have to include the headers for compilation
// CINT knows everything from the ROOT dictionary

#ifndef __CINT__
#include "TStopwatch.h"
#include "RhoBase/TRho.h"
#include "RhoManager/TEventManager.h"
#include "RhoManager/TDirectoryManager.h"
#include "RhoManager/TParameterManager.h"
#include "RhoManager/TEventCollection.h"
#include "RhoSelector/TEventSelector.h"

#include <iostream>
using namespace std;

Int_t gammagamma(int argc, char**argv);
Int_t main(int argc, char**argv) { return gammagamma(argc, argv); }
#endif

Int_t gammagamma(int argc=0,char* argv[]=0)
{
#ifdef __CINT__
    gROOT->Macro("$RHO/RhoMacros/LoadLibs.C"); // Load PAF
#endif
        
    TRho PAF("Gamma Gamma filter");	// Instantiate a named framework
    cout<<PAF;

    // Authorize this client to access the bochum server at SLAC
    PAF.Authorize();

    TParameterManager parMgr;
    PAF.RegisterService(&parMgr);

    // Define the input

    //parMgr.SetParm("file","system/1999/6/r6096_6170","Name of input file");
    parMgr.SetParm("file","test/runjpsiks310","Name of input file");

    // Define parameters (Use typed version for CINT)

    parMgr.SetIntParm("nev",1000000,"Number of events");
    parMgr.SetBoolParm("tag",kTRUE,"Process TAG data");
    parMgr.SetBoolParm("babar",kFALSE,"Read TAG data a la BaBar");
    parMgr.SetBoolParm("aod",kTRUE,"Process AOD data");
    parMgr.SetBoolParm("mct",kFALSE,"Process MCT data");
    parMgr.SetStringParm("collection","","Read a collection");
    parMgr.SetBoolParm("verbose",kFALSE,"Dump events");

    // Define rough cuts to be applied at the TAG level
    // see: http://www.slac.stanford.edu/BFROOT/www/Physics/Forum/forum/phonemeetings/forum_15jun99/dataAccess.html

    parMgr.SetDoubleParm("e1mag",4.0,"Energy cut on primary particle");
    parMgr.SetDoubleParm("e2mag",2.0,"Energy cut on secondary particle");
    parMgr.SetDoubleParm("thetalow",2.8,"Min. angle between tracks");
    parMgr.SetDoubleParm("thetahigh",3.5,"Max. angle between tracks");

    parMgr.ScanCommandLine(argc,argv);
    parMgr.PrintAllParm();

    // Cache the parameters in order to gain speed

    Bool_t   USETAG    = parMgr.GetBool("tag");
    Bool_t   USEAOD    = parMgr.GetBool("aod");
    Bool_t   VERBOSE   = parMgr.GetBool("verbose");
    Int_t    MAXEVENT  = parMgr.GetInt("nev");

    Double_t E1MAG     = parMgr.GetDouble("e1mag");
    Double_t E2MAG     = parMgr.GetDouble("e2mag");
    Double_t THETALOW  = parMgr.GetDouble("thetalow");
    Double_t THETAHIGH = parMgr.GetDouble("thetahigh");
    
    // Allocate the collection for accepted events

    TEventCollection *collection = 0;	// Collect the interesting events

    // Initialise reading of the event database and activate branches
    // Add the datafiles to be processed
    
    TEventManager* 	eventmgr  = new TEventManager();

    // Define the event input
    // Run either from a single file or from an event collection
    if (parMgr.GetString("collection")=="") {
	eventmgr->InitRead(parMgr.GetString("file"));
	collection = new TEventCollection("gammagammas.root");
    }
    else
	eventmgr->UseCollection(parMgr.GetString("collection"));

    TGammaGammaTagSelector *gammagammaSelector;	    // Instantiate an event selector
    gammagammaSelector = new TGammaGammaTagSelector();
    gammagammaSelector->SetPrimaryEnergy(E1MAG);	    // Adjust settings
    gammagammaSelector->SetSecondaryEnergy(E2MAG);
    gammagammaSelector->SetTheta(THETALOW,THETAHIGH);
    cout << *gammagammaSelector;

    TStopwatch timer;					    // Measure the execution time
    timer.Start();
    
    //____________________________________________________________________________

    Int_t counter=0, acceptedTag=0, acceptedAod=0;
    
    while ( eventmgr->NextIter() ) {

	UInt_t rNumber = eventmgr->GetRunNumber();
	UInt_t eNumber = eventmgr->GetEventNumber();
	if (eNumber > MAXEVENT) break;
	if (VERBOSE) cout << *eventmgr; // Dump the event	
	++counter;
	
	// Make a crude decision to accept the event based on the TAG
	// (Read the AOD for acceptable events only to speed up)
	
	if (USETAG) {

	    if (!gammagammaSelector->Accept(*eventmgr)) continue; // Try the next event

	    cout << "Run #" << rNumber;
	    cout << ": Accepted event TAG #" << eNumber << endl;

	}
	
        acceptedTag++;
	
	// If the event survived the raw cuts, make a more detailed inspection
	// based on the Micro-DST
	
	if (USEAOD) {
	    
	    // This is your homework !!!
	    
	}

	if (collection != 0) eventmgr->AddEventToCollection(collection);	// Collect the event
	
    }
    
    //____________________________________________________________________________
    
    timer.Stop();

    delete collection;
    delete eventmgr;

    cout<<endl<<"FilterApplication: Processed "<<counter<<" events; accepted "<< acceptedTag << " TAGs, "<< acceptedAod << " AODs" <<endl<<endl;
    
    cout<<" ----- Realtime:   "<<timer.RealTime()<<"sec"<<endl;
    cout<<" ----- Cputime:    "<<timer.CpuTime()<<"sec"<<endl;
    cout<<" ----- Time/Event: "<<timer.CpuTime()/counter<<"sec"<<endl;
    cout<<" ----- Speed:      "<<counter/timer.CpuTime()<<"Hz"<<endl;   

    return 0;
}

