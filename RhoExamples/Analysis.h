//////////////////////////////////////////////////////////////////////////
// Analysis
//
// Sample analysis program to create spectra of K0s, JPsi and B
// (inspired by BetaSampleAnalysis)
//
// Author: Sascha Berger and Marcel Kunze, Bochum University
// March '99
//////////////////////////////////////////////////////////////////////////

#include "RhoManager/TModule.h"

class VAbsPidSelector;
class VAbsVertexSelector;
class TCandList;
class TMassHistogram;
class TMomentumHistogram;
class TEoverPHistogram;
class THistogram;
class TCanvas;

class JpsiKs : public TModule
{
private:
    Bool_t graphics;
    
    VAbsPidSelector *electronSelector,
		    *muonSelector,
		    *pionSelector,
		    *kaonSelector,
		    *chargedSelector,
		    *plusSelector,
		    *minusSelector,
		    *ksMassSelector,
		    *jpsiMassSelector;
    
    VAbsVertexSelector	*vertexSelector;
    
    TCandList	    *trackList,
		    *chargedList,
		    *plusList,
		    *minusList,
		    *compositeList,
		    *pionList,
		    *muonList,
		    *ksList,
		    *jpsiRawList,
		    *jpsiList,
		    *bList;
    
    TMassHistogram  *mTrkHist,
		    *mPionHist,
		    *mMuonHist,
		    *mCombinedTrkHist,
		    *mCombinedPionHist,
		    *mCombinedMuonHist,
		    *mBHist;
    
    TMomentumHistogram *pTrkHist,
		    *pCombinedPionHist,
		    *pCombinedMuonHist,
		    *pxBHist,
		    *pyBHist,
		    *pzBHist;
    
    TEoverPHistogram *eoverpHist;
    
    THistogram *deltavse;
    
    TCanvas *c1;
    
public:
    
    JpsiKs(const char *name, const char *desc) : 
      TModule(name,desc) {}
      
    virtual ~JpsiKs() {}
    virtual Bool_t BeginJob(TEventManager *eventmgr);    
    virtual Bool_t EndJob(TEventManager *eventmgr);    
    virtual Bool_t Event(TEventManager *eventmgr);
};
