//--------------------------------------------------------------------------
// Program for the selection of clean muon control samples
// class MuMuAnalysis
//
// See:
// http://www.slac.stanford.edu/BFROOT/www/Physics/Tools/Pid/Control/Muons/muons.html
//
// Author: Helmut Schmuecker, Bochum University, Aug. 99
// Mods:   Added support for parameter manager, MK, Nov. 99
// 
// Parameters which can be modified (on the command line):
// 
// Boolean
// -------
// 
// Integer
// -------
// -       LAYERCUT                        10	(Min. number of layers)
// -       STRIPCUT                        10	(Min. number of strips)
// -           BINS                       200	(Binning factor)
// 
// Double
// ------
// -         PVISCM                         9	(scalar sum of momentum magnitudes CMS)
// -        NETPTCM                       0.5	(magnitude of vector sum of momentum vectors CMS)
// -         ACOLCM                      0.01	(PI minus the angle betwenn tracks in 3D CMS)
// -       TRKCOSCM                     0.805	(cos(theta) of track CMS)
// -         EMCCUT                         0	(Min. EMC energy)
// -        MINICUT                       0.5	(Max. EMC energy)
// 
// String
// ------
// -         tagcut _tag._nGoodTrkTight==2 && _tag._nTracks==2 && _tag._totChargeTight==0	(Dynamic cut on TAG)
//------------------------------------------------------------------------
#include "PAFExamples/MuMuAnalysis.hh"

#include "TStyle.h"
#include "TNetFile.h"

#include "RhoManager/TRunDB.h"
#include "RhoManager/TEventCollection.h"

#include "RhoManager/TEventManager.h"
#include "RhoManager/TParameterManager.h"
#include "RhoBase/TCandList.h"
#include "RhoBase/TCandListIterator.h"
#include "RhoBase/TCandidate.h"

#include "RhoHistogram/THistogram.h"
#include "RhoHistogram/TTuple.h" 

#include <iostream>
using namespace std;

#ifndef SHLIB

int main(int argc, char **argv)
{
    MuMuAnalysis myAnal("MuMu_Histo.root",argc,argv);
    
    myAnal.Analyze();
    
    return 0;
}
#endif

MuMuAnalysis::MuMuAnalysis(TString filename,int argc,char** argv) 
: TAnalysis(filename,argc,argv)
{
    nossum=0;
    novsum=0;
    novsumP=0;
    noacol=0;
    noacolP=0;
    nocos=0;
    
    // Set the default parameters

    SetParm("PVISCM",9.0,"scalar sum of momentum magnitudes CMS"); 
    SetParm("NETPTCM",0.5,"magnitude of vector sum of momentum vectors CMS");
    SetParm("ACOLCM",0.01,"PI minus the angle betwenn tracks in 3D CMS");
    SetParm("TRKCOSCM",0.805,"cos(theta) of track CMS");
	
    SetParm("LAYERCUT",10,"Min. number of layers");
    SetParm("STRIPCUT",10,"Min. number of strips");
    SetParm("EMCCUT",0.0,"Min. EMC energy");
    SetParm("MINICUT",0.5,"Max. EMC energy");
	
    SetParm("BINS",200,"Binning factor");
    SetParm("tagcut","_tag._nGoodTrkTight==2 && _tag._nTracks==2 && _tag._totChargeTight==0","Dynamic cut on TAG");
    
    SetParm("cluster","system");
    SetParm("start",5906);
    SetParm("end",5913);

    cout << "Hello muons ! " << endl;
}

void
MuMuAnalysis::BeginJob(TEventManager*)
{
    //_nTrackHist = new TH1F("ntrack","# of charged Tracks",100,0,15);
    //_eopHist = new TH1F("eop","E/p",100,0,1.5);
    
    InitHistograms();
    
    chargedList                = new TCandList();
    
    // If you want to store selected Events in a collection, you
    // should create the Collection-object at the End of this 
    // Method :
    _coll = new TEventCollection("MuMu_Collection.root");

    PVISCM   = GetDoubleParm("PVISCM");
    NETPTCM  = GetDoubleParm("NETPTCM");
    ACOLCM   = GetDoubleParm("ACOLCM");
    TRKCOSCM = GetDoubleParm("TRKCOSCM");
    EMCCUT   = GetDoubleParm("EMCCUT");
    MINICUT  = GetDoubleParm("MINICUT");
    LAYERCUT = GetIntParm("LAYERCUT");
    STRIPCUT = GetIntParm("STRIPCUT");
}

