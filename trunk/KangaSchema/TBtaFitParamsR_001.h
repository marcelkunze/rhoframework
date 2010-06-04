//--------------------------------------------------------------------------
//
// Description:
//	Class TBtaFitParamsR_001
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Stefan Kluth
//      Leif Wilden (adapted as ROOT standalone version)
//
// Copyright Information:
//	Copyright (C) 1999                LBNL, TU Dresden
//
//------------------------------------------------------------------------
#ifndef BTAFITPARAMSR001_HH
#define BTAFITPARAMSR001_HH

#include "TObject.h"

class TBtaMicroCandR;

class TBtaFitParamsR_001 {

public:

  TBtaFitParamsR_001();

  virtual ~TBtaFitParamsR_001();

  virtual void Reset();

  // Getters:

  Int_t   GetQ() const;
  void    GetP(Float_t p[5]) const;
  void    GetCov(Float_t cov[15]) const;

  // Setters:

  void    SetQ(Int_t q);
  void    SetP(const Float_t p[5]);
  void    SetCov(const Float_t cov[15]);

  friend class TBtaMicroCandR;

private:

  // the params
  Short_t _q;
  Float_t _params[5];
  
  // (4x4 neutrals, 5x5 charged)
  Float_t _cov[15];

  // disallow
  TBtaFitParamsR_001( const TBtaFitParamsR_001 & );
  TBtaFitParamsR_001 & operator= ( const TBtaFitParamsR_001 & );

  ClassDef(TBtaFitParamsR_001,1) // Kinematics info (Rho Kanga implementation)

};

#endif




