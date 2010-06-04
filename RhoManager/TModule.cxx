//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TModule								//
//                                                                      //
// Base class for modules in Rho					//
//                                                                      //
// Author: Marcel Kunze, Bochum University, Jan. 2k			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "THashList.h"
#include "RhoManager/TModule.h"
#include "RhoManager/TEventManager.h"
#include "RhoManager/TEventCollection.h"
#include "RhoManager/TEventWriter.h"
#include "RhoManager/TCandListManager.h"
#include "RhoManager/TObjectManager.h"
#include "RhoBase/TRho.h"
#include "RhoBase/TCandList.h"
#include "RhoBase/TCandidate.h"

ClassImp(TModule)

#include <iostream>
using namespace std;

TModule::TModule(const char* const theName, const char* const theDescription) : 
TNamed(theName,theDescription),_passed(kTRUE),fWriter(0),
_verbose("verbose",this),_active("active",this)
{
    _verbose.AddDescription("Verbose mode");
    _active.AddDescription("Module status");
    SetVerbose(kFALSE);
    SetActive(kTRUE);
}

TModule::~TModule()
{
    if (fWriter != 0) {

	for (int i=0;i<fWriter->GetSize();i++) {
	  VAbsWriter *w = (VAbsWriter *) fWriter->At(i);
	  w->Store();
	}

	fWriter->Delete(); delete fWriter;
	fLists->Delete(); delete fLists;
    }
}

void TModule::PrintInfo()
{
    cout << "  - " << GetName() << ':' << GetTitle();
    if (!GetBoolParm("active")) cout << " => not active";
    cout << endl; 
}


void TModule::SetParm(const char* k, Bool_t val, const char* desc) 
{
    TString key(k);
    if (key==GetName()+TString("::verbose")) { SetVerbose(val); return; }
    if (key==GetName()+TString("::active")) { SetActive(val); return; }
    TRho::Instance()->GetParameterManager()->SetParm(key,val,desc,this); 
}

void TModule::SetParm(const char* key, Int_t val, const char* desc) 
{ 
    TRho::Instance()->GetParameterManager()->SetParm(key,val,desc,this); 
}

void TModule::SetParm(const char* key, Double_t val, const char* desc) 
{ 
    TRho::Instance()->GetParameterManager()->SetParm(key,val,desc,this); 
}

void TModule::SetParm(const char* key, const char* val, const char* desc) 
{ 
    TRho::Instance()->GetParameterManager()->SetParm(key,val,desc,this); 
}

Double_t TModule::GetParm(const char* key) 
{ 
    return TRho::Instance()->GetParameterManager()->GetParm(key,this); 
}

Bool_t TModule::GetBoolParm(const char* key) 
{ 
    return TRho::Instance()->GetParameterManager()->GetBoolParm(key,this); 
}

Int_t TModule::GetIntParm(const char* key) 
{ 
    return TRho::Instance()->GetParameterManager()->GetIntParm(key,this); 
}

Double_t TModule::GetDoubleParm(const char* key) 
{ 
    return TRho::Instance()->GetParameterManager()->GetDoubleParm(key,this); 
}

TString TModule::GetStringParm(const char* key) 
{ 
    return TRho::Instance()->GetParameterManager()->GetStringParm(key,this); 
}

void TModule::PrintParm() 
{ 
    TRho::Instance()->GetParameterManager()->PrintOn(cout,this); 
}

void TModule::AddNewParm(const char* p,Bool_t b,TModule *x) 
{
    if (x == 0) x = this;
    TRho::Instance()->GetParameterManager()->SetParm(p,b,"Bool parm",x);
}

void TModule::AddNewParm(const char* p,Int_t i,TModule *x) 
{
    if (x == 0) x = this;
    TRho::Instance()->GetParameterManager()->SetParm(p,i,"Int parm",x);
}

void TModule::AddNewParm(const char* p,Double_t d,TModule *x) 
{
    if (x == 0) x = this;
    TRho::Instance()->GetParameterManager()->SetParm(p,d,"Double parm",x);
}

void TModule::AddNewParm(const char* p,const char* s,TModule *x) 
{
    if (x == 0) x = this;
    TRho::Instance()->GetParameterManager()->SetParm(p,s,"String parm",x);
}