// This is how a Tag-Selector may look like :
/*
Bool_t MuMuAnalysis::TagSelector(PAFEventBase &evb)
{
Int_t n = evb.GetTag().GetNumberOfGoodTracksLoose();
return (n>1000);
}
*/

void
MuMuAnalysis::Event(TEventManager *eventmgr)
{
    // This is the statement to add the current Event
    // to the Collection :
    //ev->AddEventToCollection(_coll);
    
    VAbsTag &tag = eventmgr->GetTag();       // Shortcut to access TAG
    
    TVector3 tmp= tag.GetPrimaryTrackMomentum();
    TVector3 priTrack(0,0,1.0);
    priTrack.SetTheta(TMath::ACos(tmp.X()));
    priTrack.SetPhi(tmp.Y());
    priTrack.SetMag(tmp.Z());
    
    Float_t momScalarSumCMS=tmp.Z();
    
    tmp=tag.GetSecondaryTrackMomentum();
    TVector3 secTrack(0,0,1.0);
    secTrack.SetTheta(TMath::ACos(tmp.X()));
    secTrack.SetPhi(tmp.Y());
    secTrack.SetMag(tmp.Z());
    
    momScalarSumCMS+=tmp.Z();
    
    TVector3 momVectorSumCMSP=(priTrack+secTrack).Perp();  // nonsense in CMS
    TVector3 momVectorSumCMS=(priTrack+secTrack);
    
    
    Float_t acol=(TMath::Pi()-(priTrack.Angle(secTrack)));
    
    Bool_t tagssumcut=kFALSE;
    Bool_t tagvsumcut=kFALSE;
    Bool_t tagvsumcutP=kFALSE;
    Bool_t tagacolcut=kFALSE;
    Bool_t tagcoscut=kFALSE;
    
    
    if(priTrack.CosTheta()<TRKCOSCM && secTrack.CosTheta()<TRKCOSCM){
	tagcoscut=kTRUE;
	nocos++;
	if(momScalarSumCMS>PVISCM){
	    tagssumcut=kTRUE;
	    nossum++;
	    if(momVectorSumCMSP.Mag()<NETPTCM){
		tagvsumcut=kTRUE;
		novsumP++;
		if(acol<ACOLCM){
		    tagacolcut=kTRUE;
		    noacolP++;
		}
	    }
	    if(momVectorSumCMS.Mag()<NETPTCM){
		tagvsumcut=kTRUE;
		novsum++;
		if(acol<ACOLCM){
		    tagacolcut=kTRUE;
		    noacol++;
		}
	    } 
	}
    }
    
    
    
    Bool_t tag2cut=kFALSE;
    if( tagssumcut && tagvsumcut &&  tagacolcut &&  tagcoscut)
	tag2cut=kTRUE;
    
    
    eventmgr->FillCharged(chargedList);
    //chargedList->Boost(-1.0*eventmgr->GetHeader().GetBoost()); //  !!!!!!!!!!!!!!!!!!!!!!
    TCandListIterator iter(*chargedList);
    TCandidate *c;
    
    Float_t momls[2],cosls[2];
    Bool_t ismu[2],isubmu[2],layercut[2],stripcut[2],emccut[2],minicut[2];
    
    Int_t trackno=-1;
    while (c = iter.Next()) {
	
	if (trackno++ >= 2){
	    cerr << "OUCH !!!" << endl;
	    break;
	}
	
	VAbsMicroCandidate &cand = c->GetMicroCandidate();
	
	TVector3 momLS  = cand.GetMomentum(); // Get the momentum
	momls[trackno] = momLS.Mag();
	cosls[trackno] = momLS.CosTheta();
	
	Int_t nifrlayers=cand.GetIfrHitLayers();
	Int_t nifrstrips=cand.GetIfrNumberOfStrips();
	Float_t emce=cand.GetEmcRawEnergy();
	
	layercut[trackno] = nifrlayers>LAYERCUT ? kTRUE : kFALSE;
	
	if(nifrlayers>0)
	    if(nifrstrips<=0){
		stripcut[trackno] = kFALSE;
		cerr << "strange" << endl;
	    }else	
		stripcut[trackno] = nifrstrips/nifrlayers<STRIPCUT ? kTRUE : kFALSE;
	    
	    emccut[trackno] = emce>EMCCUT ? kTRUE : kFALSE;
	    minicut[trackno] = emce<MINICUT ? kTRUE : kFALSE;
	    
	    if( layercut[trackno] && stripcut[trackno] && emccut[trackno] &&  minicut[trackno]){
		ismu[trackno]=kTRUE;
		isubmu[(trackno+1)%2]=kTRUE;
	    }else{
		ismu[trackno]=kFALSE;
		isubmu[(trackno+1)%2]=kFALSE;
	    }
    }
    
    for(int i=0;i<2;++i){ 
	
	TVector3 p=i ? secTrack : priTrack;
	
	//  tuple->Column("x",p.X());
	//     tuple->Column("y",p.Y());
	//     tuple->Column("z",p.Z());
	
	tuple->Column("momLS",momls[i],0.0f);
	tuple->Column("cosLS",cosls[i],0.0f);
	tuple->Column("ssum",momScalarSumCMS,0.0f);
	tuple->Column("vsum",(Float_t)momVectorSumCMS.Mag(),0.0f);
	tuple->Column("acol",acol,0.0f);
	tuple->Column("track",i,0);
	tuple->Column("ismu",char(ismu[i]),0);
	tuple->Column("isubmu",char(isubmu[i]),0);
	tuple->Column("tag2cut",char(tag2cut),0);
	
	tuple->Column("tagssumcut",char(tagssumcut),0);
	tuple->Column("tagvsumcut",char(tagvsumcut),0);
	tuple->Column("tagacolcut",char(tagacolcut),0);
	tuple->Column("tagcoscut",char(tagcoscut),0);
	
	tuple->Column("layercut",char(layercut[i]),0);
	tuple->Column("stripcut",char(stripcut[i]),0);
	tuple->Column("emccut",char(emccut[i]),0);
	tuple->Column("minicut",char(minicut[i]),0);
	
	tuple->DumpData();
	
    }
    
    
    histPVisCMall->Accumulate(momScalarSumCMS);
    histNetptCMall->Accumulate(momVectorSumCMS.Mag());
    histNetptCMallP->Accumulate(momVectorSumCMSP.Mag());
    histacolCMall->Accumulate(acol);
    if(ismu[0] || ismu[1]){
	histPVisCMmumu->Accumulate(momScalarSumCMS);
	histNetptCMmumu->Accumulate(momVectorSumCMS.Mag());
	histNetptCMmumuP->Accumulate(momVectorSumCMSP.Mag());
	histacolCMmumu->Accumulate(acol);
    }
    
    
}

