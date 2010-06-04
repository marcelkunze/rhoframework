//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TConditions	    							//
//                                                                      //
// Conditions database class						//
//                                                                      //
// Author: Marcel Kunze, RUB, Nov. 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "TString.h"
#include "TBtree.h"
#include "TTree.h"
#include "TKey.h"
#include "TNetFile.h"
#include "TAuthenticate.h"

#include "RhoConditions/TConditions.h"
#include "RhoConditions/TBeams.h"
#include "RhoConditions/TDchCond.h"
#include "RhoConditions/TSvtPidCalib.h"

ClassImp(TConditions)

#include <iostream>
using namespace std;

// Constructor

// The default location of the conditions database is expected
// at $TROOT/BaBarConditions.root
// Network access is supported via TNetFile

TConditions::TConditions(const char *db, const char *calibration, const char *bcalibration) :
fBeam(0),fPepEnergies(0),fPepBeamSpotCal(0),fPepFillPattern(0),fDchCond(0),fSvtPidCalib(0),
fCurrentTime(0), fReadBeams(kFALSE), fReadDchCond(kFALSE),
fReadPepEnergies(kFALSE), fReadPepBeamSpotCal(kFALSE), fReadPepFillPattern(kFALSE),
fReadPepBoostCal(kFALSE), fReadSvtPidCalib(kFALSE)
{
    TString baseDir = "";
    if (getenv("BFROOT")!=0) baseDir = TString(getenv("BFROOT"))+"/kanga";
    if (getenv("RHOROOT")!=0) baseDir = getenv("RHOROOT");
    
    if (baseDir.Index("root:")>=0) Authorize();

    TString name(db);

    // Special treatment of BFROOT
    if (name.Index("BFROOT")>=0) {
	if (getenv("BFROOT")) {
	    if (name.Index("$env(BFROOT)")>=0) name = getenv("BFROOT") + name(12,name.Length()-12);
	    if (name.Index("$BFROOT")>=0) name = getenv("BFROOT") + name(7,name.Length()-7);
	}
	else {
	    name = "CondDB/BaBarConditions.root";
	    cerr << "TConditions: BFROOT not set. Try to open " << name << endl;

	}
    }

    if (name == "") name = "CondDB/BaBarConditions.root";
    if (name.Data()[0] != '/' && name.Data()[0] != '$' && name.Data()[1] != ':') 
	name = baseDir + "/" + name;

    fFile = TFile::Open(name, "READ","BaBar Conditions Data",1);
    if (fFile == 0)
	cerr << "TConditions: Error opening conditions file " << name.Data() << endl;
    else
	cout << "TConditions: Opening conditions file " << name.Data() << endl;
    
    if (fFile != 0) {
	
	fBeam = new TBeams();

	// Browse the conditions file to see what containers it has, and create
	// appropriate proxies for each one
	TList *keys= fFile->GetListOfKeys();
	int nContainers= keys->GetSize()/2;
	for(int index= 0; index < nContainers; index++) {
	    TKey *key= (TKey*)(keys->At(2*index+1));
	    const char *className= key->GetTitle();
	    TString keyName = key->GetName();
	    if(strcmp(className,"PepBeamsR") == 0) {
		// Setup the PepBeams interface
		
		fBeamIndex= dynamic_cast<TBtree*> (fFile->Get("PepBeamsRIndex"));
		fBeamTree= dynamic_cast<TTree*> (fFile->Get("PepBeamsR"));
		if (fBeamTree != 0) {
		    fBeamTree->SetBranchAddress("data", &fBeam);
		    cout << "TConditions: Setting TTree for " << className << endl;
		    fReadBeams = kTRUE;
		}
		else {
		    cerr << "TConditions: Error setting TTree for " << className << endl;
		    fReadBeams = kFALSE;
		}
	    }
	    else if(strcmp(className, "PepEnergiesR") == 0) {
		// Setup the PepEnergiesR interface
		
		fPepEnergies = new PepEnergiesR();
		fPepEnergiesIndex= dynamic_cast<TBtree*> (fFile->Get("PepEnergies_PEPIIIndex"));
		fPepEnergiesTree= dynamic_cast<TTree*> (fFile->Get("PepEnergies_PEPII"));
		if (fPepEnergiesTree != 0) {
		    fPepEnergiesTree->SetBranchAddress("data", &fPepEnergies);
		    cout << "TConditions: Setting TTree for " << className << endl;
		    fReadPepEnergies = kTRUE;
		}
		else {
		    cerr << "TConditions: Error setting TTree for " << className << endl;
		    fReadPepEnergies = kFALSE;
		}
	    }
	    else if(strcmp(className, "PepBeamSpotCalR") == 0) {
	      TString calName = TString("PepBeamSpotCal_") + TString(calibration);
	      if (keyName == calName) {
		fPepBeamSpotCal = new PepBeamSpotCalR();
		TString index = calName + "Index";
		fPepBeamSpotCalIndex= dynamic_cast<TBtree*> (fFile->Get(index));
		fPepBeamSpotCalTree= dynamic_cast<TTree*> (fFile->Get(calName));
		if (fPepBeamSpotCalTree != 0) {
		    fPepBeamSpotCalTree->SetBranchAddress("data", &fPepBeamSpotCal);
		    cout << "TConditions: Setting TTree for " << calName.Data() << endl;
		    fReadPepBeamSpotCal = kTRUE;
		}
		else {
		    cerr << "TConditions: Error setting TTree for " << className << endl;
		    fReadPepBeamSpotCal = kFALSE;
		}
	      }
	    }
	    else if(strcmp(className, "PepFillPatternR") == 0) {
		// Setup the PepFillPatternR interface
		
		fPepFillPattern = new PepFillPatternR();
		fPepFillPatternIndex= dynamic_cast<TBtree*> (fFile->Get("PepFillPatternRIndex"));
		fPepFillPatternTree= dynamic_cast<TTree*> (fFile->Get("PepFillPatternR"));
		if (fPepFillPatternTree != 0) {
		    fPepFillPatternTree->SetBranchAddress("data", &fPepFillPattern);
		    cout << "TConditions: Setting TTree for " << className << endl;
		}
		else {
		    cerr << "TConditions: Error setting TTree for " << className << endl;
		    fReadPepFillPattern = kFALSE;
		}
	    }
	    else if(strcmp(className, "PepBoostCalR") == 0) {
	      TString calName = TString("PepBoostCal_") + TString(bcalibration);
	      if (keyName == calName) {
		fPepBoostCal = new PepBoostCalR();
		TString index = calName + "Index";
		fPepBoostCalIndex= dynamic_cast<TBtree*> (fFile->Get(index));
		fPepBoostCalTree= dynamic_cast<TTree*> (fFile->Get(calName));
		fReadPepBoostCal = kTRUE;
		if (fPepBoostCalTree != 0) {
		    fPepBoostCalTree->SetBranchAddress("data", &fPepBoostCal);
		    cout << "TConditions: Setting TTree for " << calName.Data() << endl;
		}
		else {
		    cerr << "TConditions: Error setting TTree for " << className << endl;
		    fReadPepBoostCal = kFALSE;
		}
	      }
	    }
	    else if(strcmp(className, "DchBetheBlochR") == 0) {
		// Setup the DchCond interface
		
		fDchCond = new TDchCond();
		fDchCondIndex= dynamic_cast<TBtree*> (fFile->Get("DchBetheBlochRIndex"));
		fDchCondTree= dynamic_cast<TTree*> (fFile->Get("DchBetheBlochR"));
		if (fDchCondTree != 0) {
		    fDchCondTree->SetBranchAddress("data", &fDchCond);
		    cout << "TConditions: Setting TTree for " << className << endl;
		    fReadDchCond = kTRUE;
		}
		else {
		    cerr << "TConditions: Error setting TTree for " << className << endl;
		    fReadDchCond = kFALSE;
		}
	    }
	    else if(strcmp(className, "SvtPidSimpleTMCalibR") == 0) {		
	      if (keyName == "SvtPidCalibP:HitTwoSide") {
		fSvtPidCalib = new TSvtPidCalib();
		TString index = TString(keyName) + "Index";
		fSvtPidCalibIndex= dynamic_cast<TBtree*> (fFile->Get(index));
		fSvtPidCalibTree= dynamic_cast<TTree*> (fFile->Get(keyName));
		fReadSvtPidCalib = kTRUE;
		if (fSvtPidCalibTree != 0) {
		    fSvtPidCalibTree->SetBranchAddress("data", &fSvtPidCalib);
		    cout << "TConditions: Setting TTree for " << keyName.Data() << endl;
		}
		else {
		    cerr << "TConditions: Error setting TTree for " << className << endl;
		    fReadSvtPidCalib = kFALSE;
		}
	      }
	    }
	}
    }
    
}

