#ifndef PAFAodCluster_H
#define PAFAodCluster_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFAodCluster							//
//                                                                      //
// Definition of the Analysis Object Data database			//
//                                                                      //
// Author: Marcel Kunze, Bochum University, March 1999			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#define NELEM_AODCLUSTER 48

#include "PAFSchema/PAFAbsPidInfo.h"
class TVector3;

//  =================================================================
//  ===== PAFAodCluster - Class for storage of Neutral cands ====
//  =================================================================

class PAFAodCluster 
{
public:
   

    // empty constructor
    PAFAodCluster();
    
    // copy constructor
    PAFAodCluster(PAFAodCluster& other);
    
    virtual ~PAFAodCluster();
    
    PAFAodCluster* GetCluster() { return this; }
    
    // use default copy constructor
    
    // Get DetStatus
    Bool_t EmcOk() const;
    Bool_t IfrOk() const;
    
    // Set DetStatus
    void SetClusterStatus(Bool_t emc, Bool_t ifr);
    
    // Get Emc Data ( <-> BtaCalqual) 
    Int_t   GetEmcNumberOfCrystals() const;
    Int_t   GetEmcNumberOfBumps() const;
    Float_t GetEmcCalEnergy() const;
    Float_t GetEmcRawEnergy() const;
    Float_t GetEmcCovarianceEnergy() const;
    Float_t GetEmcCovarianceTheta() const;
    Float_t GetEmcCovariancePhi() const;
    Float_t GetEmcCovarianceRho() const;
    Float_t GetEmcS9S25() const;
    Float_t GetEmcS1S9() const;
    Float_t GetEmcLateralShape() const;
    Float_t GetEmcSecondMoment() const;
    Float_t GetEmcZernikeMoment20() const;
    Float_t GetEmcZernikeMoment31() const;
    Int_t GetEmcStatus() const;
    Float_t GetEmcZernikeMoment42() const;
    TVector3 GetEmcCentroid() const;
    Float_t GetEmcConsistencyValue() const;
    Float_t GetEmcConsistencyLikelihood() const;
    Int_t   GetEmcConsistencyBits() const;
    Int_t   GetNeutralPidHypo() const;
    
    // Set Emc Data
    void    SetEmcNumberOfCrystals(Int_t);
    void    SetEmcNumberOfBumps(Int_t);
    void    SetEmcRawEnergy(Float_t);
    void    SetEmcCalEnergy(Float_t);
    void    SetEmcCovarianceEnergy(Float_t);
    void    SetEmcCovariancePhi(Float_t);
    void    SetEmcCovarianceTheta(Float_t);
    void    SetEmcCovarianceRho(Float_t);
    void    SetEmcLateralShape(Float_t);
    void    SetEmcS9S25(Float_t);
    void    SetEmcS1S9(Float_t);
    void    SetEmcSecondMoment(Float_t t);
    void    SetEmcZernikeMoment20(Float_t);
    void    SetEmcZernikeMoment31(Float_t);
    void    SetEmcStatus(Int_t);
    void    SetEmcZernikeMoment42(Float_t);
    void    SetEmcCentroid(TVector3);
    void    SetEmcConsistencyValue(Float_t);
    void    SetEmcConsistencyLikelihood(Float_t);
    void    SetEmcConsistencyBits(Int_t);
    void    SetNeutralPidHypo(Int_t);
    
    // GetIfrData ( <-> BtaIfrQual )
    Int_t   GetIfrPattern() const;
    Int_t   GetIfrHitLayers() const;
    Int_t   GetIfrNumberOfStrips() const;
    Int_t   GetIfrNumberOfStrips(Int_t) const;
    Int_t   GetIfrFirstHit() const;
    Int_t   GetIfrLastHit() const;
    Int_t   GetIfrClusters() const;
    Float_t GetIfrMeasuredInteractionLengths() const;
    Float_t GetIfrExpectedInteractionLengths() const;
    Float_t GetIfrInteractionLengthsBeforeIron() const;
    Float_t GetIfrTrackMatchConsistency() const;
    Float_t GetIfrEmcMatchConsistency() const;       
    Int_t   GetIfrLastBarrel() const;
    Float_t GetIfrRpcMuProb() const;
    Float_t GetIfrClusfitChi2() const;
    
