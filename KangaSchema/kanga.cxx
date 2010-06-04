//--------------------------------------------------------------------------
// Class library to read the BABAR KanGA data
// Author: Leif Wilden, TUD, 1999
//------------------------------------------------------------------------
#include "KangaSchema/kanga.h"

#include <assert.h>
#include <stdlib.h>

#include "TROOT.h"
#include "TClass.h"
#include "TRealData.h"
#include "TDataMember.h"
#include "TDataType.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TString.h"

#include "KangaSchema/TBtaMicroCandR.h"
#include "RhoBase/TSmartPointer.h"

ClassImp(TCandVariable);
ClassImp(TCandDirectory);
ClassImp(TBtaMicroCandRDir);

#include <iostream>
#include <fstream>
using namespace std;

//------------------------------------------------------------

TCandVariable::TCandVariable(Int_t AnId, const char* AName, 
			     const char* AnAlias):
  TNamed(AnAlias,AName),
  fVarID(AnId),
  fBranch(0),
  fOffset(0)
{}

TCandVariable::~TCandVariable() { delete fBranch; }

//------------------------------------------------------------

TCandDirectory::TCandDirectory(): fMaxid(0)
{
    fVarsByAlias = new THashList(kBranches);

    for (int i=0;i<kBranches;i++) {
      fDoNotProcess[i] = kTRUE;
      fCandArray[i] = 0;
    }
}

TCandDirectory::TCandDirectory(const char* AFileName): fMaxid(0)
{
    fVarsByAlias = new THashList(kBranches);

    for (int i=0;i<kBranches;i++) {
      fDoNotProcess[i] = kTRUE;
      fCandArray[i] = 0;
    }

    ReadAliases(AFileName);
}

TCandDirectory::~TCandDirectory() 
{
    fVarsByAlias->Delete(); // Remove the TCandVariables
    delete fVarsByAlias;
}

void TCandDirectory::Reset()
{
    fVarsByAlias->Delete(); // Remove the TCandVariables
    fMaxid = 0;
    for (int i=0;i<kBranches;i++) {
      fDoNotProcess[i] = kTRUE;
      fCandArray[i] = 0;
    }
}

const char* TCandDirectory::GetClassName(const TBranchClones* bc)
{
  TClass *cl = gROOT->GetClass("TBranchClones");
  if (!cl->GetListOfRealData()) cl->BuildRealData();
  TIter next(cl->GetListOfRealData());
  TRealData *rd;
  char* bcc;

  while ((rd = (TRealData*) next())) {
    if (!strcmp(rd->GetName(),"fClassName.*fData")) {
      bcc= (char*)bc;
      char *pointer = (char*)(bcc + rd->GetThisOffset());
      char **ppointer = (char**)(pointer);
      return *ppointer;
    }
  }
  return "unknown";
}

void TCandDirectory::AssignVars(TBranchClones* bc, const TObject* o)
{

  // check if ROOT knows about the given class o:

  TClass *cl = gROOT->GetClass(o->ClassName());
  if (!cl) {
    cout << "Kanga::AssignVars: The class " << o->ClassName() << " is unknown to ROOT?!";
    assert(cl);
  }

  // now loop over all data members of class o and try to find
  // a corresponding branch. If successfull set the leaf's memory
  // offset:

  if (!cl->GetListOfRealData()) cl->BuildRealData();
  //char branchname[80];
  TRealData     *rd;
  TCandVariable *var;
  TBranch       *branch;
  TLeaf         *leaf;
  Int_t         n=0;
  TIter         next(cl->GetListOfRealData());

  while ((rd = (TRealData *) next())) {

    TDataMember *member = rd->GetDataMember();
    if (!member->IsBasic())      continue;
    if (!member->IsPersistent()) continue;
    TDataType *membertype = member->GetDataType();
    if (membertype->GetType() == 0) continue;

    var= GetVarByAlias(rd->GetName());

    if (!var) {
      var= new TCandVariable(++fMaxid,rd->GetName(),rd->GetName());
      fVarsByAlias->Add(var);
      if (fMaxid>kBranches) {
	  cerr << "TCandDirectory::AssignVars: kBranches cache too small." << endl;
      }
      else {
          fCandArray[fMaxid] = var;
      }
    }

    if (!strcmp(var->GetTitle(),"none")) continue;

    //sprintf(branchname,"%s.%s",bc->GetName(),var->fVarName.Data());
    TString branchname(bc->GetName());
    branchname.Append(".");
    branchname.Append(var->GetTitle());
    branch= (TBranch*)bc->GetListOfBranches()->FindObject(branchname);
    if (!branch) {
      //cout << "Kanga::AssignVars: No branch found for member " << rd->GetName() << 
      //     " (" << branchname <<")." << endl;
      continue;
    }

    TObjArray *leaves = branch->GetListOfLeaves();
    leaf = (TLeaf*)leaves->UncheckedAt(0);
    leaf->SetOffset(rd->GetThisOffset());
    branch->GetTree()->GetBranch(branchname)->SetBit(kDoNotProcess);
    var->fOffset= rd->GetThisOffset();
    var->fBranch= new TSmartPointer(branch);
    n++;
  }
  cout << "Kanga::AssignVars:  " << n << " data members of object " << o->GetName() << 
          " have been assigned to a branch." << endl;
}