void
MuMuAnalysis::EndJob(TEventManager*)
{
    // "Finish" writes the Collection into the file specified in it's
    //  constructor (here "MyCollection.root") 
    //_coll->Finish();
    
    cout << "MuMu finished ! " << endl;
}

void MuMuAnalysis::InitHistograms()
{
    Int_t BINS = GetIntParm("BINS");

    // Set up 1d-histograms
    
    gStyle->SetHistFillColor(5);
    
    histPVisCMall = new THistogram("PVisCMall","scalar sum of momentum magnitudes (all)",BINS,0,12);
    histPVisCMmumu   = new THistogram("PVisCMmumu","scalar sum of momentum magnitudes (mumu)",BINS,0,12);
    
    histNetptCMallP   = new THistogram("NetptCMallP","magnitude of transverssal momentumsum (all)",BINS,0,5);
    histNetptCMmumuP   = new THistogram("NetptCMmumuP","magnitude of transversal momentumsum (mumu)",BINS,0,5);
    
    histNetptCMall   = new THistogram("NetptCMall","magnitude of vector sum of Momentum vectors (all)",BINS,0,5);
    histNetptCMmumu   = new THistogram("NetptCMmumu","magnitude of vector sum of Momentum vectors (mumu)",BINS,0,5);
    
    histacolCMall   = new THistogram("acolCMall","PI minus the angle between tracks in 3D (all)",BINS,0,0.5);
    histacolCMmumu   = new THistogram("acolCMmumu","PI minus the angle between tracks in 3D (mumu)",BINS,0,0.5);
    
    // Set up ntuple
    
    tuple = new TTuple("mom","lab frame tuple");
    
}


