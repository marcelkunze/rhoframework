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

// Default parameters which can be modified (on the command line):
// Boolean:
// -tag  	true            (Read the Tag)
// -aod  	true            (Read the micro DST)
// -mct  	false           (Read the MC-Truth)
// -multi       false           (Read tag and micro from different files)
// -verbose	false           (Dump events)
// -conditions	false           (Dump conditions database)
// -logbook	false           (Dump electronic logbook)
// Integer:
// -nev		1000000000      (Number of events to process)
// -run		-1              (Run number to process)
// -start	-1              (Range: First run number to process)
// -end		-1              (Range: Last run number to process)
// Double:
// -FixedFieldStrength 1.51007  (Magnet field)
// String:
// -file	""		(Input file)
// -collection	""		(Input collection)
// -tcl		""		(TCL-ish steering file)
// -logtcl	""		(Save parameters (Tcl-like))
// -cluster	""	        (Database cluster to process)
// -tagcut	""		(Cut to apply on tag data)
// -bitcut	""	        (Cut to apply on bit data)
// -store       ""              (Default event service)
// -param       ""              (Default parameter service)
// -RootConditionsFile ""	(Conditions file)
// -pepBeamSpotCalKeys "TwoTracks" (Beamspot calibration)

#include "TNetFile.h"
#include "TTreeFormula.h"
#include "TTreeFormula.h"
#include "TStopwatch.h"
#include "TObjString.h"
#include "TIterator.h"

#include "RhoBase/TRho.h"
#include "RhoBase/TFactory.h"
#include "RhoBase/TCandList.h"
#include "RhoBase/VAbsPidSelector.h"

#include "RhoManager/TAnalysis.h"
#include "RhoManager/TModule.h"
#include "RhoManager/TEventManager.h"
#include "RhoManager/TDirectoryManager.h"
#include "RhoManager/TParameterManager.h"
#include "RhoManager/TCandListManager.h"
#include "RhoManager/TObjectManager.h"

#include "RhoManager/TRunCollection.h"
#include "RhoManager/TEventCollection.h"
#include "RhoManager/TRunDB.h"
#include "RhoManager/TLogInfo.h"

#include "RhoConditions/TConditions.h"

ClassImp(TAnalysis)

#include <iostream>
using namespace std;

TAnalysis::TAnalysis(int argc, char **argv) : 
fArgc(argc), fArgv(argv), fConditions(0), fRunDB(0)
{
    // Instantiate managers if necessary
    
    TRho *appmgr = (TRho *) TRho::Instance();
    cout << *appmgr;

    if (appmgr->GetEventManager()==0) {
	TEventManager *mgr = new TEventManager();
	appmgr->SetEventManager(mgr);
    }

    if (appmgr->GetDirectoryManager()==0) {
	TDirectoryManager *mgr = new TDirectoryManager();
	appmgr->SetDirectoryManager(mgr);
    }

    if (appmgr->GetParameterManager()==0) {
	TParameterManager *mgr = new TParameterManager();
	appmgr->SetParameterManager(mgr);
    }

    fAssociator = 0;
    fCurrentCluster = TString(""); 
    fCollection = new TRunCollection("Collection");
    fMode=INVALID;
    fFileName = TString("");
    fCollectionList = new TObjArray();
    fModuleList = new TObjArray();
    fCurrentRun = 0;
    fCurrentEvent = 0;
    fNumberToProcess = 0;
    fVerbose = kFALSE;
    fOut = 0;
    
    SetDefaultParameters();	    // Defaults
}


