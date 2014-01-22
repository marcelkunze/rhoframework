//////////////////////////////////////////////////////////////////////////
// 									//
// PAFStream			               				//
// 									//
// Data stream class in PAF(Pico Analysis Framework)	    		//
// A "stream" is a file with a Tree in it                               //
// 									//
// Author: Leif Wilden & Thorsten Brandt, TU Dresden			//
// changes: network capability, Helmut Schmuecker, Oct. 99		//
// Mods:   Stream is TNamed, MK, Jan 2k					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "TFile.h"
#include "TNetFile.h"
#include "TTree.h"
#include "TBranch.h"

#include "PAFSchema/PAFStream.h"
#include "RhoBase/TRho.h"

ClassImp(PAFStream)

#include <iostream>
#include <cstdlib>
using namespace std;

//------------------------------------------------------------------------

PAFStream::PAFStream(const char* AFileNameBase, const char* ATreeName):
TNamed(AFileNameBase,ATreeName),
fCurrentFilename(""),
fFile(0), fTree(0), fOk(kFALSE), fActive(kTRUE), fLock(kFALSE), fCount(0)
{ 
    fMembers= new TObjArray(10);
}

PAFStream::~PAFStream()
{
    if (fFile != 0) fFile->Close();
    fMembers->Delete();
    delete fMembers;
    fMembers= 0;
}

//------------------------------------------------------------------------

void PAFStream::AddClonesArray(const char* ASubBranch, const char* ABranchName, 
			       void* ABranchAddress, const Text_t* AClassName)
{
    fMembers->Add(new PAFStreamMember
	(PAFStreamMember::ClonesArray,ASubBranch,ABranchName,ABranchAddress, AClassName) );
}

void PAFStream::AddLeaflist(const char* ASubBranch, const char* ABranchName,
			    void* ABranchAddress, const Text_t *ALeaflist)
{
    fMembers->Add(new PAFStreamMember
	(PAFStreamMember::Leaflist,ASubBranch,ABranchName,ABranchAddress, ALeaflist) );
}

void PAFStream::AddObject(const char* ASubBranch, const char* ABranchName,
			  void* ABranchAddress, const Text_t* AClassName)
{
    fMembers->Add(new PAFStreamMember
	(PAFStreamMember::Object,ASubBranch,ABranchName,ABranchAddress, AClassName) );
}

//------------------------------------------------------------------------

void PAFStream::PrintOn(std::ostream& o) const
{
    o << "Stream " << GetName() << "; Treename= " << GetTitle();
    if (fActive) o << "; enabled" << endl; else o << "; disabled" << endl;
    for (int j=0; j < fMembers->GetEntriesFast(); j++) {
	const PAFStreamMember *m= (PAFStreamMember*)fMembers->At(j);
	o << "  ";
	switch (m->fMemberType) {
	case (PAFStreamMember::ClonesArray): o << "ClonesArray"; break;
	case (PAFStreamMember::Leaflist)   : o << "Leaflist"; break;
	case (PAFStreamMember::Object)     : o << "Object"; break;
	}
	o << "; " << m->fSubBranch.Data() << "; " << m->fBranchName.Data() << "; " << m->fClassName << endl;
    }
}

//------------------------------------------------------------------------

TFile *
PAFStream::OpenFile(const char *filename, Bool_t activate)
{
    
    if (fFile!=NULL) {
	fFile->Close();
	fFile=NULL;
    }
    
    TString fn(filename);
    if (fn.Index(".root")<0) { // Generic name given ?
	fn = fn + GetName() + ".root";
    }
    fCurrentFilename = fn;
    
    if (fActive || activate) {
	if (fCurrentFilename.Index("root:")>=0) TRho::Instance()->Authorize();
	cout << "**** Open " << fCurrentFilename.Data() << endl;
	// static TFile::Open returns either TFile or TNetFile depending on fCurrentFilename
	fFile = TFile::Open(fCurrentFilename.Data());
	if (fFile->IsZombie()) {
	    cout << "Error opening file " << fCurrentFilename.Data() << endl;
	    fFile->Close();
	}

	if (fFile->IsOpen()) {
	    //fFile->ShowStreamerInfo();
	    const char *name = GetTitle();
	    TObject *obj = fFile->Get(name);
	    TTree* newTree = dynamic_cast<TTree*> (obj);
	    if (newTree==NULL) {
		cout << "ERROR ! Could not find " << GetTitle() 
		    << " in File " << fCurrentFilename.Data() << endl;
		return 0;
	    }
	    //newTree->SetBranchStatus("*",0); // Disable all branches
	    SetTree(newTree);
	    fOk=kTRUE;
	}
	else
	    fFile = 0;
    } else {
	SetTree(NULL);
	fOk = kFALSE;
    }
    return fFile;
}

//------------------------------------------------------------------------

void
PAFStream::CloseFile()
{
    if (fFile!=0) {
	fFile->Close();
	fFile=0;
    }
    fCount = 0;
}

//------------------------------------------------------------------------

Bool_t
PAFStream::SetTree(TTree *t)
{
    fTree=t;
    if (fActive) {
	fCount = (Int_t) t->GetEntries(); // Number of entries in the tree
	// iterate over all members and set their branch address:
	for (int j=0; j < fMembers->GetEntriesFast(); j++) {
	    PAFStreamMember *m=  (PAFStreamMember*) fMembers->At(j);
	    m->fBranch = FindBranch(m); // Set the actual branch
	}
	fOk=kTRUE;
    } else {
	fOk = kFALSE;
    }
    return fOk;
}

