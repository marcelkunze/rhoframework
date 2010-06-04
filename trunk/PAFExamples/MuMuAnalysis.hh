#ifndef MUMUAnalysis_h
#define MUMUAnalysis_h
//--------------------------------------------------------------------------
// Program for the selection of clean muon control samples
// class MuMuAnalysis
//
// See:
// http://www.slac.stanford.edu/BFROOT/www/Physics/Tools/Pid/Control/Muons/muons.html
//
// Author: Helmut Schmuecker, Bochum University, Aug. 99
//
//------------------------------------------------------------------------

#include "RhoManager/TAnalysis.h"

class TH1F;
class TEventManager;
class TEventCollection;
class TCandList;

class THistogram;
class TTuple;


class MuMuAnalysis : public TAnalysis
{
public : 
  
  MuMuAnalysis(TString filename=0,int argc=0,char** argv=0);
  ~MuMuAnalysis() { };
  
  //  per Default, PAFAnalysis provides a Tag-Selector;
  //  however, you may overwrite it to for higher speed :
  //  virtual Bool_t TagSelector(PAFEventBase &eb);
  
  virtual void BeginJob(TEventManager*);
  virtual void Event(TEventManager* ev);
  virtual void EndJob(TEventManager*);
  
  void InitHistograms();
  
private :
  
  TCandList* chargedList;
  
  THistogram* histPVisCMall;
  THistogram* histPVisCMmumu;
  THistogram* histNetptCMallP;
  THistogram* histNetptCMmumuP;
  THistogram* histNetptCMall;
  THistogram* histNetptCMmumu;
  THistogram* histacolCMall;
  THistogram* histacolCMmumu;

  TTuple* tuple;

  // TH1F *_nTrackHist;
  // TH1F *_eopHist;
  TEventCollection *_coll;

  Int_t  nossum;
  Int_t  novsum;
  Int_t novsumP;
  Int_t  noacol;
  Int_t  noacolP;
  Int_t  nocos;
  Int_t  nolayer[2];
  Int_t  nolayer2[2];
  Int_t  nostrip[2];
  Int_t  noemc[2];
  Int_t  nomini[2];


  Double_t PVISCM; // scalar sum of momentum magnitudes CMS 
  Double_t NETPTCM; // magnitude of vector sum of momentum vectors CMS
  Double_t ACOLCM; // PI minus the angle betwenn tracks in 3D CMS
  Double_t TRKCOSCM; // cos(theta) of track CMS

  Int_t LAYERCUT;
  Int_t STRIPCUT;
  Double_t EMCCUT;
  Double_t MINICUT;
  
};


#endif

