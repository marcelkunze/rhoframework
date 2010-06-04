#ifndef PAFSTREAM_H
#define PAFSTREAM_H
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

#include <iostream>

#include "TObject.h"
#include "TNamed.h"
#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TObjArray.h"

class TBranch;


//------------------------------------------------------------------------

// A PAFStreamMember represents either a Leaflist, a ClonesArray or an 
// TObject registered in a PAFStream (see below). It is used internally 
// only.

class PAFStreamMember: public TObject {
    
public:
    
    enum MemberType { ClonesArray=0, Leaflist=1, Object=2 };
    
    PAFStreamMember() { }
    PAFStreamMember(MemberType AType, const char* ASubBranch, const char* ABranchName, 
	void* ABranchAddress, const char* AClassName);
    virtual ~PAFStreamMember() { }
    
    MemberType fMemberType;
    TString    fSubBranch;
    TString    fBranchName;
    TBranch    *fBranch;
    void       *fBranchAddress;
    TString    fClassName;
};

//------------------------------------------------------------------------

// A PAFStream is a description of the file structure of a root file.
// There is one stream for each file type (by default one for Aod and 
// one for Tag).

class PAFStream: public TNamed {
    
public:
    
    PAFStream() { }
    PAFStream(const char* AFileNameBase, const char* ATreeName);
    virtual ~PAFStream();
    
    void AddClonesArray(const char* ASubBranch, const char* ABranchName, void* ABranchAddress, const Text_t* AClassName);
    void AddLeaflist(const char* ASubBranch, const char* ABranchName, void* ABranchAddress, const Text_t *ALeaflist);
    void AddObject(const char* ASubBranch, const char* ABranchName, void* ABranchAddress, const Text_t* AClassName);
    
    Bool_t IsActive() { return fActive; }
    Bool_t Enable();
    Bool_t Exists();
    void   Disable() { fActive= kFALSE; }
    void   Lock() { fLock= kTRUE; }
    void   Unlock() { fLock= kFALSE; }
    
    const char*      GetFileName() { return fCurrentFilename; }
    const char*      GetFileNameBase() { return GetName(); }
    const char*      GetTreeName() { return GetTitle(); }
    const TObjArray* GetMembers() { return fMembers; }
    const PAFStreamMember* GetMember(Int_t MemberNo) { return (PAFStreamMember*)fMembers->At(MemberNo); }
    TFile*           GetFile() { return fFile; }
    TTree*           GetTree() { return fTree; }
    
    TBranch* FindBranch(PAFStreamMember *m);

    void SetFile(TFile* AFile) { fFile= AFile; }
    Bool_t SetTree(TTree* ATree);
    Int_t GetNumberOfEvents() { return fCount; }
    
    // open file for reading , read the Tree from it, and initialise all its Branches;
    // all subsequent calls of "GetEvent(index)" refer to this Tree; returns "false" in
    // case the file does not exist 
    TFile* OpenFile(const char *name, Bool_t activate=kFALSE); 
    void CloseFile();
    Bool_t Append(TFile *);
    
    // Retrieves the n-th Element from the current Tree and returns the number of Btyes read, or 
    // -1 in case the Stream is disabled
    Int_t GetEvent(Int_t n);
    
   void PrintOn(std::ostream& o=std::cout) const;

private:
    
    TString    fCurrentFilename; // name of file from which fTree originates 
    TObjArray  *fMembers;        // a list of PAFStreamMember objects
    TFile      *fFile;
    TTree      *fTree;
    Int_t      fCount;
    Bool_t     fActive;
    Bool_t     fOk;              // false if fTree not initialised 
    Bool_t     fLock;
public:
    ClassDef(PAFStream,1) // PAF data stream
};

#endif

