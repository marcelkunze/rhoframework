#ifndef PAFMICROCANDIDATE_H
#define PAFMICROCANDIDATE_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFMicroCandidate							//
//                                                                      //
// Implementation of the PAF Micro candidate	    			//
//                                                                      //
// Author: Marcel Kunze, RUB, Mar 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include <iosfwd>
#include <assert.h>

#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"

#include "PAFSchema/PAFAodBase.h"
#include "PAFSchema/PAFAodCluster.h"
#include "PAFSchema/PAFAodTrack.h"
#include "RhoBase/VAbsPidInfo.h"
#include "RhoBase/VAbsMicroCandidate.h"

//  ========================================================================
//  ===== PAFMicroCandidate - Defining data access to PAF candidates    ====
//  ========================================================================

class PAFMicroCandidate : public VAbsMicroCandidate
{

public:
    
    // empty constructor
    PAFMicroCandidate() : fAodBase(0),fAodCluster(0),fAodTrack(0) {}
    PAFMicroCandidate(PAFAodBase*,PAFAodCluster*,PAFAodTrack*);

    virtual ~PAFMicroCandidate() {};
    
    // use default destruktor
    // use default copy constructor

    virtual void SetAodBase(PAFAodBase &a) { fAodBase = &a; }
    virtual void SetAodCluster(PAFAodCluster &a) { fAodCluster = &a; }
    virtual void SetAodTrack(PAFAodTrack &a) { fAodTrack = &a; }
    virtual const PAFAodBase& GetAodBase() const { return *fAodBase; }
    virtual const PAFAodCluster& GetAodCluster() const { return *fAodCluster; }
    virtual const PAFAodTrack& GetAodTrack() const { return *fAodTrack; }

    // basic properties:

    virtual Int_t GetCharge() const { return fAodBase->GetCharge(); }
    virtual TVector3 GetPosition() const { return fAodBase->GetPosition(); }
    virtual TVector3 GetMomentum() const { return fAodBase->GetMomentum(); }
    virtual TLorentzVector GetLorentzVector() const { return fAodBase->GetLorentzVector(); }
    virtual Float_t GetEnergy() const { return fAodBase->GetEnergy(); }
    virtual const Float_t* GetParams() { return 0; }
    virtual const Float_t* GetPositionError() { return fAodBase->GetPositionError(); }
    virtual const Float_t* GetMomentumError() { return fAodBase->GetMomentumError(); }
    virtual const Float_t* GetErrorP7() { return fAodBase->GetErrorP7(); }
    virtual const Float_t* GetCov() { return 0; }
    virtual Int_t  GetMcPid() const { return fAodBase->GetMcPid(); }			    
    virtual Int_t  GetMcIndex() const { return fAodBase->GetMcIndex(); } 
    virtual Float_t  GetMcConsistency() const { return fAodBase->GetMcConsistency(); } 

    // calorimeter properties:

    virtual Bool_t EmcOk() const { return fAodCluster->EmcOk(); }
    virtual Bool_t IfrOk() const { return fAodCluster->IfrOk(); }

