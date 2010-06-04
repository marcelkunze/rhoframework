#ifndef DSTARAnalysis_h
#define DSTARAnalysis_h
//--------------------------------------------------------------------------
// Program for the selection of clean muon control samples
// class DStarAnalysis
//
// See:
// http://www.slac.stanford.edu/BFROOT/www/Physics/Tools/Pid/Control/Muons/muons.html
//
// Author: Helmut Schmuecker, Bochum University, Aug. 99
//
//------------------------------------------------------------------------

#include "RhoManager/TAnalysis.h"
#include "RhoManager/TModule.h"

class TH1F;
class TEventManager;
class TEventCollection;
class TCandList;

class THistogram;
class TTuple;
class TCandidate;
class VAbsVertexSelector;
class TCanvas;
class TNtuple;

class DStarAnalysis : public TModule
{
public : 
    
    DStarAnalysis(const char* const theName, const char* const theDescription);
    virtual ~DStarAnalysis() { };
    
    //  per Default, TAnalysis provides a Tag-Selector;
    //  however, you may overwrite it to for higher speed :
    //  virtual Bool_t TagSelector(PAFEventBase &eb);
    
    virtual Bool_t BeginJob(TEventManager*);
    virtual Bool_t Event(TEventManager* ev);
    virtual Bool_t EndRun(TEventManager*);
    
    void InitHistograms();
    void DrawHistograms();
    
    void ComputeHardPionCuts();
    
    Bool_t DrcSimpleKaonPid(TCandidate &c);
    
private :
    
    TCanvas *c1;
    
    Double_t hardPiMinP;
    Double_t hardPiMaxP;
    Double_t hardPiPCM;
    Double_t hardPiECM;
    
    
    
    TCandList* chargedList;
    TCandList* piList;
    //TCandList* minusList;
    //TCandList* KsList;
    TCandList* leptonList;
    
    VAbsVertexSelector* vertexer;
    
    TH1F* histMissMass1;
    TH1F* histMissMass2;
    TH1F* histMissMass3;
    TH1F* histMissMass4;
    
    TEventCollection *_coll;
    TTuple* tuppa;
    
    
};


#endif