Double_t TModule::GetParmValue(const char* p,TModule *x) 
{
    if (x == 0) x = this;
    return TRho::Instance()->GetParameterManager()->GetParm(p,x);
}

// Communication

TCandList*
TModule::PutCandList(TCandList* list)
{
    // Make sure, list manager exists
    TCandListManager *listmgr = TRho::Instance()->GetCandListManager();
    if (listmgr == 0) {
	listmgr = new TCandListManager();
	TRho *rho = (TRho *) TRho::Instance();
	rho->SetCandListManager(listmgr);
    }

    // Does this list already exist ?
    TString name = list->GetName();
    if (GetCandList(name)!=0) { 
	cerr << "TModule: Trying to override existing TCandList " << name << endl;
	return GetCandList(name);
    }
    else
	return listmgr->Add(list);
}

TCandList*
TModule::GetCandList(const char *name)
{
    // Make sure, list manager exists
    TCandListManager *listmgr = TRho::Instance()->GetCandListManager();
    if (listmgr == 0) {
	listmgr = new TCandListManager();
	TRho *rho = (TRho *) TRho::Instance();
	rho->SetCandListManager(listmgr);
    }

    return listmgr->Get(name);
}


TObject*
TModule::Put(TObject* obj)
{
    // Make sure, list manager exists
    TObjectManager *objmgr = TRho::Instance()->GetObjectManager();
    if (objmgr == 0) {
	objmgr = new TObjectManager();
	TRho *rho = (TRho *) TRho::Instance();
	rho->SetObjectManager(objmgr);
    }

    // Does this list already exist ?
    TString name = obj->GetName();
    if (Get(name)!=0) { 
	cerr << "TModule: Trying to override existing TObject " << name << endl;
	return Get(name);
    }
    else
	return objmgr->Add(obj);
}

TObject*
TModule::Get(const char *name)
{
    // Make sure, list manager exists
    TObjectManager *objmgr = TRho::Instance()->GetObjectManager();
    if (objmgr == 0) {
	objmgr = new TObjectManager();
	TRho *rho = (TRho *) TRho::Instance();
	rho->SetObjectManager(objmgr);
    }

    return objmgr->Get(name);
}

// ------------------- Generate particle selection -------------------------

void
TModule::WriteCandidate(TCandidate *theCand, const char *theMapName,
			       Bool_t useForFilter)
{
    static UInt_t oldEvent   = 0;
    static Bool_t initialize = kTRUE;

    if (initialize) {				// Open the output stream
	if (fWriter==0) fWriter = new THashList();
	if (fLists==0)  fLists  = new THashList();
	initialize = kFALSE;
    }

    // Check if we already have a suiting VAbsWriter for the map
    // Instanciate it if needed and add it to the corresponding list

    VAbsWriter *w = (VAbsWriter *) fWriter->FindObject(theMapName);
    
    if ( w == 0) {
      cout << "TModule::WriteCandidate: Allocate writer for " << theMapName << endl;
      if (useForFilter) {
	TEventCollection *coll = new TEventCollection(theMapName);
	w = new TEventWriter(coll);
      }
      else {
	w = new TEventWriter(theMapName);
      }
      fWriter->Add(w);
    }


    TCandList *l = (TCandList *) fLists->FindObject(theMapName);

    if ( l == 0) {
      cout << "TModule::WriteCandidate: Allocate list for " << theMapName << endl;
      l = new TCandList(theMapName);
      fLists->Add(l);
    }

    // Note the candidate

    l->Add(*theCand);

    // Write the event
    
    TEventManager *evtmgr = TRho::Instance()->GetEventManager();
    if (evtmgr->GetEventNumber() != oldEvent) {
	for (int i=0;i<fWriter->GetSize();i++) {
	  VAbsWriter *w = (VAbsWriter *) fWriter->At(i);
	  TCandList   *l = (TCandList *) fLists->At(i);
	  if (l!=0 && w!=0) {
            cout << "TModule::WriteCandidate: Writer " << w->GetName() << " writes list " << l->GetName() << endl;
	    w->WriteEvent(evtmgr,l);
	    l->Cleanup();
	  }
	  else
	    cerr << "TModule::WriteCandidate: Writer " << w->GetName() << " could not write list " << l->GetName() << endl;
	}
	oldEvent = evtmgr->GetEventNumber();
    }
}