TConditions::~TConditions() 
{
    delete fBeam;
    delete fPepEnergies;
    delete fPepBeamSpotCal;
    delete fPepFillPattern;
    delete fPepBoostCal;
    delete fDchCond;
    delete fSvtPidCalib;
    fFile->Close(); 
}

// Trigger the input of a suiting beam info object
// Time is given in seconds since 1/1/1901

Bool_t TConditions::At(TRhoTime t)
{
    return At(t.Seconds());
}

Bool_t TConditions::At(ULong_t when) {
    
    Bool_t valid = kFALSE;
    
    TCondKey key(when);
    
    // Read beam info
    
    if (fReadPepEnergies && fPepEnergiesIndex!=0) {
	const RooCondInterval* entry= (const RooCondInterval*)(fPepEnergiesIndex->FindObject(&key));
	if(entry!=0) {
	    int index = entry->index();
	    fPepEnergiesTree->GetEntry(index);
	    valid = kTRUE;
	    fCurrentTime = when;
	}
	else
	    cerr << "Error in reading PepEnergies index at (UTC)" << TRhoTime(when) << endl;
    }
    
    if (fReadPepBeamSpotCal && fPepBeamSpotCalIndex!=0) {
	const RooCondInterval* entry= (const RooCondInterval*)(fPepBeamSpotCalIndex->FindObject(&key));
	if(entry!=0) {
	    int index = entry->index();
	    fPepBeamSpotCalTree->GetEntry(index);
	    valid = kTRUE;
	    fCurrentTime = when;
	}
	else
	    cerr << "Error in reading PepBeamSpotCal index at (UTC)" << TRhoTime(when) << endl;
    }
    
    if (fReadPepFillPattern && fPepFillPatternIndex!=0) {
	const RooCondInterval* entry= (const RooCondInterval*)(fPepFillPatternIndex->FindObject(&key));
	if(entry!=0) {
	    int index = entry->index();
	    fPepFillPatternTree->GetEntry(index);
	    valid = kTRUE;
	    fCurrentTime = when;
	}
	else
	    cerr << "Error in reading PepFillPattern index  at (UTC)" << TRhoTime(when) << endl;
    }
    
    if (fReadPepBoostCal && fPepBoostCalIndex!=0) {
	const RooCondInterval* entry= (const RooCondInterval*)(fPepBoostCalIndex->FindObject(&key));
	if(entry!=0) {
	    int index = entry->index();
	    fPepBoostCalTree->GetEntry(index);
	    valid = kTRUE;
	    fCurrentTime = when;
	}
	else
	    cerr << "Error in reading PepBoostCal index at (UTC)" << TRhoTime(when) << endl;
    }
    
    if (fReadBeams && fBeamIndex!=0) {
	const RooCondInterval* entry= (const RooCondInterval*)(fBeamIndex->FindObject(&key));
	if(entry!=0) {
	    int index = entry->index();
	    fBeamTree->GetEntry(index);
	    valid = kTRUE;
	    fCurrentTime = when;
	}
	else
	    cerr << "Error in reading beam index at (UTC)" << TRhoTime(when) << endl;
    }
    else {
	// The new conditions data holds energy and calibration
	if (fPepEnergies!=0 && fPepBeamSpotCal!=0)
	  fBeam->Update(*fPepEnergies,*fPepBeamSpotCal);
	else
	  cerr << "TConditions: Could not update TBeams" << endl;
    }

    
    // Read Dch info
    
    if (fReadDchCond && fDchCondIndex!=0) {
	const RooCondInterval* entry= (const RooCondInterval*)(fDchCondIndex->FindObject(&key));
	if(entry!=0) {
	    int index = entry->index();
	    fDchCondTree->GetEntry(index);
	    valid = kTRUE;
	    fCurrentTime = when;
	}
	else
	    cerr << "Error in reading DchCond index at (UTC)" << TRhoTime(when) << endl;
    }
    
    
    // Read SVT info
    
    if (fReadSvtPidCalib && fSvtPidCalibIndex!=0) {
	const RooCondInterval* entry= (const RooCondInterval*)(fSvtPidCalibIndex->FindObject(&key));
	if(entry!=0) {
	    int index = entry->index();
	    fSvtPidCalibTree->GetEntry(index);
	    valid = kTRUE;
	    fCurrentTime = when;
	}
	else
	    cerr << "Error in reading SvtPidCalib index at (UTC)" << TRhoTime(when) << endl;
    }
    
    
    return valid;
}