TAnalysis::TAnalysis(const char* output,int argc, char **argv) :
fArgc(argc), fArgv(argv), fConditions(0), fRunDB(0)
{
    // Instantiate managers if necessary
    
    TRho *appmgr = (TRho *) TRho::Instance();
    cout << *appmgr;

    if (appmgr->GetEventManager()==0) {
	TEventManager *mgr = new TEventManager();
	appmgr->SetEventManager(mgr);
    }

    if (appmgr->GetDirectoryManager()==0) {
	TDirectoryManager *mgr = new TDirectoryManager();
	appmgr->SetDirectoryManager(mgr);
    }

    if (appmgr->GetParameterManager()==0) {
	TParameterManager *mgr = new TParameterManager();
	appmgr->SetParameterManager(mgr);
    }

    fAssociator = 0;
    fCurrentCluster = TString(""); 
    fCollection = new TRunCollection("Collection");
    fMode=INVALID;
    fFileName = output;
    fCollectionList = new TObjArray();
    fModuleList = new TObjArray();
    fCurrentRun = 0;
    fCurrentEvent = 0;
    fNumberToProcess = 0;
    fVerbose = kFALSE;
    fOut = 0;
    
    SetDefaultParameters();	    // Defaults
}


TAnalysis::~TAnalysis()
{
    if (fRunDB!=0) { delete fRunDB; }
    if (fAssociator!=0) { delete fAssociator; }
    if (fCollection!=0) { delete fCollection; }
    if (fCollectionList!=0) { fCollectionList->Delete(); delete fCollectionList; }
    if (fModuleList!=0) { fModuleList->Delete(); delete fModuleList; }
    if (fTimer!=0) { delete fTimer; }
    if (fOut!=0) { fOut->Close(); delete fOut; }
}

TAssociator& TAnalysis::GetAssociator()
{
    TEventManager *evtmgr = TRho::Instance()->GetEventManager();

    if (fAssociator == 0) {
	fAssociator = new TChi2Associator(*evtmgr);
    }
    
    return *fAssociator;
}

Bool_t TAnalysis::CheckNewRun()
{
    TEventManager *evtmgr = TRho::Instance()->GetEventManager();

    if (fCurrentRun != evtmgr->GetRunNumber()) {
	if (fCurrentRun != 0) EndRun(evtmgr); // Do not at the beginning
	BeginRun(evtmgr);
	fCurrentRun = evtmgr->GetRunNumber();
	return kTRUE;
    }
    
    return kFALSE;
}

Bool_t TAnalysis::CheckNewEvent()
{
    TEventManager *evtmgr = TRho::Instance()->GetEventManager();

    if (fCurrentEvent != evtmgr->GetEventNumber()) {
	fCurrentEvent = evtmgr->GetEventNumber();
	return kTRUE;
    }
    
    return kFALSE;
}


void TAnalysis::BeginJob(TEventManager *evtmgr)
{
    // Make sure eventmanager exists
    if (evtmgr==0) evtmgr = TRho::Instance()->GetEventManager();

    cout << "BeginJob" << endl;
    Bool_t result = kTRUE;
    for (Int_t k=0; k<fModuleList->GetEntries(); k++) {
	TModule *m = (TModule*) (*fModuleList)[k];
	m->PrintInfo();
	if (!m->IsActive()) continue;
	result &= m->BeginJob(evtmgr);
    }

    // Do we need conditions database support ?
    if (GetBoolParm("conditions")) fConditions = TRho::Instance()->GetConditions();

    //return result;
}


void TAnalysis::EndJob(TEventManager *evtmgr)
{
    // Make sure eventmanager exists
    if (evtmgr==0) evtmgr = TRho::Instance()->GetEventManager();

    cout << "EndJob" << endl;
    Bool_t result = kTRUE;
    for (Int_t k=0; k<fModuleList->GetEntries(); k++) {
	TModule *m = (TModule*) (*fModuleList)[k];
	m->PrintInfo();
	if (!m->IsActive()) continue;
	result &= m->EndJob(evtmgr);
    }
    //return result;
}


