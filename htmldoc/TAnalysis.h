#ifndef TANALYSIS_H
#define TANALYSIS_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TAnalysis								//
//                                                                      //
// Abstract class to define an analysis framework			//
//                                                                      //
// Author: Thorsten Brandt, Dresden University, Aug. 99			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "TString.h"
#include "RhoBase/TRho.h"
#include "RhoManager/TRunCollection.h"
#include "RhoManager/TAssociator.h"

class TStopwatch;
class TObjArray;
class TFile;

class TEventManager;
class TTreeFormula;
class TDirectoryManager;
class TListManager;
class TModule;
class TConditions;
class TCandList;
class TRunDB;

class TAnalysis : public TObject 
{
public :

    enum AnalysisModes{INVALID=0,RUNS,FILES,COLLECTIONS};
    
    TAnalysis(int argc=0, char **argv=0);
    TAnalysis(const char* output,int argc=0, char **argv=0);
    virtual ~TAnalysis();

    void SetVerbose(Bool_t yesno) { fVerbose = yesno; }
    Bool_t Verbose() { return fVerbose; }

    void SetCommandLine(Int_t argc, char** argv) {fArgc = argc; fArgv = argv; }
    void SetNumberOfEvents(UInt_t);
    void SetInput(UInt_t from,UInt_t to=0);
    void AddFile(const char*);
    void AddCollection(const char*);
    void ClearInput();
    void CheckIndex(const char* path="");
    void SetTagSelection(const char* expr,const char* bitExpr="");
    void SetCluster(const char* name);
    TEventManager& GetEventManager() const { return *TRho::Instance()->GetEventManager(); }
    void SetAssociator(TAssociator& a) { fAssociator = &a; }
    TAssociator& GetAssociator();
    TDirectoryManager& GetDirectoryManager() const { return *TRho::Instance()->GetDirectoryManager(); }
    TParameterManager& GetParameterManager() const { return *TRho::Instance()->GetParameterManager(); }
    TCandList* PutCandList(TCandList *list);
    TCandList* GetCandList(const char *name);
    TObject* Put(TObject *obj);
    TObject* Get(const char *name);

    virtual Bool_t TagSelector();
    virtual void Initialize() {}
    virtual void BeginJob(TEventManager *evtmgr=0);
    virtual void Event(TEventManager *evtmgr);
    virtual void EndJob(TEventManager *evtmgr=0);
    virtual void BeginRun(TEventManager *evtmgr=0);
    virtual void EndRun(TEventManager *evtmgr=0);
    virtual void PrintInfo();

    void Add(TModule *module);

    void Analyze(Long_t nFirst=0,Long_t nLast=1000000000,Bool_t parms = kTRUE);
    void Run() { Analyze(); }

    // Parameter setting

    void SetDefaultParameters();
    void EvaluateParameters();

    void SetParm(const char* key, Bool_t val, const char* desc=0, TModule *x=0);
    void SetBoolParm(const char* key, Bool_t val, const char* desc=0, TModule *x=0);
    void SetParm(const char* key, Int_t val, const char* desc=0, TModule *x=0);
    void SetIntParm(const char* key, Int_t val, const char* desc=0, TModule *x=0);
    void SetParm(const char* key, Double_t val, const char* desc=0, TModule *x=0);
    void SetDoubleParm(const char* key, Double_t val, const char* desc=0, TModule *x=0);
    void SetParm(const char* key, const char* val, const char* desc=0, TModule *x=0);
    void SetStringParm(const char* key, const char* val, const char* desc=0, TModule *x=0);
    Double_t GetParm(const char* key, TModule *x=0);
    Bool_t GetBoolParm(const char* key, TModule *x=0);
    Int_t GetIntParm(const char* key, TModule *x=0);
    Double_t GetDoubleParm(const char* key, TModule *x=0);
    TString GetStringParm(const char* key, TModule *x=0);
    void PrintParm();
    Bool_t ReadParm(const char* file);
    void WriteParm(const char* file);
    void ScanCommandLine();
    void DumpEvent(std::ostream &o=std::cout);

protected:
    void InitializeSelectors();
    Bool_t CheckNewRun();
    Bool_t CheckNewEvent();

private :
    TStopwatch *fTimer;  //! Do not stream
    TAssociator* fAssociator;  //! Do not stream
    TConditions* fConditions; //! Do not stream
    TRunDB *fRunDB;  //! Do not stream
    Int_t   fMode;  //! Do not stream
    TString fFileName;  //! Do not stream
    TFile  *fOut;  //! Do not stream
    TString fCurrentCluster;  //! Do not stream
    UInt_t  fCurrentRun;  //! Do not stream
    UInt_t  fCurrentEvent;  //! Do not stream
    UInt_t  fNumberToProcess;  //! Do not stream
    Bool_t  fVerbose;  //! Do not stream

    TRunCollection *fCollection;  //! Do not stream
    TObjArray *fCollectionList;  //! Do not stream
    TObjArray *fModuleList;  //! Do not stream 
    int fArgc;  //! Do not stream
    char **fArgv; //! Do not stream

    UInt_t fCounter,fAccepted;

    void AnalyzeRuns(Long_t nfirst, Long_t nLast);
    void AnalyzeFiles(Long_t nfirst, Long_t nLast);
    void AnalyzeCollections(Long_t nfirst, Long_t nLast);
  
   public:
    ClassDef(TAnalysis,1) // Analysis base class
};


#endif