void TConditions::Authorize(const char *u, const char *p)
{
// Authorization for ROOTD might be explicit or by use of environment variables

    TString user,passwd;

    if (u != 0)
       user = u;
    else
       if (getenv("RHOUSER")!=0)  user = getenv("RHOUSER");

    if (p != 0)
       passwd = p;
    else
       if (getenv("RHOPASSWD")!=0) passwd = getenv("RHOPASSWD");

    cout << "TConditions::Authorize " << user.Data() << " for ROOTD access" << endl;

    TAuthenticate::SetGlobalUser(user); 
    TAuthenticate::SetGlobalPasswd(passwd);
}

// Print the conditions data

void TConditions::PrintOn(std::ostream& o) const
{
    o << endl;
    o << "Conditions info at (UTC): " << TRhoTime(fCurrentTime) << endl;
    if (fBeam!=0) o << *fBeam << endl;
    if (fReadPepEnergies && fPepEnergies!=0) o << *fPepEnergies << endl;
    if (fReadPepBeamSpotCal && fPepBeamSpotCal!=0) o << *fPepBeamSpotCal << endl;
    if (fReadPepFillPattern && fPepFillPattern!=0) o << *fPepFillPattern << endl;
    if (fReadPepBoostCal && fPepBoostCal!=0) o << *fPepBoostCal << endl;
    if (fReadDchCond && fDchCond!=0) o << *fDchCond << endl;
    if (fReadSvtPidCalib && fSvtPidCalib!=0) o << *fSvtPidCalib << endl;
}


// --------------------
// non-member functions
// --------------------

std::ostream&  operator << (std::ostream& o, const TConditions& a) {a.PrintOn(o); return o;}


ClassImp(RooCondInterval)

RooCondInterval::RooCondInterval()
{
    // don't set valid bit in the default ctor
}

RooCondInterval::~RooCondInterval() { }

Bool_t RooCondInterval::IsSortable() const {
    return kTRUE;
}

Int_t RooCondInterval::Compare(const TObject* other) const {
    UInt_t key= other->GetUniqueID();
    if(key < GetUniqueID()) return +1; // we are "larger" than the other object
    if(key >= _end) return -1;  // we are "smaller" than the other object
    return 0; // we are "equal" to the other object
}


ClassImp(RooCondData)

RooCondData::RooCondData() { }

RooCondData::~RooCondData() { }

Stat_t RooCondData::Store(TBranch& where) const {
    Stat_t index= where.GetEntries();
    where.Fill();
    return index;
}

