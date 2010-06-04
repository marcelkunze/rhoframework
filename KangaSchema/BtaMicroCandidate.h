#ifndef BTAMICROCANDIDATE_H 
#define BTAMICROCANDIDATE_H 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// BtaMicroCandidate	                                                //
//                                                                      //
// Implementation of the abstract interface AbsMicroCanidate		//
// for an underlying Kanga BtaMicroCanidateR object.			//
//									//
// Author List:								//
// Marcel Kunze,  RUB, Nov. 99						//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include <iosfwd>
#include <assert.h> 
 
#include "TObject.h" 
#include "TVector3.h" 
#include "TLorentzVector.h" 
 
#include "RhoBase/VAbsPidInfo.h"
#include "RhoBase/VAbsMicroCandidate.h" 
#include "KangaSchema/TBtaMicroCandR.h" 
 
//  ============================================================================ 
//  ===== BtaMicroCandidate - Defining data access to Beta style candidates ==== 
//  ============================================================================ 
 
class BtaMicroCandidate : public VAbsMicroCandidate 
{ 
     
public: 
     
    // empty constructor 
    BtaMicroCandidate() : fBtaMicroCandR(0) {} 
    BtaMicroCandidate(TBtaMicroCandR* bmc) { fBtaMicroCandR= bmc; } 
     
    virtual ~BtaMicroCandidate() {}; 
     
    // use default destruktor 
    // use default copy constructor 
     
    virtual void  SetBtaMicroCandR(TBtaMicroCandR &bmc) { fBtaMicroCandR = &bmc; } 
    virtual const TBtaMicroCandR& GetBtaMicroCandR() const { return *fBtaMicroCandR; } 
     
    // basic properties: 
     
    virtual Int_t GetCharge() const { return fBtaMicroCandR->GetFitParams()->GetQ(); } 
     
    virtual TVector3 GetPosition() const {  
	return fBtaMicroCandR->GetFitParams()->GetPos(); 
    } 
     
    virtual TVector3 GetMomentum() const {  
	return fBtaMicroCandR->GetFitParams()->GetP(); 
    } 
     
    virtual TLorentzVector GetLorentzVector() const { 
	return fBtaMicroCandR->GetFitParams()->GetP4(); 
    } 
     
    virtual Float_t GetEnergy() const { 
	return (fBtaMicroCandR->GetFitParams()->GetP4()).E(); 
    } 
     
    virtual const Float_t* GetParams() { 
      return fBtaMicroCandR->GetFitParams()->GetParams(); 
    } 
     
    virtual const Float_t* GetPositionError() { 
      return fBtaMicroCandR->GetFitParams()->GetCov7(); 
    } 
     
    virtual const Float_t* GetMomentumError() {  
      return &(fBtaMicroCandR->GetFitParams()->GetCov7())[18]; 
    } 
     
    virtual const Float_t* GetErrorP7() { 
	return fBtaMicroCandR->GetFitParams()->GetCov7(); 
    } 
     
    virtual const Float_t* GetCov() { 
	return fBtaMicroCandR->GetFitParams()->GetCov(); 
    } 
     
    virtual Int_t   GetMcPid() const { return fMcHypo ; } 
    virtual Int_t   GetMcIndex() const { return fMcIndex ; } 
    virtual Float_t GetMcConsistency() const { return fMcConsistency ; } 
     
    // calorimeter properties: 
     
    virtual Bool_t EmcOk() const { assert(0); return 0; } 
    virtual Bool_t IfrOk() const { assert(0); return 0; } 
     
