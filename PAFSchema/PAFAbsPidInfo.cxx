//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFAbsPidInfo							//
//                                                                      //
// Particle identification info class					//
//                                                                      //
// Author: T. Brandt, TUD						//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "PAFSchema/PAFAbsPidInfo.h"


ClassImp(PAFAbsPidInfo)

#include <iostream>
using namespace std;

PAFAbsPidInfo::PAFAbsPidInfo()
{
  for (Int_t i=0; i<4; i++) {
   _likelihood[i]=0;
   _significanceLevel[i]=0;
  }
  _status=32768;
  _sign=0;
}


const Float_t*
PAFAbsPidInfo::GetLikelihood() const
{
  return _likelihood;
}

Float_t
PAFAbsPidInfo::GetLikelihood(Int_t hypo) const
{
  if ((hypo>=0) && (hypo<5)) {
    return _likelihood[hypo];
  } else {
    return 0;
  }
}


const Float_t*
PAFAbsPidInfo::GetSignificance() const
{
  return _significanceLevel;
}


Float_t
PAFAbsPidInfo::GetSignificance(Int_t hypo) const
{
  if ((hypo>=0) && (hypo<5)) {
    return _significanceLevel[hypo];
  } else {
    return 0;
  }
}

Int_t
PAFAbsPidInfo::GetStatus(Int_t hypo) const
{
  if ((hypo>=0) && (hypo<5)) {
    UShort_t m = 7 << (3*hypo);
    return  (_status & m) >> (3*hypo);
  } else {
    return CONS_noMeasure;
  }
}

Int_t 
PAFAbsPidInfo::GetSign(Int_t hypo) const
{
  if ((hypo>=0) && (hypo<5)) {
    UShort_t m = 3 << (2*hypo);
    return  (_sign & m) >> (2*hypo);
  } else {
    return CONS_unknown;
  }
}


Int_t
PAFAbsPidInfo::IsValid() const
{
  if ((_status & (UShort_t) 32768) == 0) {
    return -1; 
  } else {
    return 0;
  }
}


void
PAFAbsPidInfo::SetStats(Int_t hypo, Float_t sl, Float_t lhood, Int_t status, Int_t sign)
{
  if ((hypo>=0) && (hypo<5)) {
    _status = _status & (UShort_t) 32767; 
    _significanceLevel[hypo] = sl;
    _likelihood[hypo] = lhood;
    _status = _status & ~(7 << (3*hypo));
    _status = _status | (status << (3*hypo));

    _sign = _sign & ~(3 << (2*hypo));
    _sign = _sign | ((sign&3) << (2*hypo));
  }
}

void 
PAFAbsPidInfo::Invalidate()
{
  _status = _status | (UShort_t) 32768;
}

void
PAFAbsPidInfo::SetNoMeas()
{
  for (Int_t i=0; i<5; i++) {
    SetStats(i,0.0,1.0,CONS_noMeasure,CONS_unknown);
  }
}

void
PAFAbsPidInfo::PrintOn(std::ostream& o) const
{
  if (IsValid()) {
    o << "Status : " << GetStatus() << " (valid) " << endl;
  } else {
    o << "Status : " << GetStatus() << " (*invalid*) " << endl;
  }
  
  for (Int_t i=0; i<5; i++) {
    o << "     Hypo  " << i << " : ";
    o << "SL = " << GetSignificance(i) << " ; LH = " << GetLikelihood(i) << " ; ";
    o << "Stat = ";
    Int_t k = GetStatus(i);
    if (k==CONS_OK) { o << "OK"; }
    if (k==CONS_noMeasure) { o << "noMeasure"; }
    if (k==CONS_unPhysical) { o << "unPhysical"; }
    if (k==CONS_underFlow) { o << "underFlow"; }
    o << " ; Sign = ";
    k = GetSign(i);
    if (k==CONS_left) { o << "left"; }
    if (k==CONS_right) { o << "right"; }
    if (k==CONS_unknown) { o << "unknown"; }
    o << endl;
  }
}