void TAnalysis::BeginRun(TEventManager *evtmgr)
{
    // Make sure eventmanager exists
    if (evtmgr==0) evtmgr = TRho::Instance()->GetEventManager();

    cout << "SOR #" << evtmgr->GetRunNumber() << endl;
    Bool_t result = kTRUE;
    for (Int_t k=0; k<fModuleList->GetEntries(); k++) {
	TModule *m = (TModule*) (*fModuleList)[k];
	if (!m->IsActive()) continue;
	result &= m->BeginRun(evtmgr);
    }

    // Access conditions database
    if (fConditions != 0 && GetBoolParm("conditions")) {
	TRhoTime theTime(evtmgr->GetTime());
	fConditions->At(theTime); // Read the conditions
	cout << *fConditions;
    }

    //return result;
}


void TAnalysis::EndRun(TEventManager *evtmgr)
{
    // Make sure eventmanager exists
    if (evtmgr==0) evtmgr = TRho::Instance()->GetEventManager();

    cout << "EOR #" << fCurrentRun << endl;
    Bool_t result = kTRUE;
    for (Int_t k=0; k<fModuleList->GetEntries(); k++) {
	TModule *m = (TModule*) (*fModuleList)[k];
	if (!m->IsActive()) continue;
	result &= m->EndRun(evtmgr);
    }

    // Access conditions database
    if (fConditions != 0 && GetBoolParm("conditions")) {
	TRhoTime theTime(evtmgr->GetTime());
	fConditions->At(theTime); // Read the conditions
	cout << *fConditions;
    }

    //return result;
}


void TAnalysis::Event(TEventManager *evtmgr)
{
    Bool_t result = kTRUE;

    // Make sure eventmanager exists
    if (evtmgr==0) evtmgr = TRho::Instance()->GetEventManager();

    // Set the event time
    TRhoTime theTime(evtmgr->GetTime());
    TRho::Instance()->SetTime(&theTime);

    // Access the conditions database
    if (fConditions != 0) {
	fConditions->At(theTime); // Read the conditions
	if (fVerbose) cout << *fConditions;
    }

    // Reset the candidate buffer
    TFactory::Reset();

    for (Int_t k=0; k<fModuleList->GetEntries(); k++) {
	TModule *m = (TModule*) (*fModuleList)[k];
	if (!m->IsActive()) continue;
	m->SetPassed(kTRUE);	    // Preset module status to acceptance
	result &= m->Event(evtmgr);
	if (!result || !m->Passed()) break; // Skip rest of processing
    }
    //return result;
}


void TAnalysis::PrintInfo()
{
    cout << endl; 
    cout << " *** " << fCounter << " Events processed " << endl;
    cout << " *** " << fAccepted << " Events survived TAG cut " << endl;
    cout << endl;
    if (fTimer==0) return;
    cout << " ----- Realtime:   "<<fTimer->RealTime()<<"sec"<<endl;
    cout << " ----- Cputime:    "<<fTimer->CpuTime()<<"sec"<<endl;
    cout << " ----- Time/Event: "<<fTimer->CpuTime()/fCounter<<"sec"<<endl;
    cout << " ----- Speed:      "<<fCounter/fTimer->CpuTime()<<"Hz"<<endl;   
    cout << endl; 
}


void
TAnalysis::CheckIndex(const char* ifp)
{
/*
    TString indexFilePath(ifp);
    // Build a sensible default path in case a path does not exist
    if (indexFilePath == "") indexFilePath = TString(TRho::Instance()->GetEventStore()) + "/" + fCurrentCluster;
    
    // Check, if the index file exists and update, if necessary
    TString index;
    index.Append(indexFilePath);
    index.Append("/Index.root");
    
    //static TFile::Open returns either TFile or TNetFile depending on fcurrentFilename
    TFile* indexFile=TFile::Open(index);
    if (!indexFile->IsOpen()) {
	cout << "CheckIndex: Database directory " << index.Data() << " does not exist." << endl;
	cout << "CheckIndex: Trying to update " << indexFilePath.Data() << endl;
	if (fRunDB==0) fRunDB = new TRunDB(TRho::Instance()->GetEventStore()); 
	fRunDB->Update(indexFilePath);
    }
    else {
	indexFile->Close();
	cout << "CheckIndex: Database directory " << index.Data() << " OK" << endl;
    }
    delete indexFile;
*/
}

