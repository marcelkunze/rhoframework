// Filter macro for dimuon events in the PAF
// see: http://www.slac.stanford.edu/~mazzoni/babar/filters.html
// Author: Marcel Kunze, Bochum University, June '99
// Mods:   Add Kanga file support, MK, RUB, Nov.99
//
// Parameters which can be modified (on the command line):
// 
// Boolean
// -------
// -            tag                      true	(Process TAG data)
// -          babar                      true	(Read TAG data a la BaBar)
// -            aod                      true	(Process AOD data)
// -            mct                     false	(Process MCT data)
// -        verbose                     false	(Dump events)
// 
// Integer
// -------
// -            nev                   1000000	(Number of events)
// -          nchar                         2	(Number of prongs)
// -        totchar                        10	(Total charge)
// -           ndch                        20	(Number of hits in the DCH)
// 
// Double
// ------
// -           dphi                        11	(Alignment of tracks in phi)
// -         dcosth                       0.1	(Alignment of tracks in theta)
// -         ppelow                        -1	(p1Mag + p2Mag - ECMS)
// -        ppehigh                       0.5	(p1Mag + p2Mag - ECMS)
// -           ecut                         1	(Cut on p12EmcCandEnergy)
// -           etot                         0	(Total energy)
// -          ptmax                       0.5	(Max. Pt)
// -           absz                         3	(Distance of track in z)
// -           doca                         2	(Distance of closest approach)
// -             pt                         0	(Perp. momentum)
// -           emin                      0.05	(Energy deposit in the EMC)
// -           emax                       0.3	(Energy deposit in the EMC)
// 
// String
// ------
// -           file              test/run7024	(Name of input file)
// -     collection                     	(Read a collection)
//

#ifndef __CINT__
#include "TMath.h"
#include "TStopwatch.h"
#include "RhoBase/TRho.h"
#include "RhoBase/TCandidate.h"
#include "RhoBase/TCandList.h"
#include "RhoManager/TEventManager.h"
#include "RhoManager/TParameterManager.h"
#include "RhoManager/TEventCollection.h"
#include "RhoConditions/TConditions.h"
#include "RhoConditions/TBeams.h"
#include "RhoBase/VAbsTag.h"

#include <iostream>
using namespace std;

Int_t dimuon(int argc, char**argv);

Int_t main(int argc, char**argv) { return dimuon(argc, argv); }

#endif

