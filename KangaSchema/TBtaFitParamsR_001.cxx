//--------------------------------------------------------------------------
//
// Description:
//      Class TBtaFitParamsR_001
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Yury Kolomensky
//      Leif Wilden
//
// Copyright Information:
//	Copyright (C) 1999         Caltech, TU Dresden
//
//------------------------------------------------------------------------

#include "KangaSchema/TBtaFitParamsR_001.h"
#include "KangaSchema/kanga.h"

ClassImp(TBtaFitParamsR_001)

TBuffer &operator>>(TBuffer &buf, TBtaFitParamsR_001 *&obj)
{
   obj = (TBtaFitParamsR_001 *) buf.ReadObject(TBtaFitParamsR_001::Class());
   return buf;
}

TBtaFitParamsR_001::TBtaFitParamsR_001()
{
  Reset();
}

TBtaFitParamsR_001::~TBtaFitParamsR_001() {}

void TBtaFitParamsR_001::Reset()
{
  _q= 0;
  int i= 0;
  for( i= 0; i < 5; i++ ) _params[i]= 0;
  for( i= 0; i < 15; i++ ) _cov[i]= 0;
}

// Getters:

Int_t TBtaFitParamsR_001::GetQ() const
{ 
  TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_q);
  return _q;
}

void TBtaFitParamsR_001::GetP(Float_t p[5]) const
{
  TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_params);
  p[0]= _params[0];
  p[1]= _params[1];
  p[2]= _params[2];
  p[3]= _params[3];
  p[4]= _params[4];
}

void TBtaFitParamsR_001::GetCov(Float_t cov[15]) const
{
  TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_cov);
  for (Int_t i=0; i<15; i++) cov[i]= _cov[i];
}

// Setters:

void TBtaFitParamsR_001::SetQ(Int_t q) 
{ _q= q; }

void TBtaFitParamsR_001::SetP(const Float_t p[5]) 
{ _params[0]= p[0]; _params[1]= p[1]; _params[2]= p[2]; _params[3]= p[3]; _params[4]= p[4]; }

void TBtaFitParamsR_001::SetCov(const Float_t cov[15])
{
  for (Int_t i=0; i<15; i++) _cov[i]= cov[i];
}