void
TAnalysis::SetNumberOfEvents(UInt_t n) 
{
    fNumberToProcess = n;
}

void
TAnalysis::SetInput(UInt_t from, UInt_t to) 
{
    if (fRunDB==0) {
	fRunDB = new TRunDB(TRho::Instance()->GetEventStore()); 
	fRunDB->SetCluster(fCurrentCluster);
    }
    fRunDB->AddRunsToCollection(fCollection,from,to);
    fMode=RUNS;
}


void 
TAnalysis::Add(TModule *module)
{
    fModuleList->Add(module);
    Put(module); // Add it to object inventory
}


void
TAnalysis::AddFile(const char* name)
{
    TRho::Instance()->GetDirectoryManager()->AddFile(name);
    fMode=FILES;
}


void 
TAnalysis::AddCollection(const char* name)
{
    TObjString *t = new TObjString(name);
    fCollectionList->Add(t);
    fMode=COLLECTIONS;
}


void
TAnalysis::ClearInput()
{
    fMode=INVALID;
    if (fCollection!=0) { delete fCollection; fCollection = new TRunCollection(); }
    if (fCollectionList!=0) { fCollectionList->Delete(); }
}


void
TAnalysis::SetTagSelection(const char* expr1,const char* expr2)
{
    TEventManager *evtmgr = TRho::Instance()->GetEventManager();
    if (evtmgr != 0) {
	evtmgr->SetTagFormula(expr1);
	evtmgr->SetBitFormula(expr2);
    }
}


void TAnalysis::SetCluster(const char* name)
{
    fCurrentCluster = name;
    if (fRunDB==0) fRunDB = new TRunDB(TRho::Instance()->GetEventStore()); 
    fRunDB->SetCluster(name);
    //CheckIndex(); // Make sure index exists
}


Bool_t TAnalysis::TagSelector()
{
    TEventManager *evtmgr = TRho::Instance()->GetEventManager();
    return (evtmgr->EvalBitFormula()!=0 && evtmgr->EvalTagFormula()!=0);
}


void
TAnalysis::Analyze(Long_t nFirst, Long_t nLast, Bool_t parms)
{
    TEventManager *evtmgr = TRho::Instance()->GetEventManager();

    // The hierarchy of parameter setting is as follows:
    // 0) Check, whether we have request for parameters
    // 1) Set default parameters (constructor)
    // 2) Override from TCL file
    // 3) Override from command line (highest priority)
    // 4) Override from another TCL file
    
    if (parms) {
        
	Bool_t useCommandLine = (fArgc > 1);

	TString nextFile = GetStringParm("tcl");
	while (nextFile != "" && nextFile != "none") {
	  SetParm("tcl","");
	  ReadParm(nextFile); // Read the Tcl file
	  nextFile = GetStringParm("tcl");
	}

	SetParm("tcl","");
	if (useCommandLine) ScanCommandLine();	// Override parms

	nextFile = GetStringParm("tcl");
	while (nextFile != "" && nextFile != "none") {
	  SetParm("tcl","");
	  ReadParm(nextFile); // Read the Tcl file
	  nextFile = GetStringParm("tcl");
	}

	ScanCommandLine();  // Override parms

	EvaluateParameters();

	PrintParm();							// Dump to screen
    } 

    InitializeSelectors(); // Set the selector criteria

    Initialize(); // For AppModule
    
    // Open the output file for histograms
    
    if (fFileName.Length()>0) { fOut = new TFile(fFileName.Data(),"RECREATE"); fOut->cd(); }
    
    if (fOut!=0) fOut->cd();	    // Switch to the output file
    
    BeginJob(evtmgr);						// Setting of parms etc.
    
    fTimer = new TStopwatch();
    
    switch(fMode) {  
    case RUNS : AnalyzeRuns(nFirst,nLast) ; break;
    case FILES : AnalyzeFiles(nFirst,nLast) ; break;
    case COLLECTIONS : AnalyzeCollections(nFirst,nLast); break;
    default : 
	if (TRho::Instance()->GetDirectoryManager()->GetNumberOfFiles()>0) {
	  cout << *TRho::Instance()->GetDirectoryManager();
	  fMode = FILES;
	  AnalyzeFiles(nFirst,nLast); // Might come from TCL input
	}
	else
	  cout << " NO Input defined " << endl; break;
    }
    
    PrintInfo();
    
    EndRun(evtmgr);
    EndJob(evtmgr);

    if (fOut!=0) {
	fOut->cd();
	fOut->Write();
    }
    
    if (parms && GetStringParm("logtcl")!="") {
	WriteParm(GetStringParm("logtcl"));	// Save the actual settings
    }
}