Int_t dimuon(int argc=0,char* argv[]=0)
{
#ifdef __CINT__
    Double_t const  kPI        = 3.14159265358979323846;
    gROOT->Macro("$PAF/PAFMacros/LoadLibs.C"); // Load PAF
#endif

    TRho PAF("Dimuon filter");		// Instantiate a named framework
    cout<<PAF;

    // Authorize this client to access the bochum server at SLAC
    PAF.Authorize();

    // Set up parameter manager
    TParameterManager *parMgr = PAF.RegisterService(new TParameterManager);

    // Define parameters (Use typed version for CINT)

    parMgr->SetStringParm("file","test/run7024","Name of input file");
    parMgr->SetIntParm("nev",1000000,"Number of events");
    parMgr->SetBoolParm("tag",kTRUE,"Process TAG data");
    parMgr->SetBoolParm("babar",kTRUE,"Read TAG data a la BaBar");
    parMgr->SetBoolParm("aod",kTRUE,"Process AOD data");
    parMgr->SetBoolParm("mct",kFALSE,"Process MCT data");
    parMgr->SetStringParm("collection","","Read a collection");
    parMgr->SetBoolParm("verbose",kFALSE,"Dump events");

    // Define rough cuts to be applied at the TAG level
    // see: http://babar-hn.slac.stanford.edu:5090/HyperNews/get/physAnal/223.html

    parMgr->SetDoubleParm("dphi",11.0,"Alignment of tracks in phi");
    parMgr->SetDoubleParm("dcosth",0.1,"Alignment of tracks in theta");
    parMgr->SetDoubleParm("ppelow",-1.0,"p1Mag + p2Mag - ECMS");
    parMgr->SetDoubleParm("ppehigh",0.5,"p1Mag + p2Mag - ECMS");
    parMgr->SetDoubleParm("ecut",1.0,"Cut on p12EmcCandEnergy");

    // Define more refined cuts to be applied at the Micro level
    // see: http://babar-hn.slac.stanford.edu:5090/HyperNews/get/TauQED/23/2.html

    parMgr->SetDoubleParm("etot",0.0,"Total energy");
    parMgr->SetIntParm("nchar",2,"Number of prongs");
    parMgr->SetDoubleParm("ptmax",0.5,"Max. Pt");
    parMgr->SetIntParm("totchar",10,"Total charge");
    parMgr->SetDoubleParm("absz",3.0,"Distance of track in z");
    parMgr->SetDoubleParm("doca",2.0,"Distance of closest approach");
    parMgr->SetDoubleParm("ndch",20,"Number of hits in the DCH");
    parMgr->SetParm("pt",0.0,"Perp. momentum");
    parMgr->SetDoubleParm("emin",0.05,"Energy deposit in the EMC");
    parMgr->SetDoubleParm("emax",0.3,"Energy deposit in the EMC");

    parMgr->ScanCommandLine(argc,argv);
    parMgr->PrintAllParm();

    // Cache the parameters in order to gain speed

    Double_t DPHI    = parMgr->GetDouble("dphi");
    Double_t DCOSTH  = parMgr->GetDouble("dcosth");
    Double_t PPELOW  = parMgr->GetDouble("ppelow");
    Double_t PPEHIGH = parMgr->GetDouble("ppehigh");
    Double_t ECUT    = parMgr->GetDouble("ecut");

    Double_t ETOT    = parMgr->GetDouble("etot");
    Int_t    NCHAR   = parMgr->GetInt("nchar");
    Double_t PTMAX   = parMgr->GetDouble("ptmax");
    Int_t    TOTCHAR = parMgr->GetInt("totchar");
    Double_t ABSZ    = parMgr->GetDouble("absz");
    Double_t DOCA    = parMgr->GetDouble("doca");
    Int_t    NDCH    = parMgr->GetInt("ndch");
    Double_t PT      = parMgr->GetDouble("pt");
    Double_t EMIN    = parMgr->GetDouble("emin");
    Double_t EMAX    = parMgr->GetDouble("emax");
    
    // Allocate the collection for accepted events

    TEventCollection *collection = 0;	// Collect the interesting events

    // Initialise a conditions database

    TConditions *theConditions = PAF.RegisterService(new TConditions);

    // Initialise reading of the event database and activate branches
    // Add the datafiles to be processed
    
    TEventManager* eventmgr = PAF.RegisterService(new TEventManager());

    // Define the event input
    // Run either from a single file or from an event collection
    if (parMgr->GetString("collection")=="") {
	eventmgr->InitRead(parMgr->GetString("file"));
	collection = new TEventCollection("dimuons.root");
    }
    else
	eventmgr->UseCollection(parMgr->GetString("collection"));
	
    TCandList* trackList = new TCandList();		// This will hold the selected candidates
    
    TStopwatch timer;				// Measure the execution time
    timer.Start();
    
    //____________________________________________________________________________
        
    Int_t counter=0, acceptedTag=0, acceptedAod=0;
    
    while ( eventmgr->NextIter() ) {
	
	UInt_t rNumber = eventmgr->GetRunNumber();
	UInt_t eNumber = eventmgr->GetEventNumber();
	if (eNumber > parMgr->GetInt("nev")) break;
	
	++counter;

	// Get the beam info

        TRhoTime theTime(eventmgr->GetTime());
	theConditions->At(theTime);		    // Read the beam conditions DB
	TBeams *beams = theConditions->GetBeams();
	Double_t ecms = beams->EnergyCM();
	TVector3 boost = beams->BoostCMtoLab();

	
	// Make a crude decision to accept the event based on the TAG
	// (Read the AOD for acceptable events only to speed up)
	
	if (parMgr->GetBool("tag")) {
	    VAbsTag &tag = eventmgr->GetTag();	// Shortcut to access TAG
	    Float_t p1Phi, p2Phi, p1CosTheta, p2CosTheta, p1Mag, p2Mag, 
		p1EmcCandEnergy, p2EmcCandEnergy;
	    
	    if (parMgr->GetBool("babar")) {
	      tag.getFloat(p1Phi,"p1Phi");
	      tag.getFloat(p2Phi,"p2Phi");
	      tag.getFloat(p1CosTheta,"p1CosTheta"); 
	      tag.getFloat(p2CosTheta,"p2CosTheta"); 
	      tag.getFloat(p1Mag,"p1Mag"); 
	      tag.getFloat(p2Mag,"p2Mag"); 
	      tag.getFloat(p1EmcCandEnergy,"p1EmcCandEnergy"); 
	      tag.getFloat(p2EmcCandEnergy,"p2EmcCandEnergy"); 
	    }
	    else {
	      p1CosTheta = (Float_t) tag.GetPrimaryTrackMomentum().X();
	      p2CosTheta = (Float_t) tag.GetSecondaryTrackMomentum().X();
	      p1Phi = (Float_t) tag.GetPrimaryTrackMomentum().Y();
	      p2Phi = (Float_t) tag.GetSecondaryTrackMomentum().Y();
	      p1Mag = (Float_t) tag.GetPrimaryTrackMomentum().Z();
	      p2Mag = (Float_t) tag.GetSecondaryTrackMomentum().Z();
	      p1EmcCandEnergy = tag.GetPrimaryTrackShowerEnergy();
	      p2EmcCandEnergy = tag.GetSecondaryTrackShowerEnergy();
	    }

	    // Preselect the interesting events 
	    // according to energy, mass and angular constraints
	    
	    if (tag.GetNumberOfGoodTracksLoose() < NCHAR) continue;
	    
	    // Preselect the interesting events in CMS 
	    // according to angular constraints
	    
	    Double_t dcosth = TMath::Abs(p1CosTheta + p2CosTheta);	    // Ideally cancels to zero
	    if (dcosth > DCOSTH) continue;			    // Does it match ?
	    
	    Double_t dphi = TMath::Abs(p1Phi - p2Phi);
	    dphi = TMath::Abs(dphi - TMath::Pi());			    // Those are close to 180 deg.
	    if (dphi/TMath::Pi()*180. > DPHI) continue;			    // Does it match ?
	    
	    if (p1EmcCandEnergy >= ECUT  || p2EmcCandEnergy >= ECUT) continue; 
	    Float_t ppe = p1Mag + p2Mag - ecms;
	    if (ppe <= PPELOW  || ppe >= PPEHIGH) continue; 
	    
	    cout << "Run #" << rNumber;
	    cout << ": Accepted event TAG #" << eNumber << endl;
	    
	}
	
        acceptedTag++;
	
	// If the event survived the raw cuts, make a more detailed inspection
	// based on the Micro-DST
	
	if (parMgr->GetBool("aod")) {
	    
	    Float_t energy = 0.0;
	    Int_t nGoodTrks = 0, nCharge = 0;
	    Bool_t  highpt = kFALSE;
	    
	    trackList->SetNumberOfCandidates(0);	// Reset the candidate list
	    
	    Int_t n = eventmgr->GetNumberOfTracks();
	    for (Int_t i=0; i<n; i++) {
		
		VAbsMicroCandidate &aod = eventmgr->GetMicroCandidate(i);  // Use shortcut to access Micro
		
		// Look at EMC
		
		if (aod.GetEmcRawEnergy() > EMIN)		// Check energy deposit
		    energy += aod.GetEmcRawEnergy();		// Accumulate energy deposit
		
		Int_t charge = aod.GetCharge();
		if (charge == 0) continue;			// Skip neutrals
		
		// Apply tracking cuts (Select good tracks from the primary vertex)
		
		Double_t mom  = aod.GetMomentum().Perp();	// Get the transverse momentum
		Double_t zpos = fabs(aod.GetPosition().Z());	// Get the position along the beam axis
		Double_t dist = aod.GetPosition().Perp();	// Get distance of closest approach
		
		if (zpos >= ABSZ) continue;			// Check track z position
		if (dist >= DOCA) continue;			// and distance of closest approach
		if (aod.GetDchHits() <= NDCH) continue;		// Check number of hits
		if (mom <= PT) continue;			// Check track momentum
		if (aod.GetEmcRawEnergy() < EMIN) continue;     // Is the track matched ?
		if (aod.GetEmcRawEnergy() >= EMAX) continue;    // Is the track matched ?
		
		// Prepare quantities for the final selection
		
		if (mom > PTMAX) highpt = kTRUE;			// Check track momentum
		
		nGoodTrks++;					// Count the good tracks
		nCharge += charge;				// Accumulate charge
		
		if (parMgr->GetBool("verbose")) {
		    cout << "Accepted track #" << i+1;
		    cout << aod;
		}
		
		// Put the candidate into a list
		
		TLorentzVector v = aod.GetLorentzVector();
		trackList->Put(TCandidate(v,charge));
		
		// The following also works (but is slower)
		//		PAFMicroCandidate cand = eventmgr->GetMicroCandidate(i);
		//		trackList->Put(TCandidate(cand,i));
		
	    }
	    
	    // Make a final decision on number of tracks, charge and energy balance
	    
	    if (energy <= ETOT) continue;			// Require minimum energy deposit
	    if (nGoodTrks != NCHAR) continue;			// Require NCHAR tracks
	    if (!highpt) continue;				// Require Pt > PTMAX
	    if (abs(nCharge) >= TOTCHAR) continue;		// Require charge balance
	    
	    // Are the two tracks back-to-back in the CMS (in phi) ?
	    // Note, there is also a very small perpendicular boost due to the beam crossing
	    
	    trackList->Boost(-1.0*boost);			  // Transform into CMS
	    TCandidate *c1 = trackList->Get(0);
	    TCandidate *c2 = trackList->Get(1);
	    Double_t phi1 = c1->P3().Phi();			  // Transform to deg.
	    Double_t phi2 = c2->P3().Phi();
	    Double_t dphi = TMath::Abs(phi1 - phi2);
	    dphi = TMath::Abs(dphi - TMath::Pi());			  // Those are close to 180 deg.
	    if (dphi/TMath::Pi()*180. > DPHI) continue;			  // Does it match ?
	    
	    acceptedAod++;
	    cout << "Run #" << rNumber;
	    cout << ": Accepted event AOD #" << eNumber;
	    const TRhoTime &time = eventmgr->GetTime();
	    cout << " at " << time << "(GMT)" << endl;
	    cout << "Properties:";
	    cout << "  Tracks=" << nGoodTrks;
	    cout << ", Charge=" << nCharge;
	    cout << ", Energy=" << energy << " GeV" << endl;
	    
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

