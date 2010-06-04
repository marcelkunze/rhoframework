//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFListBase								//
//                                                                      //
// Keep track of candidate lists in the event	    			//
//                                                                      //
// Author: Th. Brandt, TUD, Aug.99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "PAFSchema/PAFListBase.h"

#include "TClonesArray.h"
#include "TObjArray.h"
#include "TTree.h"
#include "TBranch.h"
#include "TBranchClones.h"
#include "TList.h"
#include "TString.h"
#include "TFile.h"

#include "PAFSchema/PAFCompAodBase.h"

ClassImp(PAFListBase)

#include <iostream>
using namespace std;

PAFListBase::PAFListBase()
: fMode(0), fTree(0), fActiveList(0)
{
    buf=0;
    fNlists=fN=0;
    fListBuffer = new TClonesArray* [MAXLIST];
    fNelem = new Int_t[MAXLIST];
    fMode=-1;
    for (int i=0;i<MAXLIST;i++) fNelem[i] = 0;
}

PAFListBase::PAFListBase(Int_t mode)
: fMode(mode), fTree(0), fActiveList(0)
{
    fNlists=fN=0;
    fMode=-1;
    fListBuffer = new TClonesArray* [MAXLIST];
    fNelem = new Int_t[MAXLIST];
    for (int i=0;i<MAXLIST;i++) fNelem[i] = 0;
}

PAFListBase::~PAFListBase()
{
    //fListBuffer->Delete();
    //delete fListBuffer;
}

void PAFListBase::Fill()
{
    if ((fTree!=NULL) && (fNlists>0) ) { 
	fTree->Fill(); 
    }
}

UInt_t PAFListBase::GetNumberOfLists() { return fNlists; }

const char* PAFListBase::GetListName(Int_t i) { return fBranches[i]->GetName(); }

void PAFListBase::AddCompAodBase(PAFCompAodBase &compaod, char *listname)
{
    if (fMode!=0) {
	if (fTree!=0) {
	    TBranchClones *br = (TBranchClones*) fTree->GetBranch(listname);
	    if (br!=0) {
		TClonesArray *buffer = br->GetList();
		Int_t pos = buffer->GetLast()+1;
		new ((*buffer)[pos]) PAFCompAodBase(compaod);
		fNelem[fN]++; 
	    }  else {
		cerr << " *** ERROR IN PafListBase::AddCompAodBase : List \'";
		cerr << listname << "\' is not present " << endl; 
	    }
	} else {
	    cerr <<  " *** ERROR IN PafListBase::AddCompAodBase; No Tree defined " << endl;
	}
    } else {
	cerr <<  " *** ERROR IN PafListBase::AddCompAodBase; Not in write mode " << endl;
    }
}


void PAFListBase::AddCompAodBase(PAFCompAodBase &compaod)
{
    if (fMode!=0) {
	if (fTree!=0) {
	    if (fActiveList!=NULL) {
		Int_t pos = fNelem[fN];
		new ((*fActiveList)[pos]) PAFCompAodBase(compaod);
		PAFCompAodBase *test = (PAFCompAodBase *) (*fActiveList)[pos];
		fNelem[fN]++; 
	    } else {
		cerr <<  " *** ERROR IN PafListBase::AddCompAodBase; No active List defined " << endl;
	    }
	} else {
	    cerr <<  " *** ERROR IN PafListBase::AddCompAodBase; No Tree defined " << endl;
	}
    } else {
	cerr <<  " *** ERROR IN PafListBase::AddCompAodBase; Not in write mode " << endl;
    }
}

void PAFListBase::SetActiveList(const char *listname)
{
    if (fMode!=0) {
	if (fTree!=0) {
	    TBranchClones *br = (TBranchClones*) fTree->GetBranch(listname);
	    if (br!=0) {
		fActiveList = br->GetList();
	    }  else {
		cerr << " *** ERROR IN PafListBase::SetActiveList : List \'";
		cerr << listname << "\' is not present " << endl; 
		fActiveList=NULL;
	    }
	} else {
	    cerr <<  " *** ERROR IN PafListBase::SetActiveList; No Tree defined " << endl;
	}
    } else {
	cerr <<  " *** ERROR IN PafListBase::SetActiveList; Not in write mode " << endl;
    }
}

void PAFListBase::AddList(char *name)
{
    if (fNlists>=MAXLIST) {
	cerr << "PAFListBase: Limit of " << MAXLIST << " reached." << endl;
	return;
    }
    if (fMode!=0) {
	if (fTree!=NULL) {
	    if (fTree->GetBranch(name)==NULL) {
		fListBuffer[fNlists] = new TClonesArray("PAFCompAodBase");
		fTree->Branch(name, &(fListBuffer[fNlists]));
		fBranches[fNlists] = (TBranchClones*) fTree->GetBranch(name);
		fNelem[fNlists]=0;
		fN = fNlists; 
		fNlists++;
		cout << "AddList " << name << endl;
	    } else {
		fN = GetBranchPos(name,1);
		if (fN>-1) {
		    fNelem[fN]=0;
		}
		else {
		    cerr << " !!! WARNING IN PAFListBase::AddList : List \'";
		    cerr << name << "\' is already present " << endl;
		}
	    }
	} else {
	    cerr <<  " *** ERROR IN PafListBase::AddList; No Tree defined " << endl;
	}
    } else {
	cerr <<  " *** ERROR IN PafListBase::AddList; Not in write mode " << endl;
    }
}

