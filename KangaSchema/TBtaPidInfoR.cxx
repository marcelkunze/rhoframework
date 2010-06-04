//--------------------------------------------------------------------------
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Stefan Kluth
//      Leif Wilden
//
// Copyright Information:
//	Copyright (C) 1999      LBNL, TU Dresden
//
//------------------------------------------------------------------------

#include "KangaSchema/TBtaPidInfoR.h"
#include "KangaSchema/kanga.h"

#include <assert.h>

#include "TBuffer.h"

ClassImp(PidSystem)
ClassImp(TBtaPidInfoR)

TBuffer &operator>>(TBuffer &buf, TBtaPidInfoR *&obj)
{
   obj = (TBtaPidInfoR *) buf.ReadObject(TBtaPidInfoR::Class());
   return buf;
}

ComPackUFloat TBtaPidInfoR::packer(FRACLEN,EXPLEN,BIAS);

TBtaPidInfoR::TBtaPidInfoR()
{
  Reset();
}

TBtaPidInfoR::~TBtaPidInfoR() {}

void TBtaPidInfoR::Reset()
{
  for (int j1=0;j1<5;j1++){ 
    for (int j2=0;j2<5;j2++) _Cons[j1+j2*5]= 0;
    _status[j1+0*5]= 0;
    _status[j1+1*5]= 0;
    _status[j1+2*5]= 0;
  }
  _nent= 0;
  for( unsigned i= 0; i < NCHENT; i++ ) {
    _coValues[i]= 0.0;
    _coLhoods[i]= 0.0;
    _coBits[i]= 0;
  }
}

TBtaPidInfoR & TBtaPidInfoR::operator = ( const TBtaPidInfoR & rhs ) {

  _nent= rhs._nent;
  for( unsigned i= 0; i < _nent; i++ ) {
    _coValues[i]= rhs._coValues[i];
    _coLhoods[i]= rhs._coLhoods[i];
    _coBits[i]= rhs._coBits[i];
  }
  return *this;

}

// Stream an object of class TBtaPidInfoR. This replaces rootcint generated
// code in order to use Write- and ReadFastArray to avoid reading lots of 
// 0s in case of neutrals:
void TBtaPidInfoR::Streamer( TBuffer & R__b ) {

   if( R__b.IsReading() ) {
      Version_t R__v= R__b.ReadVersion(); 
      // if (R__v) { }
      R__b >> _nent;
      R__b.ReadFastArray( _coValues, _nent );
      R__b.ReadFastArray( _coLhoods, _nent );
      R__b.ReadFastArray( _coBits, _nent );
   } 
   else {
      R__b.WriteVersion( TBtaPidInfoR::IsA() );
      R__b << _nent;
      R__b.WriteFastArray( _coValues, _nent );
      R__b.WriteFastArray( _coLhoods, _nent );
      R__b.WriteFastArray( _coBits, _nent );
   }

}

// Getters:

UShort_t TBtaPidInfoR::GetNent() const
{
  TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pi_nent);
  return _nent;
}

void TBtaPidInfoR::GetCoValues(Float_t values[NCHENT]) const
{
  TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pi_coValues);
  for (Int_t i= 0; i<NCHENT; i++) values[i]= _coValues[i];
}

void TBtaPidInfoR::GetCoLhoods(Float_t lhoods[NCHENT]) const
{
  TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pi_coLhoods);
  for (Int_t i= 0; i<NCHENT; i++) lhoods[i]= _coLhoods[i];
}

void TBtaPidInfoR::GetCoBits(UChar_t bits[NCHENT]) const
{
  TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pi_coBits);
  for (Int_t i= 0; i<NCHENT; i++) bits[i]= _coBits[i];
}

void TBtaPidInfoR::SetCoValues(const Float_t values[NCHENT])
{
  for (Int_t i= 0; i<NCHENT; i++) _coValues[i]= values[i];
}

void TBtaPidInfoR::SetCoLhoods(const Float_t lhoods[NCHENT])
{
  for (Int_t i= 0; i<NCHENT; i++) _coLhoods[i]= lhoods[i];
}

// Setters:

void TBtaPidInfoR::SetCoBits(const UChar_t bits[NCHENT])
{
  for (Int_t i= 0; i<NCHENT; i++) _coBits[i]= bits[i];
}


Float_t* TBtaPidInfoR::GetConsistency(PidSystem::System sys)
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pi_status);
	d_UShort status;
	for(int j=0;j<5;j++) {
	    switch (sys) {
	      case PidSystem::dch: status = _status [j+0*5]& 0xf; break;
	      case PidSystem::svt: status = (_status[j+0*5]>>4)& 0xf; break;
	      case PidSystem::drc: status = _status [j+1*5]& 0xf; break;
	      case PidSystem::emc: status = (_status[j+1*5]>>4)& 0xf; break;
	      case PidSystem::ifr: status = _status [j+2*5]& 0xf; break;
	    }
	    double value;
	    packer.unpack(status&0xfff,value);
	    _coValues[5*sys+j] = value;
	}
    }
    return &_coValues[5*sys];
}

Float_t* TBtaPidInfoR::GetLikelihood(PidSystem::System sys)
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pi_cons);
	d_UShort cons;
	for(int j=0;j<5;j++) {
	    switch (sys) {
	      case PidSystem::dch: cons = _Cons[j+0*5]; break;
	      case PidSystem::svt: cons = _Cons[j+1*5]; break;
	      case PidSystem::drc: cons = _Cons[j+2*5]; break;
	      case PidSystem::emc: cons = _Cons[j+3*5]; break;
	      case PidSystem::ifr: cons = _Cons[j+4*5]; break;
	    }
	    double value;
	    packer.unpack(cons&0xfff,value);
	    _coLhoods[5*sys+j] = value;
	}
    }
    return &_coLhoods[5*sys];
}

UChar_t* TBtaPidInfoR::GetStatus(PidSystem::System sys)
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pi_status);
	d_UShort status;
	for(int j=0;j<5;j++) {
	    switch (sys) {
	      case PidSystem::dch: status = _status [j+0*5]& 0xf; break;
	      case PidSystem::svt: status = (_status[j+0*5]>>4)& 0xf; break;
	      case PidSystem::drc: status = _status [j+1*5]& 0xf; break;
	      case PidSystem::emc: status = (_status[j+1*5]>>4)& 0xf; break;
	      case PidSystem::ifr: status = _status [j+2*5]& 0xf; break;
	    }
	    _coBits[5*sys+j] = status>>12;
	}
    }
    return &_coBits[5*sys];
}