TCandVariable* TCandDirectory::GetVarByAlias(const char* AVarAlias) const
{
  TCandVariable* v= (TCandVariable*) fVarsByAlias->FindObject(AVarAlias);
  return v;
}

TCandVariable* TCandDirectory::GetVarByID(const Int_t AVarID) const
{
  if (AVarID>=kBranches) return 0;
  return fCandArray[AVarID];
}

void TCandDirectory::ReadAliases(const char* AFileName)
{
  Int_t     id,n=0;
  Char_t    name[80],alias[80];
  ifstream  s;
  TCandVariable* var;

  s.open(AFileName);
  if (!s) {
    cout << "Kanga::ReadAliases: File " << AFileName << " does not exist!" << endl;
    assert(0);
  };
  while (!s.eof()) {
    s >> name;
    s >> alias;
    id= 0;
    s >> id;
    if (id>0) {
      if (id>kBranches) {
	cerr << "TCandDirectory::ReadAliases: kBranches cache too small. Requesting entry " << id << endl;
	continue;
      }
      var= new TCandVariable(id,name,alias);
      fVarsByAlias->Add(var);
      fCandArray[id] = var;
      if (id>fMaxid) fMaxid= id;
      n++;
    }
  }
  s.close();
  cout << "Kanga::ReadAliases: " << n << " aliases read from " << AFileName << "." << endl;  
}

void TCandDirectory::ReadBranch(const Int_t AVarID)
{
  //if (fDoNotProcess[AVarID]) return; // We do not have this variable on file

  TCandVariable* v= GetVarByID(AVarID);
  if (!v) { 
    cout << "Kanga::ReadBranch: Can not read variable with ID=" << AVarID << " (Schema evolution ?). " << endl;
    InvalidateBranch(AVarID);
    return;
  }

  if (v->fBranch == 0) {
    cout << "Kanga::ReadBranch: Try to access non-existant branch " << v->GetTitle() << endl;
    return;
  }

  v->fBranch->CheckBranch();
}

//------------------------------------------------------------

TBtaMicroCandRDir::~TBtaMicroCandRDir()
{
    delete fInstance;
    fInstance = 0;
}

Int_t TBtaMicroCandRDir::fSchema = 0;

TBtaMicroCandRDir* TBtaMicroCandRDir::fInstance;

TBtaMicroCandRDir* TBtaMicroCandRDir::Instance()
{
  if (!fInstance) fInstance= new TBtaMicroCandRDir();
  return fInstance;
}

void TBtaMicroCandRDir::Init(TTree *t, TClonesArray** c, const char* SchemaDir)
{
  TBranch* aodBranch = t->GetBranch("BtaAod");
  assert(aodBranch);
  TIterator* aodIter = aodBranch->GetListOfBranches()->MakeIterator();
  TBranchClones *bc = 0;
  TObject* p = 0;
  while ( (p=aodIter->Next()) && !bc ) {
    if ( !strcmp(static_cast<TBranch*>(p)->GetName(),"_array") ) {
      bc=static_cast<TBranchClones*>(p);
    }
  }

  if (!bc) { cout << "Kanga::Init: The tree t does not contain a branch with name _array?!" << endl; }
  assert(bc);
  if (c) bc->SetAddress(c);

  TString schema(GetClassName(bc));

  if (schema=="BtaMicroCandR_001") fSchema = 1;
  if (schema=="BtaMicroCandR_002") fSchema = 2;
  if (schema=="BtaMicroCandR_003") fSchema = 3;
  if (schema=="BtaMicroCandR_004") fSchema = 4;
  if (schema=="BtaMicroCandR_005") fSchema = 5;

  cout << "Loading schema #" << fSchema << " for " << schema << endl;

  TString s(SchemaDir);
  if (!SchemaDir) s+=".";
  s+= "/";
  s+= schema;
  s+= ".sdf";
  Reset();
  ReadAliases(s.Data());
  TBtaMicroCandR bmc;
  AssignVars(bc,&bmc);
}

Int_t TBtaMicroCandRDir::GetSchema() { return fSchema; }
void TBtaMicroCandRDir::SetSchema(Int_t s) { fSchema = s; }

//------------------------------------------------------------