    virtual Int_t GetEmcStatus() const
    { return (Int_t) fBtaMicroCandR->GetCalQual()->GetStatus(); } 
    virtual Int_t   GetEmcNumberOfCrystals() const  
    { return fBtaMicroCandR->GetCalQual()->GetNCrystals(); } 
    virtual Int_t   GetEmcNumberOfBumps() const  
    { return fBtaMicroCandR->GetCalQual()->GetNBumps(); } 
    virtual Float_t GetEmcRawEnergy() const  
    { return fBtaMicroCandR->GetCalQual()->GetRawEnergy(); } 
    virtual Float_t GetEmcCalEnergy() const
    { return fBtaMicroCandR->GetCalQual()->GetEnergy(); } 
    virtual Float_t GetEmcCovarianceEnergy() const  
    { Float_t cov[10]; fBtaMicroCandR->GetCalQual()->GetCov(cov); return cov[0]; } 
    virtual Float_t GetEmcCovarianceTheta() const  
    { Float_t cov[10]; fBtaMicroCandR->GetCalQual()->GetCov(cov); return cov[2]; } 
    virtual Float_t GetEmcCovariancePhi() const  
    { Float_t cov[10]; fBtaMicroCandR->GetCalQual()->GetCov(cov); return cov[5]; } 
    virtual Float_t GetEmcCovarianceRho() const  
    { Float_t cov[10]; fBtaMicroCandR->GetCalQual()->GetCov(cov); return cov[9]; } 
    virtual Float_t GetEmcS9S25() const  
    { return fBtaMicroCandR->GetCalQual()->GetS9S25(); } 
    virtual Float_t GetEmcS1S9() const  
    { return fBtaMicroCandR->GetCalQual()->GetS1S9(); } 
    virtual Float_t GetEmcLateralShape() const  
    { return fBtaMicroCandR->GetCalQual()->GetLat(); } 
    virtual Float_t GetEmcSecondMoment() const  
    { return fBtaMicroCandR->GetCalQual()->GetSecondMomentTP(); } 
    virtual Float_t GetEmcZernikeMoment20() const  
    { return fBtaMicroCandR->GetCalQual()->GetAbsZern20(); } 
    virtual Float_t GetEmcZernikeMoment31() const  
    { return fBtaMicroCandR->GetCalQual()->GetAbsZern31(); } 
    virtual Float_t GetEmcZernikeMoment42() const  
    { return fBtaMicroCandR->GetCalQual()->GetAbsZern42(); } 
    virtual TVector3 GetEmcCentroid() const  
    { return TVector3(fBtaMicroCandR->GetCalQual()->GetCentroidX(), 
    fBtaMicroCandR->GetCalQual()->GetCentroidY(), 
    fBtaMicroCandR->GetCalQual()->GetCentroidZ()); } 
    virtual Float_t  GetEmcConsistencyValue() const   
    { return fBtaMicroCandR->GetCalQual()->GetCdPhiValue();} 
    virtual Float_t  GetEmcConsistencyLikelihood() const   
    { return fBtaMicroCandR->GetCalQual()->GetCdPhiLhood();} 
    virtual Int_t    GetEmcConsistencyBits() const   
    { return fBtaMicroCandR->GetCalQual()->GetCdPhiBits();} // 8.3.x 
    virtual Int_t   GetNeutralPidHypo() const  
    { return fBtaMicroCandR->GetCalQual()->GetPidHypo(); } 
     
    // IFR properties

    virtual Int_t   GetIfrPattern() const  
    { return fBtaMicroCandR->GetIfrQual()->GetIfrBits(); } 
    virtual Int_t   GetIfrNumberOfStrips() const  
    { return fBtaMicroCandR->GetIfrQual()->GetIfrNstrip(); } 
     
    virtual Int_t   GetIfrNumberOfStrips(Int_t layer) const {  
	UShort_t LayStrips[20]; 
	fBtaMicroCandR->GetIfrQual()->GetIfrLayStrips(LayStrips); 
	return LayStrips[layer]; 
    } 
     
    virtual Int_t   GetIfrFirstHit() const;  
    virtual Int_t   GetIfrLastHit() const; 
    virtual Int_t   GetIfrHitLayers() const 
    { return 0;} 
    virtual Float_t GetIfrMeasuredInteractionLengths() const  
    { return fBtaMicroCandR->GetIfrQual()->GetMeasuredInteractionLengths(); } 
    virtual Float_t GetIfrExpectedInteractionLengths() const  
    { return fBtaMicroCandR->GetIfrQual()->GetExpectedInteractionLengths(); } 
    virtual Float_t GetIfrInteractionLengthsBeforeIron() const 
    { return fBtaMicroCandR->GetIfrQual()->GetInteractionLengthsBeforeIron(); } 
    virtual Float_t GetIfrTrackMatch() const 
    { return fBtaMicroCandR->GetIfrQual()->GetIfrTrkMatch(); } 
    virtual Float_t GetIfrEmcMatch() const 
    { return fBtaMicroCandR->GetIfrQual()->GetIfrEmcMatch(); }       
    virtual Float_t GetIfrClusterFit() const
    { return fBtaMicroCandR->GetIfrQual()->GetIfrClusterFit(); }       
    virtual Float_t GetIfrRpcMuonProbability() const
    { return fBtaMicroCandR->GetIfrQual()->GetIfrRpcMuonProbability(); }       

    virtual Int_t    GetIfrLastBarrel() const  
	{ return fBtaMicroCandR->GetIfrQual()->GetLastBarrel(); } // 8.3.x 
     
    // track properties: 
     
    virtual Bool_t SvtOk() const { assert(0); return 0; } 
    virtual Bool_t DchOk() const { assert(0); return 0; } 
    virtual Bool_t DrcOk() const { assert(0); return 0; } 
     