//------------------------------------------------------------------------

Bool_t
PAFStream::Enable()
{
    if (fLock) return kFALSE;
    
    fActive=kTRUE;
    if (!fOk) {
	if (fTree!=NULL) { 
	    for (int j=0; j < fMembers->GetEntriesFast(); j++) {
		PAFStreamMember *m=  (PAFStreamMember*) fMembers->At(j);
		m->fBranch = FindBranch(m); // Set the actual branch
	    }
	    fOk = kTRUE;
	} 
	else
	    return Exists();
    }
    return kTRUE;
}

//------------------------------------------------------------------------

Bool_t
PAFStream::Exists()
{
    if (fFile != 0) return kTRUE; // Already open
    if (fCurrentFilename == "") return kFALSE; // Do not try to open this, yet no file assigned
    if (fCurrentFilename.Index("root:")>=0) TRho::Instance()->Authorize();
    cout << "PAFStream::OpenFile " << fCurrentFilename.Data() << endl;
    fFile = TFile::Open(fCurrentFilename.Data());
    if (fFile->IsOpen()) {
	TTree* newTree = (TTree*) fFile->Get(GetTitle());
	if (newTree==NULL) {
	    cout << "ERROR ! Could not find " << GetTitle() 
		<< " in File " << fCurrentFilename.Data() << endl;
	    fActive = kFALSE;
	    return kFALSE; 
	}
	SetTree(newTree);
	fOk=kTRUE;
    }
    else
	return kFALSE;
    
    return kTRUE;
}

//------------------------------------------------------------------------

PAFStreamMember::PAFStreamMember(MemberType AMemberType, const char* ASubBranch,
				 const char* ABranchName, void* ABranchAddress, const char* AClassName):
fMemberType(AMemberType),
fSubBranch(ASubBranch),
fBranchName(ABranchName),
fBranch(0),
fBranchAddress(ABranchAddress),
fClassName(AClassName)
{
}


//------------------------------------------------------------------------

Bool_t
PAFStream::Append(TFile *file)
{ 
    fTree = 0;
    Bool_t ok = kFALSE;
    
    if (file->IsOpen()) {
	TTree* newTree = (TTree*) file->Get(GetTitle());
	if (newTree == 0) {
	    cerr << "PAFStream: Could not find " << GetTitle() 
		<< " in file " << file->GetName() << endl;
	    fActive = kFALSE;
	    return kFALSE;
	}
	ok = SetTree(newTree);
	fFile = file;
    }
    else {
	cerr << "PAFStream: Could not append " << GetTitle()
	    << " to file " << file->GetName() << endl;
	fActive = kFALSE;
    }
    
    return ok;
}

//------------------------------------------------------------------------

// Retrieves the n-th Element from the current Tree and returns the number of Btyes read, or 
// -1 in case the Stream is disabled
Int_t PAFStream::GetEvent(Int_t n) 
{
    if (n>fCount) return -1; // We try to read beyond last entry
    
    if (fLock) return 0;
    
    Int_t nBytes = 0;
    if (fActive) { 
	if (fTree) {
	    //return fTree->GetEvent(n); // This traverses all branches
	    //The following goes faster as only the actual branches are read
	    for (int j=0; j < fMembers->GetEntriesFast(); j++) {
		PAFStreamMember *m=  (PAFStreamMember*) fMembers->At(j);
		if (m->fBranch!=0) nBytes += m->fBranch->GetEvent(n); // Get all branches
	    }
	    
	} else {
	    cout << "PAFStream::GetEvent: NO STREAM !!"<<endl;
	    return nBytes;
	}
    } 

    return nBytes; 
}

//------------------------------------------------------------------------

TBranch* PAFStream::FindBranch(PAFStreamMember *m)
{
    if (fTree==0 || m==0) return 0;

    if (!strcmp("",m->fBranchName.Data())) return 0; // Nothing to do

    // Data lives in the top level
    if (!strcmp("",m->fSubBranch.Data())) {
	cout << "PAFStream::FindBranch: Looking for " << m->fBranchName.Data() << " in " << GetTitle();
	TBranch *theBranch = fTree->GetBranch(m->fBranchName.Data());
	if (theBranch != 0) {
	    cout << " (OK)" << endl;
	    theBranch->ResetBit(kDoNotProcess);
	    theBranch->SetAddress(m->fBranchAddress);
	}
	else
	    cout << " (not OK)" << endl;

	return theBranch;
    }

    // Data lives in a sub branch
    TBranch *b = fTree->GetBranch(m->fSubBranch.Data());
    if (b) {
      cout << "PAFStream::FindBranch: Looking for " << m->fBranchName.Data() << " in " << m->fSubBranch.Data();
      b->ResetBit(kDoNotProcess);
      TIterator* iter = b->GetListOfBranches()->MakeIterator();
      TObject* p = 0;
      while ( (p=iter->Next()) ) {
	  TBranch *branch = static_cast<TBranch*>(p);
	  const char *name = branch->GetName();
          if (!strcmp(name,m->fBranchName.Data())) {
	      cout << " (OK)" << endl;
	      branch->ResetBit(kDoNotProcess);
	      branch->SetAddress(m->fBranchAddress);
	      return branch;
	  }
      }
      cout << " (not OK)" << endl;
    }
    else	
	cerr << "PAFStream::FindBranch: The tree " << GetTitle() << " does not contain a branch " << m->fSubBranch.Data() << endl;

    return 0;
}
