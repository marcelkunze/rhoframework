#ifndef PAFLISTBASE_HH
#define PAFLISTBASE_HH
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

#include "TObject.h"
#include <iosfwd>

#define MAXLIST 50

class TClonesArray;
class TBranchClones;
class TObjArray;
class PAFCompAodBase;
class TTree;

typedef TClonesArray* TClonesPtr;

class PAFListBase : public TObject {

  public :

  PAFListBase();
  PAFListBase(Int_t mode);

  virtual ~PAFListBase();

  // Append composite AOD - Object to specific list 
  void AddCompAodBase(PAFCompAodBase &compaod, char *listname);

  // Append composite AOD - Object to active list
  void AddCompAodBase(PAFCompAodBase &compaod);

  // Set 'active' list
  void SetActiveList(const char *name);

  // Set Tree ; for each list, there is a Branch with the same
  // name in this tree
  void SetTree(TTree *t);
  TTree* GetTree() { return fTree; }
  void Fill();

  // set mode ( 0 = read , 1 = write)
  void SetMode(Int_t m) { fMode=m; }

  // Assign list for writing; must be 
  // called for each list _before_ the first Event
  void AddList(char *name);					    

  // Get Pointer to contents (=TClonesArray) of specific list
  TClonesArray* GetList(const char *name, Int_t nEvent);
  TClonesArray* GetPidList(const char *name, Int_t nEvent);

  Int_t GetListLength(const char *name,Int_t nEvent);
  Int_t GetPidListLength(const char *name,Int_t nEvent);

  UInt_t GetNumberOfLists();
  const char* GetListName(Int_t i);

  // Clean All Lists
  void Reset();
  void PrintOn(ostream &o);

  private :
    
    Int_t GetBranchPos(const char *name,Int_t );
    // Buffers for Branches; (*_theListBuffer)[i] is the 
    // TClonesArray of the i-th Branch
    TClonesPtr       *fListBuffer;
    Int_t            *fNelem;
    TBranchClones*    fBranches[MAXLIST];
    Int_t	      fNlists, fN;
    
    // (*_nameList)[i] = name of list corresponding to i-th
    // Buffer ( == name if i-th Branch);
    // TObjArray *fNameList;

    TTree* fTree;
    TClonesArray *fActiveList;  

    Int_t fMode;    // 0 = read, 1 = write

    TClonesArray *buf;
};

#endif
