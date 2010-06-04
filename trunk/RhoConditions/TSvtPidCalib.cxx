//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TSvtPidCalib			    					//
//                                                                      //
// SVT conditions class							//
//                                                                      //
// Author: Marcel Kunze, RUB, Nov. 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "RhoConditions/TSvtPidCalib.h"
#include "TMath.h"

ClassImp(SvtPidSimpleTMCalibR)

#include <iostream>
#include <iomanip>
using namespace std;

SvtPidSimpleTMCalibR::SvtPidSimpleTMCalibR()
{ 
  // these values are taken from SvtPidInfo. They should be overridden by
  // condDB

  _aMean = 3.00;
  _pMean = 2.66;
  _pgb   = 0.025;
  _cMean = 3.54;

  _aSig1  = 0.254;
  _pSig1  = 2.97;
  _bSig1  = 0.933;
  _cSig1  = 0.443;

  _aSig2  = 0.286;
  _pSig2  = 3.11;
  _bSig2  = 1.029;
  _cSig2  = 0.465;
}

SvtPidSimpleTMCalibR::~SvtPidSimpleTMCalibR() { }

Double_t
SvtPidSimpleTMCalibR::Beta2dedx(Double_t beta) const {  
  Double_t dedx=0.;
  if (beta > 0 && beta <= 1.) {
    // put in a relativistic rise with a plateau for electrons
    Double_t gammaBeta = 40.;
    if ( beta < 1.) gammaBeta = beta / sqrt(1.0-beta*beta);
    Double_t modifiedGammaBeta = gammaBeta;
    if(gammaBeta>40.) modifiedGammaBeta=40.;
    dedx=_aMean*pow(beta,-(double)_pMean) * pow(modifiedGammaBeta,(double)_pgb);
  }
  return dedx;
}

Double_t
SvtPidSimpleTMCalibR::Dedx2beta(Double_t dedx) const 
{
  //
  // this is now only an approximation (good to ~5%)
  //
  Double_t beta=1.;
  if (dedx > 0 && dedx < _cMean) {
    beta=pow(dedx/_aMean,-1./_pMean);
  }

  beta=TMath::Max(TMath::Min(beta,1.),0.);

  return beta;
}

Double_t
SvtPidSimpleTMCalibR::Beta2sig1(Double_t beta) const 
{
  //
  // This is actually sig(log(dedx)), but the fit is
  // better for sig(dedx). The fit variable is gamma*beta
  //

  Double_t sig1=0;
  if (beta > 0 && beta <= 1.) {
    Double_t dedx=Beta2dedx(beta);
    // put in a relativistic rise with a plateau for electrons
    Double_t gammaBeta = 40.;
    if ( beta < 1.) gammaBeta = beta / sqrt(1.0-beta*beta);
    if ( dedx>0 ) {
      sig1=_aSig1*pow(gammaBeta/_bSig1,-(double)_pSig1)/dedx+_cSig1;
    }
  }
  return sig1;
}

Double_t
SvtPidSimpleTMCalibR::Beta2sig2(Double_t beta) const 
{
  //
  // This is actually sig(log(dedx)), but the fit is
  // better for sig(dedx). The fit variable is gamma*beta
  //

  Double_t sig2=0;
  if (beta > 0 && beta <= 1.) {
    Double_t dedx=Beta2dedx(beta);
    Double_t gammaBeta = 40.;
    if ( beta < 1.) gammaBeta = beta / sqrt(1.0-beta*beta);
    if ( dedx>0 ) {
      sig2=_aSig2*pow(gammaBeta/_bSig2,-(double)_pSig2)/dedx+_cSig2;
    }
  }
  return sig2;
}

void SvtPidSimpleTMCalibR::PrintOn(std::ostream& os) const 
{
  os << "Class SvtPidSimpleTMCalib:" << endl;

  os << "\t meanPar: "; 
  os << setw(10) << setprecision(4) << _aMean 
     << setw(10) << setprecision(4) << _pMean 
     << setw(10) << setprecision(4) << _pgb 
     << setw(10) << setprecision(4) << _cMean 
     << endl;

  os << "\t sig1Par: ";
  os << setw(10) << setprecision(4) << _aSig1 
     << setw(10) << setprecision(4) << _bSig1
     << setw(10) << setprecision(4) << _pSig1
     << setw(10) << setprecision(4) << _cSig1
     << endl;

  os << "\t sig2Par: ";
  os  << setw(10) << setprecision(4) << _aSig2
      << setw(10) << setprecision(4) << _bSig2 
      << setw(10) << setprecision(4) << _pSig2
      << setw(10) << setprecision(4) << _cSig2
      << endl;
}

std::ostream&  operator << (std::ostream& o, const SvtPidSimpleTMCalibR& a) { a.PrintOn(o); return o; }

ClassImp(TSvtPidCalib)