    virtual Float_t GetTrackLength() const  
    { return fBtaMicroCandR->GetTrkQual()->GetTLen(); } 
    virtual Float_t GetTrackStartFoundRange() const  
    { return fBtaMicroCandR->GetTrkQual()->GetStFound(); } 
    virtual Int_t   GetSvtHitPattern() const  
    { return fBtaMicroCandR->GetTrkQual()->GetSvtPattern(); } 
    virtual Int_t   GetSvtHits() const  
    { return fBtaMicroCandR->GetTrkQual()->GetNSvtHits(); } 
    virtual Int_t   GetDchInnerLayer() const  
    { return fBtaMicroCandR->GetTrkQual()->GetFirstDch(); } 
    virtual Int_t   GetDchOuterLayer() const  
    { return fBtaMicroCandR->GetTrkQual()->GetLastDch(); } 
    virtual Int_t   GetDchHits() const  
    { return fBtaMicroCandR->GetTrkQual()->GetNDchHits(); } 
    virtual Int_t   GetChargedPidHypo() const  
    { return fBtaMicroCandR->GetTrkQual()->GetPidHypo(); } 
    virtual Int_t   GetDegreesOfFreedom() const  
    { return fBtaMicroCandR->GetTrkQual()->GetNDof(); } 
    virtual Int_t   GetFitStatus() const  
    { return fBtaMicroCandR->GetTrkQual()->GetFitStatus(); } 
    virtual Float_t GetChiSquared() const  
    { return fBtaMicroCandR->GetTrkQual()->GetChi2(); } 
    virtual Float_t  GetProbability() const 
    { /* cout << "GetProbability() needs implementation" << endl;*/ return 0; } // Again in 8.6.x

    // Pid quality (needs to be updated to new 8.5.x schema !!!)

    virtual Float_t GetDrcThetaC() const  
    { return fBtaMicroCandR->GetPidQual()->GetThetaDirc(); } 
    virtual Float_t GetDrcThetaCError() const  
    { return fBtaMicroCandR->GetPidQual()->GetEThetaDirc(); } 
    virtual Int_t   GetDrcNumberOfPhotons() const  
    { return fBtaMicroCandR->GetPidQual()->GetNPhot(); } 
    virtual Int_t   GetDrcNumberOfBackgroundPhotons() const  
    { return fBtaMicroCandR->GetPidQual()->GetNBkgd(); } 
    virtual Int_t   GetDrcNumberOfExpectedPhotons(Int_t hypo) const 
    { return fBtaMicroCandR->GetPidQual()->GetNExPhot(hypo); } 
    virtual Float_t  GetDchDeltaMomentum() const 
    { return fBtaMicroCandR->GetPidQual()->GetDeltaDchMom(); } 
    virtual Int_t   GetDrcInBar() const
    { return fBtaMicroCandR->GetPidQual()->GetDrcInBar(); } 
    virtual Int_t   GetDrcExitBar() const
    { return fBtaMicroCandR->GetPidQual()->GetDrcExitBar(); } 
    virtual Float_t  GetDrcXPos() const
    { return fBtaMicroCandR->GetPidQual()->GetDrcXPos(); } 
    virtual Float_t  GetDrcPhiAtEmc() const
    { return fBtaMicroCandR->GetPidQual()->GetDrcPhiAtEmc(); } 
     virtual Float_t  GetDrcThetaAtEmc() const
    { return fBtaMicroCandR->GetPidQual()->GetDrcThetaAtEmc(); } 
    virtual Float_t  GetDrcDeltaMomentum() const
    { return fBtaMicroCandR->GetPidQual()->GetDeltaDrcMom(); } 
    virtual Float_t GetDchMeanDEdx() const  
    { return fBtaMicroCandR->GetPidQual()->GetDEdXDch(); } 
    virtual Float_t GetSvtMeanDEdx() const  
    { return fBtaMicroCandR->GetPidQual()->GetDEdXSvt(); } 
    virtual Int_t    GetDchSamples() const  
    { return fBtaMicroCandR->GetPidQual()->GetNSamplesDch(); } 
    virtual Int_t    GetSvtSamples() const 
    { return fBtaMicroCandR->GetPidQual()->GetNSamplesSvt(); } 
     
    // Access to PID information
    virtual VAbsPidInfo& GetDrcPidInfo() const 
    { return GetAbsPidInfo(PidSystem::drc); }
    virtual VAbsPidInfo& GetDchPidInfo() const 
    { return GetAbsPidInfo(PidSystem::dch); }
    virtual VAbsPidInfo& GetSvtPidInfo() const 
    { return GetAbsPidInfo(PidSystem::svt); }
    virtual VAbsPidInfo& GetEmcPidInfo() const 
    { return GetAbsPidInfo(PidSystem::emc); }
    virtual VAbsPidInfo& GetIfrPidInfo() const 
    { return GetAbsPidInfo(PidSystem::ifr); }

    VAbsPidInfo& GetAbsPidInfo(PidSystem::System sys) const;

    // Modifiers (for event display) 
     
    virtual void    SetPosition(TVector3 x) {  } 
    virtual void    SetMcPid(Int_t pid) { fMcHypo = pid; } 
    virtual void    SetMcIndex(Int_t index) { fMcIndex = index; } 
    virtual void    SetMcConsistency(Float_t cons) { fMcConsistency = cons; } 
     
private : 
    TBtaMicroCandR *fBtaMicroCandR;   //! Do not stream 
    Int_t	   fMcHypo, fMcIndex;
    Float_t	   fMcConsistency;

     
public : 
    ClassDef(BtaMicroCandidate,1)  //PAF Kanga implementation of the MicroCandidate
	 
}; 

ostream&  operator << (ostream& o, const BtaMicroCandidate&); 

#endif                                            
 
 
