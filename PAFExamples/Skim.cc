//--------------------------------------------------------------------------
// Analysis example class in PAF(Pico Analysis Framework)
// class Skim
//
// Write out skim files
//
// Author: M. Kunze, Bochum University, Apr. 00
//
//------------------------------------------------------------------------

#include "PAFExamples/Skim.hh"
#include "RhoBase/TRho.h"
#include "RhoManager/TAnalysis.h"
#include "RhoManager/TParameterManager.h"
#include "RhoManager/TEventCollection.h"
#include "RhoManager/TEventWriter.h"

#include <iostream>
using namespace std;

Skim::Skim(const char* const theName, const char* const theDescription) : 
TModule(theName, theDescription) 
{
    SetParm("skim",kTRUE,"Perform a skim");
    SetParm("collection",kFALSE,"Output a skim collection");
    SetParm("name","Skim","Name of skim file");
}

Skim::~Skim() 
{
    delete fWriter;
}

Bool_t Skim::BeginJob(TEventManager* evtmgr) 
{
    if (GetBoolParm("skim")) {
	TString name = GetStringParm("name");
	if (GetBoolParm("collection")) {
	    TEventCollection *coll = new TEventCollection(name+".root");
	    fWriter = new TEventWriter(coll);
	}
	else {
	    fWriter = new TEventWriter(name);
	}
    }

    return kTRUE;
}

//start the Analysis

Bool_t Skim::Event(TEventManager *eventmgr) 
{       
    if (Verbose()) {
	cout<<"Accepted event "<<eventmgr->GetEventNumber()<<endl;
	for (int i=0;i<eventmgr->GetNumberOfLists();i++)
	    cout << "\t" << eventmgr->GetListName(i) << endl;
    }
    if ( GetBoolParm("skim")) eventmgr->WriteEvent(fWriter);
    return kTRUE;
}

Bool_t Skim::EndJob(TEventManager* eventmgr) 
{
    if (GetBoolParm("skim")) {
	fWriter->Store();
    }
    return kTRUE;
}

// This little main program drives the sample analysis
// If a network file is given, data is fetched from the server
// Initialization for the SLAC service:
// setenv PAFROOT root://bbr-bochum2/PAF/

int main(int argc,char* argv[])
{
    // Create a named Framework and register services
    TRho PAF("Sample Analysis");
    
    // Instantiate the analysis and add a sample module
    // Parameter presets can be overriden from file or command line
    TAnalysis *analysis = new TAnalysis(argc,argv);
    analysis->SetParm("tcl","Skim.tcl"); // Read a TCL file
    
    Skim *theModule = new Skim("Skim","Sample Analysis Module");
    analysis->Add(theModule);
    
    analysis->SetParm("tagcut","");  // Do not apply fast tag selection
    
    PAF.RegisterService(analysis);   // The sample analysis
    
    PAF.GetAnalysis()->Run(); // Execute the analysis
    
    return 0;
}