Int_t
PAFListBase::GetBranchPos(const char *name, Int_t enableDau)
{
    if (fTree==NULL) { return -1; }
    Int_t i=0;   
    TString nameStr(name); 
    TBranchClones *theBranch=NULL;  
    while ((i<fNlists) && (theBranch==NULL) ) {
	if (nameStr==TString(fBranches[i]->GetName())) {
	    theBranch = fBranches[i];
	} else {
	    i++;
	}
    }
    if (!theBranch) {
	theBranch=(TBranchClones*) fTree->GetBranch(name);
	if (theBranch) {
	    fListBuffer[fNlists] = new TClonesArray("PAFCompAodBase");
	    fBranches[fNlists] = theBranch;
	    theBranch->SetAddress(&(fListBuffer[fNlists]));
	    TString subbranch1 =  TString(name)+".fAodRef";
	    TString subbranch4 =  TString(name)+".fPdtRef";
	    TString subbranch2 =  TString(name)+".fDaughters[5]";
	    TString subbranch3 =  TString(name);
	    Int_t version = fTree->GetCurrentFile()->GetVersion();
	    if (version<30000) subbranch3 += "_"; // ROOT2 requires an underscore here...
	    fTree->SetBranchStatus(subbranch1,1);
	    fTree->SetBranchStatus(subbranch4,1);
	    if (enableDau) fTree->GetBranch(subbranch2)->ResetBit(kDoNotProcess); // ROOT 2.23
	    //fTree->SetBranchStatus(subbranch2,enableDau);
	    fTree->GetBranch(subbranch3)->SetAddress(&(fNelem[fNlists]));
	    if (fNlists<MAXLIST) 
		fNlists++;
	    else
		return -1;
	} else {
	    return -1;
	}
    } 
    return fN = i;
}

TClonesArray*
PAFListBase::GetList(const char *name, Int_t nEvent)
{
    Int_t i = GetBranchPos(name,1);
    if (i==-1) { return NULL; }
    if (nEvent>=0) fBranches[i]->GetEntry(nEvent);
    return fListBuffer[i];
}

Int_t 
PAFListBase::GetListLength(const char* name, Int_t nEvent)
{
    Int_t i = GetBranchPos(name,1);
    if (i==-1) { return 0; }
    if (nEvent>=0) fBranches[i]->GetEntry(nEvent);
    return fNelem[i];
}

TClonesArray*
PAFListBase::GetPidList(const char *name, Int_t nEvent)
{
    Int_t i = GetBranchPos(name,0);
    if (nEvent>=0) fBranches[i]->GetEntry(nEvent);
    return fListBuffer[i];
}

Int_t 
PAFListBase::GetPidListLength(const char* name, Int_t nEvent)
{
    Int_t i = GetBranchPos(name,0);
    if (nEvent>=0) fBranches[i]->GetEntry(nEvent);
    return fNelem[i];
}

void
PAFListBase::Reset()
{
    for (Int_t i=0; i<fNlists; i++) {
	fListBuffer[i]->Clear();
	fNelem[i] = 0;
    }
}

void
PAFListBase::PrintOn(std::ostream &o)
{
    o << "PAFListBase : " << endl;
    if (fTree==NULL) {
	o << "no tree defined; no list present" << endl;
	return;
    }
    
    o << " Number of Lists : " << fNlists << endl << endl;
    for (Int_t i=0; i<fNlists; i++) {
	TString name = fBranches[i]->GetName();
	Int_t currentIndex = fTree->GetReadEvent();
	Int_t len = GetListLength(name,currentIndex);
	if (len<=0) continue;
	o << "List # " << i << " has name " << name << "; Entries : " << len;
	if (fBranches[i]->GetList()!=NULL) { 
	    TClonesArray *a = GetList(name,currentIndex);
	    //if (a->GetLast()==-1) {
	    //o << " - empty - " << endl;
	    //} else { 
	    o << endl;
	    for (Int_t k=0; k<len; k++) {
		PAFCompAodBase *comp = (PAFCompAodBase*) ((*a)[k]);
		o << "   --> "; comp->PrintOn(cout);
	    }
	    //}
	}
    }
}

void
PAFListBase::SetTree(TTree *t)
{
    fTree=t;
    fNlists=0;
    //  if (fTree!=0) { fTree->SetBranchStatus("*",0); }
}
