#ifndef TMODULE_H
#define TMODULE_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TModule								//
//                                                                      //
// Base class for modules						//
//                                                                      //
// Author: Marcel Kunze, Bochum University, Jan. 2k			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "TNamed.h"
#include "RhoBase/TRho.h"
#include "RhoManager/TParameterManager.h"
#include "RhoManager/TEventManager.h"

class THashList;
class TCandList;
class VAbsTag;
class TCandidate;

class TModule : public TNamed {
public :
    
    TModule(const char* const theName, const char* const theDescription);
    virtual ~TModule();
    
    virtual Bool_t BeginJob(TEventManager *evnt=0) { return kTRUE; }
    virtual Bool_t Event(TEventManager *evnt) { return kTRUE; }
    virtual Bool_t EndJob(TEventManager *evnt=0) { return kTRUE; }
    virtual Bool_t BeginRun(TEventManager *evnt=0) { return kTRUE; }
    virtual Bool_t EndRun(TEventManager *evnt=0) { return kTRUE; }
    virtual void PrintInfo();
    
    // Parameter setting
    
    void SetVerbose(Bool_t yesno) { SetParm("verbose",yesno); _verbose.Set(yesno); }
    Bool_t Verbose() { return GetBoolParm("verbose"); }
    void SetActive(Bool_t yesno) { SetParm("active",yesno); _active.Set(yesno); }
    Bool_t IsActive() { return GetBoolParm("active"); }
    void Enable() { SetActive(kTRUE); }
    void Disable() { SetActive(kFALSE); }
    const char* Name() { return GetName(); }
    
    void SetParm(const char* key, Bool_t val, const char* desc=0);
    void SetParm(const char* key, Int_t val, const char* desc=0);
    void SetParm(const char* key, Double_t val, const char* desc=0);
    void SetParm(const char* key, const char* val, const char* desc=0);
    Double_t GetParm(const char* key);
    Bool_t GetBoolParm(const char* key);
    Int_t GetIntParm(const char* key);
    Double_t GetDoubleParm(const char* key);
    TString GetStringParm(const char* key);
    void PrintParm();
    void AddNewParm(const char* p,Bool_t b,TModule *x=0);
    void AddNewParm(const char* p,Int_t i,TModule *x=0);
    void AddNewParm(const char* p,Double_t d,TModule *x=0);
    void AddNewParm(const char* p,const char* s,TModule *x=0);
    Double_t GetParmValue(const char* p,TModule *x=0);
    
    // Communication

    TCandList* PutCandList(TCandList *list);
    TCandList* GetCandList(const char *name);
    TObject* Put(TObject *obj);
    TObject* Get(const char *name);

    // FilterModule Interface
    Bool_t Passed( ) const { return _passed; }
    void SetPassed( Bool_t flag ) { _passed = flag; }
    VAbsTag* GetTag() { return &TRho::Instance()->GetEventManager()->GetTag(); }
    void WriteCandidate(TCandidate *theCand, const char *theMapName, Bool_t useForFilter = kFALSE);

protected:
    TParmBool _verbose;  //! Do not stream
    TParmBool _active;  //! Do not stream
    Bool_t  _passed;  //! Do not stream 
    
private :
    THashList* fWriter;  //! Do not stream
    THashList* fLists;  //! Do not stream

public:
    ClassDef(TModule,1)  // Module base class

};


#endif
