#ifndef PAFAodTrack_H
#define PAFAodTrack_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFAodTrack								//
//                                                                      //
// Definition of the Analysis Object Data database (charged part)	//
//                                                                      //
// Author: Marcel Kunze, Bochum University, March 1999			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#define NELEM_AODTRACK 43

#include "PAFSchema/PAFAbsPidInfo.h"

//  =================================================================
//  ===== PAFAodTrack - Class for storage of charged cands ====
//  =================================================================


class PAFAodTrack 
{
public:
    
    // empty constructor
    PAFAodTrack();
    
    // copy constructor
    PAFAodTrack(PAFAodTrack &other);
    
    // Do not use default destructor, as this might not be virtual
    virtual ~PAFAodTrack();
    
    PAFAodTrack* GetTrack() { return this; }
    
    // use default copy constructor
    
    // Get DetStatus
    Bool_t SvtOk() const;
    Bool_t DchOk() const;
    Bool_t DrcOk() const;
    
    // Set DetStatus
    void SetTrackStatus(Bool_t svt,Bool_t dch, Bool_t drc);
    
    // Get Track Data ( <-> BtaTrkQual)
    Float_t GetTrackLength() const;
    Float_t GetTrackStartFoundRange() const;
    Int_t   GetSvtHitPattern() const;
    Int_t   GetSvtHits() const;
    Int_t   GetDchInnerLayer() const;
    Int_t   GetDchOuterLayer() const;
    Int_t   GetDchHits() const;
    Int_t   GetChargedPidHypo() const;
    Int_t   GetDegreesOfFreedom() const;
    Int_t   GetFitStatus() const;
    Float_t GetProbability() const;
    Float_t GetChiSquared() const;

    
    // Set Track Data
    void    SetTrackLength(Float_t);
    void    SetTrackStartFoundRange(Float_t);
    void    SetSvtHitPattern(Int_t);
    void    SetSvtHits(Int_t);
    void    SetDchInnerLayer(Int_t);
    void    SetDchOuterLayer(Int_t);
    void    SetDchHits(Int_t);
    void    SetChargedPidHypo(Int_t);
    void    SetDegreesOfFreedom(Int_t);
    void    SetFitStatus(Int_t);
    void    SetProbability(Float_t);
    void    SetChiSquared(Float_t);
    
    // Set Pid Data ( <-> BtaPitqual )
    // void    SetDrcStatus(Int_t); TB 21.02.00 : No status in 8.5.x BtaPidQual
    void    SetDrcNumBar(Int_t);
    void    SetDrcExit(Int_t);
    void    SetDrcXpos(Float_t);	      
    void    SetDrcThetaC(Float_t);
    void    SetDrcThetaCError(Float_t);
    void    SetDrcNumberOfPhotons(Int_t nphot);
    void    SetDrcNumberOfBackgroundPhotons(Int_t nphot);
    void    SetDrcNumberOfExpectedPhotons(Int_t nphot,Int_t hypo=0);
    void    SetDchDeltaMomentum(Float_t);
    void    SetDrcDeltaMomentum(Float_t);
    void    SetDrcPhiAtEmc(Float_t);
    void    SetDrcThetaAtEmc(Float_t);
    void    SetDchMeanDEdx(Float_t);
    void    SetSvtMeanDEdx(Float_t);
    void    SetDchSamples(Int_t);
    void    SetSvtSamples(Int_t);
    void    SetEmcInterPhi(Float_t); 
    void    SetEmcInterTheta(Float_t); 

    // Get Pid Data
    //Int_t       GetDrcStatus() const;  TB 21.02.00 : No status in 8.5.x BtaPidQual
    Int_t       GetDrcNumBar() const;
    Int_t       GetDrcExit() const;
    Float_t     GetDrcXpos() const;
    Float_t     GetDrcThetaC() const;
    Float_t     GetDrcThetaCError() const;
    Int_t       GetDrcNumberOfPhotons() const;
    Int_t       GetDrcNumberOfBackgroundPhotons() const;
    Int_t       GetDrcNumberOfExpectedPhotons(Int_t hypo=0) const;
    Float_t     GetDchDeltaMomentum() const;
    Float_t     GetDrcDeltaMomentum() const; 
    Float_t     GetDrcPhiAtEmc() const;
    Float_t     GetDrcThetaAtEmc() const;
    Float_t     GetDchMeanDEdx() const; 
    Float_t     GetSvtMeanDEdx() const;
    Int_t	GetDchSamples() const;
    Int_t	GetSvtSamples() const;
    Float_t     GetEmcInterPhi() const;
    Float_t     GetEmcInterTheta() const;    

    // Set PidInfo
    void SetDrcPidInfo(PAFAbsPidInfo& p);
    void SetDchPidInfo(PAFAbsPidInfo& p);
    void SetSvtPidInfo(PAFAbsPidInfo& p);
    
    // Get PidInfo
    const PAFAbsPidInfo& GetDrcPidInfo() const;
    const PAFAbsPidInfo& GetDchPidInfo() const;
    const PAFAbsPidInfo& GetSvtPidInfo() const;
    void PrintOn( std::ostream& o =std::cout) const;
    
    //BtaTrkQual________________________________________________
    Byte_t  _SVThitpattern,		    // 1
	    _SVThits,			    // 2
	    _DCHinnerlayer,		    // 3
	    _DCHouterlayer,		    // 4
	    _DCHhits;			    // 5
    Char_t  _TRKpidhypo;		    // 6
    Short_t _TRKndof,			    // 7
	    _TRKfitstatus;		    // 8
    Float_t _TRKprob,			    // 9
	    _TRKchi2,			    // 10
	    _TRKtlen;			    // 11
    
    //BtaPIDQual________________________________________________
    Float_t _DRCthetaC,			    // 12
	    _DRCethetaC,		    // 13
	    _DCHmom,			    // 14
	    _DRCmom;			    // 15
    Int_t   _DRCnphot,			    // 16 
	    _DRCnbkgd;			    // 17
    Short_t _DRCstatus;			    // 18
    Short_t _DRCnumbar;			    // 19
    Short_t _DRCexit;			    // 20
    Float_t _DRCxpos;			    // 21
					  
    Float_t _DCHdedx,			    // 22
	    _SVTdedx;			    // 23
    	
    // PidInfo__________________________________________________
    PAFAbsPidInfo _DCHPidInfo,		    // 24-27
		  _DRCPidInfo,		    // 28-31
		  _SVTPidInfo;		    // 32-35
    
    // Hit patterns
    Short_t _pattern;			    // 36
    
    // 8.3.x schema
    
    Float_t _TRKst;			    // 37
    Short_t _DRCnexp[5];		    // 38
    Char_t  _DCHsamples,		    // 39
	    _SVTsamples;		    // 40

    // 8.6.x schema
 
    Float_t _EMCinterphi;		    // 41
    Float_t _EMCintertheta;		    // 42

public :
    
    ClassDef(PAFAodTrack,1) //Tracking info
	
};

std::ostream&  operator << (std::ostream& o, const PAFAodTrack&);

#endif                                           