// _____________________________________________________

void
TAnalysis::AnalyzeRuns(Long_t nFirst, Long_t nLast)
{
    TEventManager *evtmgr = TRho::Instance()->GetEventManager();

    fCollection->InitRead();
    fCollection->PrintOn(cout);

    evtmgr->UseCollection(fCollection);
    
    fCounter  = 0;
    fAccepted = 0;
    
    fTimer->Start();
    while ( (nFirst<=nLast) && (evtmgr->NextIter()) ) {
	
	if (fNumberToProcess>0 && fCounter>=fNumberToProcess) break;
	
	CheckNewRun();
	if (TagSelector()) {
	    Event(evtmgr);
	    fAccepted++;
	    if (fVerbose) cout << *evtmgr;
	}
	fCounter++;
    }
    fTimer->Stop();

}

// ___________________________________________________________________

void TAnalysis::AnalyzeFiles(Long_t nFirst, Long_t nLast)
{
    TEventManager *evtmgr = TRho::Instance()->GetEventManager();

    evtmgr->UseCollection(TRho::Instance()->GetDirectoryManager());
    
    fCounter  = 0;
    fAccepted = 0;
    
    fTimer->Start();
    while ( (nFirst<=nLast) && (evtmgr->NextIter()) ) {
	
	if (fNumberToProcess>0 && fCounter>=fNumberToProcess) break;
	
	CheckNewRun();
	if (TagSelector()) {
	    Event(evtmgr);
	    fAccepted++;
	    if (fVerbose) cout << *evtmgr;
	}
	fCounter++;
    }
    fTimer->Stop();
    
}



// ________________________________________________________



void TAnalysis::AnalyzeCollections(Long_t nFirst, Long_t nLast)
{
    TEventManager *evtmgr = TRho::Instance()->GetEventManager();

    fCounter  = 0;
    fAccepted = 0;
    
    fTimer->Start();
    for (Int_t k=0; k<fCollectionList->GetEntries(); k++) {
	TObjString *t = (TObjString*) (*fCollectionList)[k];
	char *c = (char*)  t->String().Data();
	cout << " Using collection " << t->String().Data() << endl;
	evtmgr->UseCollection(c);
	while ( (nFirst<=nLast) && (evtmgr->NextIter()) ) {
	    
	    if (fNumberToProcess>0 && fCounter>=fNumberToProcess) break;
	    
	    CheckNewRun();
	    if (TagSelector()) {
		Event(evtmgr);
		fAccepted++;
		if (fVerbose) cout << *evtmgr;
	    }
	    fCounter++;
	}
    }
    fTimer->Stop();
    
}


TCandList*
TAnalysis::PutCandList(TCandList* list)
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
	cerr << "TAnalysis: Trying to override existing TCandList " << name << endl;
	return GetCandList(name);
    }
    else
	return listmgr->Add(list);
}

TCandList*
TAnalysis::GetCandList(const char *name)
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
TAnalysis::Put(TObject* obj)
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
	cerr << "TAnalysis: Trying to override existing TObject " << name << endl;
	return Get(name);
    }
    else
	return objmgr->Add(obj);
}