    virtual Int_t   GetEmcStatus() const { return fAodCluster->GetEmcStatus(); } 
    virtual Int_t   GetEmcNumberOfCrystals() const { return fAodCluster->GetEmcNumberOfCrystals(); }
    virtual Int_t   GetEmcNumberOfBumps() const { return fAodCluster->GetEmcNumberOfBumps(); }
    virtual Float_t GetEmcRawEnergy() const { return fAodCluster->GetEmcRawEnergy(); }
    virtual Float_t GetEmcCalEnergy() const { return fAodCluster->GetEmcCalEnergy(); } 
    virtual Float_t GetEmcCovarianceEnergy() const { return fAodCluster->GetEmcCovarianceEnergy(); }
    virtual Float_t GetEmcCovarianceTheta() const { return fAodCluster->GetEmcCovarianceTheta(); }
    virtual Float_t GetEmcCovariancePhi() const { return fAodCluster->GetEmcCovariancePhi(); }
    virtual Float_t GetEmcCovarianceRho() const { return fAodCluster->GetEmcCovarianceRho(); }
    virtual Float_t GetEmcS9S25() const { return fAodCluster->GetEmcS9S25(); }
    virtual Float_t GetEmcS1S9() const { return fAodCluster->GetEmcS1S9(); }
    virtual Float_t GetEmcLateralShape() const { return fAodCluster->GetEmcLateralShape(); }
    virtual Float_t GetEmcSecondMoment() const { return fAodCluster->GetEmcSecondMoment(); }
    virtual Float_t GetEmcZernikeMoment20() const { return fAodCluster->GetEmcZernikeMoment20(); }
    virtual Float_t GetEmcZernikeMoment31() const { return fAodCluster->GetEmcZernikeMoment31(); }
    virtual Float_t GetEmcZernikeMoment42() const { return fAodCluster->GetEmcZernikeMoment42(); }
    virtual TVector3 GetEmcCentroid() const { return fAodCluster->GetEmcCentroid(); }
    virtual Float_t GetEmcConsistencyValue() const { return fAodCluster->GetEmcConsistencyValue(); }
    virtual Float_t GetEmcConsistencyLikelihood() const { return fAodCluster->GetEmcConsistencyLikelihood(); }
    virtual Int_t   GetEmcConsistencyBits() const { return fAodCluster->GetEmcConsistencyBits(); }
    virtual Int_t   GetNeutralPidHypo() const { return fAodCluster->GetNeutralPidHypo(); }

    virtual Int_t   GetIfrPattern() const { return fAodCluster->GetIfrPattern(); }
    virtual Int_t   GetIfrHitLayers() const { return fAodCluster->GetIfrHitLayers(); }
    virtual Int_t   GetIfrNumberOfStrips() const { return fAodCluster->GetIfrNumberOfStrips(); }
    virtual Int_t   GetIfrNumberOfStrips(Int_t layer) const { return fAodCluster->GetIfrNumberOfStrips(layer); }
    virtual Int_t   GetIfrFirstHit() const { return fAodCluster->GetIfrFirstHit(); }
    virtual Int_t   GetIfrLastHit() const { return fAodCluster->GetIfrLastHit(); }
    virtual Int_t   GetIfrClusters() const { return fAodCluster->GetIfrClusters(); }
    virtual Float_t GetIfrMeasuredInteractionLengths() const { return fAodCluster->GetIfrMeasuredInteractionLengths(); }
    virtual Float_t GetIfrExpectedInteractionLengths() const { return fAodCluster->GetIfrExpectedInteractionLengths(); }
    virtual Float_t GetIfrInteractionLengthsBeforeIron() const { return fAodCluster->GetIfrInteractionLengthsBeforeIron(); }
    virtual Float_t GetIfrTrackMatch() const { return fAodCluster->GetIfrTrackMatchConsistency(); }
    virtual Float_t GetIfrEmcMatch() const { return fAodCluster->GetIfrEmcMatchConsistency(); }       
    virtual Float_t GetIfrClusterFit() const { return fAodCluster->GetIfrClusfitChi2(); }
    virtual Float_t GetIfrRpcMuonProbability() const { return fAodCluster->GetIfrRpcMuProb(); } 
    virtual Int_t   GetIfrLastBarrel() const  { return fAodCluster->GetIfrLastBarrel(); }

    // track properties:

    virtual Bool_t SvtOk() const { return fAodTrack->SvtOk(); }
    virtual Bool_t DchOk() const { return fAodTrack->DchOk(); }
    virtual Bool_t DrcOk() const { return fAodTrack->DrcOk(); }

    virtual Float_t GetTrackLength() const { return fAodTrack->GetTrackLength(); }
    virtual Float_t GetTrackStartFoundRange() const { return fAodTrack->GetTrackStartFoundRange(); }
    virtual Int_t   GetSvtHitPattern() const { return fAodTrack->GetSvtHitPattern(); }
    virtual Int_t   GetSvtHits() const { return fAodTrack->GetSvtHits(); }
    virtual Int_t   GetDchInnerLayer() const { return fAodTrack->GetDchInnerLayer(); }
    virtual Int_t   GetDchOuterLayer() const { return fAodTrack->GetDchOuterLayer(); }
    virtual Int_t   GetDchHits() const { return fAodTrack->GetDchHits(); }
    virtual Int_t   GetChargedPidHypo() const { return fAodTrack->GetChargedPidHypo(); }
    virtual Int_t   GetDegreesOfFreedom() const { return fAodTrack->GetDegreesOfFreedom(); }
    virtual Int_t   GetFitStatus() const { return fAodTrack->GetFitStatus(); }
    virtual Float_t GetProbability() const { return fAodTrack->GetProbability(); }
    virtual Float_t GetChiSquared() const { return fAodTrack->GetChiSquared(); }

