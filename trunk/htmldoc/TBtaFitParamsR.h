//--------------------------------------------------------------------------
//
// Description:
//	Class TBtaFitParamsR
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Stefan Kluth
//      Leif Wilden (adapted as ROOT standalone version)
//      M. Kunze, added support for Kanga schema >=3, Dec. 2k
//
// Copyright Information:
//	Copyright (C) 1999                LBNL, TU Dresden
//
//------------------------------------------------------------------------
#ifndef TBTAFITPARAMSR_HH
#define TBTAFITPARAMSR_HH

#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"

class TBtaMicroCandR;
class TFitParams;
class TVectorD;

class TBtaFitParamsR {
    
public:
    
    TBtaFitParamsR();
    
    virtual ~TBtaFitParamsR();
    
    virtual void Reset();
    
    // Getters:
    
    Int_t	    GetQ() const;
    TVector3	    GetPos();
    TVector3	    GetP();
    TLorentzVector  GetP4();
    Float_t*	    GetParams();
    Float_t*	    GetCov7();
    Float_t*	    GetCov();

    void    GetPos(Float_t pos[3]) const;
    void    GetP4(Float_t p4[4]) const;
    void    GetParams(Float_t p[5]) const;
    void    GetCov7(Float_t cov7[28]) const;
    void    GetCov(Float_t cov[15]) const;
    
    // Setters:
    
    void    SetQ(Int_t q);
    void    SetPos(const Float_t pos[3]);
    void    SetP4(const Float_t p4[4]);
    void    SetParams(const Float_t p[5]);
    void    SetCov7(const Float_t cov7[28]);
    void    SetCov(const Float_t cov[15]);
	  
    friend class TBtaMicroCandR;
    
private:
    
    TFitParams* GetTFitParams(Int_t _q, Float_t *_params, Float_t *_cov);

    // the params
    Char_t  _q;
    Char_t  _qold;
    Float_t _pos[3];
    Float_t _p4[4];
    Float_t _params[5]; // Helix parameters
    UShort_t _diag[15];
    Char_t   _corr[10];
    
    // the 7X7 covariance
    Float_t _cov7[28];
    // the new cov. matrix
    Float_t _cov[15];
    
    Float_t fCov[28]; //!
    
    // disallow
    TBtaFitParamsR( const TBtaFitParamsR & );
    TBtaFitParamsR & operator= ( const TBtaFitParamsR & );

    Int_t nParm() const {return (_q==0) ? 4:5;}
    Int_t nCorr() const {Int_t np=nParm();return Int_t( np*(np-1)/2. );}
    Int_t nOct() const{return (_q==0) ? nCorr():0;}
    Int_t nShort() const{return (_q==0) ? nParm():nParm()+nCorr();}
    Int_t indexCorr(Int_t i, Int_t j) const;
    void Load() ;   
    void LoadRotation() ;   
    void ComPackedCovariance(const TVectorD& eigenValues,const TVectorD& angles);
public:
    ClassDef(TBtaFitParamsR,2) // Kinematics info (T Kanga implementation)	
};

#endif