TObject*
TAnalysis::Get(const char *name)
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

void TAnalysis::SetDefaultParameters()
{
    SetParm("tag",kTRUE,"Read the TAG stream");
    SetParm("aod",kTRUE,"Read the micro stream");
    SetParm("mct",kFALSE,"Read the MC-Truth stream");
    SetParm("store",TRho::Instance()->GetEventStore(),"Event store");
    SetParm("param",TRho::Instance()->GetParameterStore(),"Parameter store"); 
    SetParm("tcl","","Tcl-like steering file");
    SetParm("file","","Input file");
    SetParm("collection","","Input collection");
    SetParm("nev",1000000000,"Number of events to process");
    SetParm("run",-1,"Run number to process");
    SetParm("start",-1,"Range: First run number to process");
    SetParm("end",-1,"Range: Last run number to process");
    SetParm("cluster","","Database cluster to process");
    SetParm("tagcut","","Cut to apply on tag data");
    SetParm("bitcut","","Cut to apply on bit data");
    SetParm("verbose",kFALSE,"Dump events");
    SetParm("conditions",kFALSE, "Dump conditions database");
    SetParm("logbook",kFALSE, "Dump electronic logbook");
    SetParm("FixedFieldStrength", 1.51007, "Magnet field");
    SetParm("RootConditionsFile", "","Conditions file");
    SetParm("pepBeamSpotCalKeys", "TwoTracks", "Beamspot calibration");
    SetParm("multi",kFALSE,"Read tag and micro from different files");
    SetParm("readMaps",kFALSE,"Read candidate lists");
    SetParm("readCmpList",kFALSE,"Read composite lists");
    SetParm("logtcl","","Save parameters (Tcl-like)");
}

void TAnalysis::EvaluateParameters()
{
    // Read from a special event store ?
    if (GetStringParm("store") != "") {
	TRho *rho = (TRho *) TRho::Instance();
	rho->SetEventStore(GetStringParm("store"));
    }

    // Read from a special parameter store ?
    if (GetStringParm("param") != "") {
	TRho *rho = (TRho *) TRho::Instance();
        rho->SetParameterStore(GetStringParm("param"));
    }

    // Set the magnetic field
    TRho *rho = (TRho *) TRho::Instance();
    rho->SetMagnetField(GetDoubleParm("FixedFieldStrength"));

    // Read a special conditions DB ?
    if (GetStringParm("RootConditionsFile") != "") {
	const char *file = GetStringParm("RootConditionsFile");
	const char *key  = GetStringParm("pepBeamSpotCalKeys");
	cout << "TAnalysis::EvaluateParameters: Opening conditions from " << file << " with key " << key << endl;
	TConditions *cond = new TConditions(file,key);
        TRho::Instance()->SetConditions(cond);
    }

    SetVerbose(GetBoolParm("verbose"));   // Set debug mode
    
    SetNumberOfEvents(GetIntParm("nev")); // Set the max. number of events
    
    // Define the database cluster
    if (GetStringParm("cluster") != "")
	SetCluster(GetStringParm("cluster"));
    
    // Process a special file
    if (GetStringParm("file") != "") {
	ClearInput(); // Remove all data sources
	AddFile(GetStringParm("file"));
    }
    
    // Process a collection
    if (GetStringParm("collection") != "") {
	ClearInput(); // Remove all data sources
	AddCollection(GetStringParm("collection"));
    }
    
    // Process a special run
    if (GetIntParm("run") >= 0) {
	CheckIndex();			  // Make sure index exists
	SetInput(GetIntParm("run"));
    }
    
    // Process a range of runs
    if (GetIntParm("start") >= 0) {
	CheckIndex();			  // Make sure index exists
	if (GetIntParm("end") < 0)
	    SetInput(GetIntParm("start"),1000000);
	else
	    SetInput(GetIntParm("start"),GetIntParm("end"));
    }
    
    // Install a dynamic cut on the TAG and BIT data
    if (GetStringParm("tagcut") != "" || GetStringParm("bitcut") != "")
	SetTagSelection(GetStringParm("tagcut"),GetStringParm("bitcut"));
}