    virtual Float_t GetDrcThetaC() const { return fAodTrack->GetDrcThetaC(); }
    virtual Float_t GetDrcThetaCError() const { return fAodTrack->GetDrcThetaCError(); }
    //virtual Float_t GetDrcRingProbability() const { return fAodTrack->GetDrcRingProbability(); }
    //virtual Int_t   GetDrcNumberDegreesOfFreedom() const { return fAodTrack->GetDrcNumberDegreesOfFreedom(); }
    virtual Int_t   GetDrcNumberOfPhotons() const { return fAodTrack->GetDrcNumberOfPhotons(); }
    virtual Int_t   GetDrcNumberOfBackgroundPhotons() const { return fAodTrack->GetDrcNumberOfBackgroundPhotons(); }
    virtual Int_t   GetDrcNumberOfExpectedPhotons(Int_t n) const { return fAodTrack->GetDrcNumberOfExpectedPhotons(n); }
    virtual Float_t GetDchDeltaMomentum() const { return fAodTrack->GetDchDeltaMomentum(); }
    virtual Float_t GetDrcDeltaMomentum() const { return fAodTrack->GetDrcDeltaMomentum(); }

    virtual Int_t GetDrcInBar() const { return fAodTrack->GetDrcNumBar(); } 
    virtual Int_t GetDrcExitBar() const { return fAodTrack->GetDrcExit(); }
    virtual Float_t GetDrcXPos() const { return fAodTrack->GetDrcXpos(); }
    virtual Float_t GetDrcPhiAtEmc() const { return fAodTrack->GetDrcPhiAtEmc(); }
    virtual Float_t GetDrcThetaAtEmc() const { return fAodTrack->GetDrcThetaAtEmc(); }
    virtual Float_t GetDchMeanDEdx() const { return fAodTrack->GetDchMeanDEdx(); } 
    virtual Float_t GetSvtMeanDEdx() const { return fAodTrack->GetSvtMeanDEdx(); }
    virtual Int_t   GetDchSamples() const { return fAodTrack->GetDchSamples(); }
    virtual Int_t   GetSvtSamples() const { return fAodTrack->GetSvtSamples(); }

    // Access to PID information
    virtual VAbsPidInfo& GetDrcPidInfo() const { return (VAbsPidInfo&)fAodTrack->GetDrcPidInfo(); }
    virtual VAbsPidInfo& GetDchPidInfo() const { return (VAbsPidInfo&)fAodTrack->GetDchPidInfo(); }
    virtual VAbsPidInfo& GetSvtPidInfo() const { return (VAbsPidInfo&)fAodTrack->GetSvtPidInfo(); }
    virtual VAbsPidInfo& GetEmcPidInfo() const { return (VAbsPidInfo&)fAodCluster->GetEmcPidInfo(); }
    virtual VAbsPidInfo& GetIfrPidInfo() const { return (VAbsPidInfo&)fAodCluster->GetIfrPidInfo(); }

    // Modifiers (for event display)

    virtual void    SetPosition(TVector3 x) { fAodBase->SetPosition(x); }
    virtual void    SetMcPid(Int_t pid) { fAodBase->SetMcPid(pid); }
    virtual void    SetMcIndex(Int_t index) { fAodBase->SetMcIndex(index); } 
    virtual void    SetMcConsistency(Float_t cons) { fAodBase->SetMcConsistency(cons); } 

private :
    PAFAodBase    *fAodBase;	// !Do not stream
    PAFAodCluster *fAodCluster;	// !Do not stream
    PAFAodTrack   *fAodTrack;	// !Do not stream
    
public :
    ClassDef(PAFMicroCandidate,1) // PAF implementation of the MicroCandidate
	
};

std::ostream&  operator << (std::ostream& o, const PAFMicroCandidate&);

#endif                                           