    // SetIfrData 
    void    SetIfrPattern(Int_t);
    void    SetIfrHitLayers(Int_t);
    void    SetIfrNumberOfStrips(Int_t);
    void    SetIfrNumberOfStrips(Int_t,Int_t);
    void    SetIfrFirstHit(Int_t);
    void    SetIfrLastHit(Int_t);
    void    SetIfrClusters(Int_t);
    void    SetIfrMeasuredInteractionLengths(Float_t);
    void    SetIfrExpectedInteractionLengths(Float_t);
    void    SetIfrInteractionLengthsBeforeIron(Float_t);
    void    SetIfrTrackMatchConsistency(Float_t);
    void    SetIfrEmcMatchConsistency(Float_t);
    void    SetIfrLastBarrel(Int_t);
    void    SetIfrClusfitChi2(Float_t);
    void    SetIfrRpcMuProb(Float_t);
    
    // Get PidInfo
    const PAFAbsPidInfo& GetEmcPidInfo() const;
    const PAFAbsPidInfo& GetIfrPidInfo() const;
    
    // SetPidInfo
    void SetEmcPidInfo(PAFAbsPidInfo &p);
    void SetIfrPidInfo(PAFAbsPidInfo &p);
    
    inline Bool_t isValid() { return _EMCcrystals; }
    // output
    void PrintOn( std::ostream& o =std::cout) const;

    //BtaCalQual________________________________________________
    
    
    
    Short_t _EMCcrystals;	// 1                
    Byte_t  _EMCbumps;		// 2
    Float_t _EMCenergy,		// 3
	    _EMClateralshape,	// 4
	    _EMC20,		// 5
	    _EMC31,		// 6
	    _EMC42,		// 7
	    _EMCsecmom,		// 8
	    _EMCconsistency,	// 9
	    _EMCcalenergy;	// 10
    Byte_t  _EMCstatus;		// 11
    Char_t  _EMCpidhypo;	// 12
    Float_t _EMCcx,		// 13
	    _EMCcy,		// 14
	    _EMCcz,		// 15
	    _EMCcovenergy,	// 16
	    _EMCcovtheta,	// 17
	    _EMCcovphi,		// 18
	    _EMCcovrho;		// 19
    
    //BtaIfrQual________________________________________________
    Byte_t  _IFRlayers,		// 20
	    _IFRstrips,		// 21
	    _IFRpattern,	// 22
	    _IFRclusters,	// 23
	    _IFRfirsthit,	// 24
	    _IFRlasthit;	// 25
    Float_t _IFRiron,		// 26
	    _IFRtrk,		// 27
	    _IFRemc;		// 28
    Float_t _IFRtrkmatch,	// 29
	    _IFRemcmatch;	// 30
    Byte_t  _IFRlaystrips[20];	// 31
    Float_t _IFRrpcMuProb;	// 32
    Float_t _IFRclusfitChi2;	// 33
					    
    // Hit patterns
    Char_t  _cpattern;		// 34
    
    // PidInfo__________________________________________________
    PAFAbsPidInfo _EMCPidInfo,	// 35-38
		  _IFRPidInfo;	// 39-42
    
    // 8.3.x schema
    Float_t _EMCs9s25,		// 43
	    _EMCs1s9,		// 44
	    _EMClikelihood;	// 45
    Char_t  _EMCbits,		// 46
	    _IFRlastbar;	// 47
    
public :
    
    ClassDef(PAFAodCluster,2) //EMC and IFR info
	
};

std::ostream&  operator << (std::ostream& o, const PAFAodCluster&);

#endif                                           