void TAnalysis::InitializeSelectors()
{
    // Loop through the selectors and set the criteria
    TObjectManager *objmgr = TRho::Instance()->GetObjectManager();
    if (objmgr==0) return;
    TIterator *iter = objmgr->GetListOfObjects()->MakeIterator();
    TObject *o = 0;
    while ((o=iter->Next()) != 0) {
	if (o->InheritsFrom("VAbsPidSelector")) {
	    VAbsPidSelector *s = (VAbsPidSelector *) o;
	    s->Initialize();
	}
    }
}

void TAnalysis::DumpEvent(std::ostream &o) 
{ 
    TEventManager *evtmgr = TRho::Instance()->GetEventManager();
    evtmgr->PrintOn(o); 
}

void TAnalysis::SetParm(const char* k, Bool_t val, const char* desc, TModule *x) 
{
    TString key(k);
    if (key=="verbose") SetVerbose(val);
    TRho::Instance()->GetParameterManager()->SetParm(key,val,desc,x); 
}

void TAnalysis::SetParm(const char* key, Int_t val, const char* desc, TModule *x) 
{ 
    TRho::Instance()->GetParameterManager()->SetParm(key,val,desc,x); 
}

void TAnalysis::SetParm(const char* key, Double_t val, const char* desc, TModule *x) 
{ 
    TRho::Instance()->GetParameterManager()->SetParm(key,val,desc,x); 
}

void TAnalysis::SetParm(const char* key, const char* val, const char* desc, TModule *x) 
{ 
    TRho::Instance()->GetParameterManager()->SetParm(key,val,desc,x); 
}

void TAnalysis::SetBoolParm(const char* key, Bool_t val, const char* desc, TModule *x) { SetParm(key,val,desc,x); }
void TAnalysis::SetIntParm(const char* key, Int_t val, const char* desc, TModule *x) { SetParm(key,val,desc,x); }
void TAnalysis::SetDoubleParm(const char* key, Double_t val, const char* desc, TModule *x) { SetParm(key,val,desc,x); }
void TAnalysis::SetStringParm(const char* key, const char* val, const char* desc, TModule *x) { SetParm(key,val,desc,x); }

Double_t TAnalysis::GetParm(const char* key, TModule *x) 
{ 
    return TRho::Instance()->GetParameterManager()->GetParm(key,x); 
}

Bool_t TAnalysis::GetBoolParm(const char* key, TModule *x) 
{ 
    return TRho::Instance()->GetParameterManager()->GetBoolParm(key,x); 
}

Int_t TAnalysis::GetIntParm(const char* key, TModule *x) 
{ 
    return TRho::Instance()->GetParameterManager()->GetIntParm(key,x); 
}

Double_t TAnalysis::GetDoubleParm(const char* key, TModule *x) 
{ 
    return TRho::Instance()->GetParameterManager()->GetDoubleParm(key,x); 
}

TString TAnalysis::GetStringParm(const char* key, TModule *x) 
{ 
    return TRho::Instance()->GetParameterManager()->GetStringParm(key,x); 
}

void TAnalysis::PrintParm() 
{ 
    TRho::Instance()->GetParameterManager()->PrintOn(cout); 
}

Bool_t TAnalysis::ReadParm(const char* file) 
{ 
    return TRho::Instance()->GetParameterManager()->ReadParm(file); 
}

void TAnalysis::WriteParm(const char* file) 
{ 
    TRho::Instance()->GetParameterManager()->WriteParm(file); 
}

void TAnalysis::ScanCommandLine() 
{ 
    TRho::Instance()->GetParameterManager()->ScanCommandLine(fArgc,fArgv); 
}

