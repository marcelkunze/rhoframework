// Sample event display program

// Author: Marcel Kunze, Bochum University
// March '99

#define DISPLAY kTRUE
#define FILENAME "$RHO/Data/test/runjpsiks310"

void display(const char *run=FILENAME,const char *collection="", int eventNumber=0, int runNumber=0);

#ifndef __CINT__
#include "TROOT.h"
#include "RhoBase/TRho.h"
#include "RhoBase/VAbsReader.h"
#include "RhoConditions/TConditions.h"
#include "RhoManager/TParameterManager.h"
#include "RhoManager/TEventManager.h"
#include "RhoSelector/TSimpleVertexSelector.h"
#include "RhoTools/TPrintTree.h"
#include "PAFSchema/PAFEventBase.h"
#include "PAFDisplay/PAFDisplay.h"

#include <iostream>
using namespace std;


Int_t main(int argc, char**argv) { return display(); }

#endif

R__EXTERN PAFDisplay *gDisplay;
R__EXTERN TRho *gRho;

void display(const char *run,const char *collection, int eventNumber, int runNumber)
{
#ifdef __CINT__
gROOT->Macro("$RHO/RhoMacros/LoadLibs.C");
#endif

    gRho = new TRho("PAF Event Display");

    gRho->RegisterService(new TConditions("$RHO/Data/CondDB/BaBarConditions.root"));

    // Authorize this client to access the bochum server at SLAC
    TRho::Instance()->Authorize();

    TParameterManager *parmgr = gRho->RegisterService(new TParameterManager());
    parmgr->SetParm("tag",kTRUE);
    parmgr->SetParm("aod",kTRUE);
    parmgr->SetParm("mct",kTRUE);
    parmgr->SetParm("map",kTRUE);
    parmgr->SetParm("cmp",kFALSE);
    parmgr->SetParm("multi",kFALSE);
    parmgr->SetStringParm("Assoc","Map");
    parmgr->SetStringParm("List","");
    parmgr->SetIntParm("TxtFont",0);

    TEventManager *eventmgr = gRho->RegisterService(new TEventManager());

    gRho->PrintOn();

    eventmgr->InitRead(run);

    if (collection != "") eventmgr->UseCollection(collection); // Run on preselected events

    TPrintTree *tree = new TPrintTree;
    tree->AddTerminal("pi0 pi+ pi- K+ K-");
    eventmgr->GetReader()->SetPrintTree(tree);

    eventmgr->NextIter();

    gDisplay = new PAFDisplay(eventmgr,700);

    // Activate a vertex selector (show vertexed tracks)

    TSimpleVertexSelector *gVertexSelector = new TSimpleVertexSelector();
    gVertexSelector->SetDistanceOfClosestApproach(0.01); // 100 micron
    gDisplay->SetVertexSelector(gVertexSelector);
    parmgr->SetParm("Vertexing",kTRUE);

    // Is there a request to skim to a specific run/event ?

    if (eventNumber == 0) return;
    if (runNumber == 0) runNumber = eventmgr->GetRunNumber();

    // Skim the file until we found the event in question

    //eventmgr->DisableAod(); // This is to speed up the search

    cout << "Event #" << eventNumber << " in run #" << runNumber << " is ";

    Int_t oldrun = 0;
    Int_t currentevent = 1;
    do  {
	Int_t rn = eventmgr->GetRunNumber();
	Int_t en = eventmgr->GetEventNumber();
	PAFEventBase &header = eventmgr->GetHeader();
	Int_t stamp = header.GetMajorID();
	if (rn != oldrun) { currentevent = 1; oldrun = rn; }
	currentevent++;
	if (rn != runNumber) continue;
	if (eventNumber>0 && currentevent!=eventNumber) continue;
	if (eventNumber<0 && stamp != eventNumber) continue;
	cout << en << "(" << stamp << "," << header.GetMinorID() << ")" << endl;
	gDisplay->ShowEvent(en);
	return;
    } while (eventmgr->NextIter());

    cout << "not on file. " << endl;
}
