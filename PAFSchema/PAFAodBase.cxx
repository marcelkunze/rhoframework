//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFAodBase								//
//                                                                      //
// Definition of the Analysis Object Data database			//
//                                                                      //
// Author: Marcel Kunze, Bochum University, March 1999			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "PAFSchema/PAFAodBase.h"

#include "TString.h"

ClassImp(PAFAodBase)

#include <iostream>
using namespace std;

PAFAodBase::PAFAodBase() :
_xposition(0),
_yposition(0),
_zposition(0),
_xmomentum(0),
_ymomentum(0),
_zmomentum(0),
_energy(0),
_mcCons(0),
_mcHypo(0),
_mcIndex(-1),
_DetBitpattern(0),
_charge(-1)
{
    for (int j=0; j<MATRIXSIZE;j++) {
	_errP7[j] = 0;
    }    
}

PAFAodBase::PAFAodBase(PAFAodBase& other)
{
    // copy data
    _xposition = other._xposition;
    _yposition = other._yposition;
    _zposition = other._zposition;
    _xmomentum = other._xmomentum;
    _ymomentum = other._ymomentum;
    _zmomentum = other._zmomentum;
    _energy = other._energy;
    _mcCons = other._mcCons;
    _mcHypo = other._mcHypo;
    _mcIndex = other._mcIndex;
    _DetBitpattern = other._DetBitpattern;
    _charge = other._charge;
    for (int i=0; i<MATRIXSIZE; i++) {
	_errP7[i] = other._errP7[i];
    }
}

PAFAodBase::~PAFAodBase()  
{ 
}

void
PAFAodBase::Reset() 
{
    _xposition=0; _yposition=0; _zposition=0;
    _xmomentum=0; _ymomentum=0; _zmomentum=0;
    _energy=0; _mcCons=0; _mcHypo=0; _mcIndex=0;
    _DetBitpattern=0, _charge=-1;
    for (int j=0; j<MATRIXSIZE;j++) {
	_errP7[j] = 0;
    }
    
}

void PAFAodBase::SetCharge(Int_t q) { _charge = (Char_t) q; }
Int_t PAFAodBase::GetCharge() const 
{ 
    return _charge; 
};

// ____ set & get position __________

void 
PAFAodBase::SetPosition(TVector3 t) {
    _xposition = (Float_t) t.X();
    _yposition = (Float_t) t.Y();
    _zposition = (Float_t) t.Z();
}

TVector3 
PAFAodBase::GetPosition()  const 
{
    return TVector3(_xposition,_yposition,_zposition);
}


// ____ set & get momentum __________


void PAFAodBase::SetMomentum(TVector3 t) 
{
    _xmomentum = (Float_t) t.X();
    _ymomentum = (Float_t) t.Y();
    _zmomentum = (Float_t) t.Z();
}

TVector3 PAFAodBase::GetMomentum() const 
{
    return TVector3(_xmomentum,_ymomentum,_zmomentum);
}


TLorentzVector 
PAFAodBase::GetLorentzVector() const 
{ 
    return TLorentzVector(_xmomentum,_ymomentum,_zmomentum,_energy); 
}

// ____ set & get energy __________

void 
PAFAodBase::SetEnergy(Float_t t) 
{
    _energy=t;
}

Float_t PAFAodBase::GetEnergy() const 
{
     return _energy;
}


// ____ set & get ErrorMatrix _______


void 
PAFAodBase::SetPositionError(Float_t *d) 
{
    for (int i=0;i<6;i++) _errP7[i]=d[i];
}

const Float_t*
PAFAodBase::GetPositionError() const
{ 
    return &(_errP7[0]);
}


void 
PAFAodBase::SetMomentumError(Float_t *d) 
{
    for (int i=18;i<28;i++) _errP7[i]=d[i];
}

const Float_t*
PAFAodBase::GetMomentumError() const
{
    return &(_errP7[18]);
}


void 
PAFAodBase::SetErrorP7(Float_t *d)
{
    for (int i=0;i<28;i++) _errP7[i]=d[i];
}

const Float_t*
PAFAodBase::GetErrorP7() const
{
    return _errP7;
}

// ____ set & get MC consistency __________

void
PAFAodBase::SetMcConsistency(Float_t cons) 
{
    _mcCons = cons;
}

Float_t
PAFAodBase::GetMcConsistency() const
{
    return _mcCons;
}


// ____ set & get MCPid __________

void 
PAFAodBase::SetMcPid(Int_t pid)
{
    _mcHypo = pid;
}

void 
PAFAodBase::SetMcIndex(Int_t no)
{
    _mcIndex = no;
}

Int_t
PAFAodBase::GetMcPid() const
{
    return _mcHypo;
}

Int_t
PAFAodBase::GetMcIndex() const
{
    return _mcIndex;
}


// set & get DETbitpattern

void 
PAFAodBase::SetBitPatternOfDetectors(Int_t p)
{
    _DetBitpattern=(Short_t) p;
}


Int_t 
PAFAodBase::GetBitPatternOfDetectors()
{
    return _DetBitpattern;
}


void PAFAodBase::PrintOn(std::ostream &o) const
{
    TVector3 r = GetPosition();
    TVector3 p = GetMomentum();
    o << " FIT position                     : (" << r.X() << ";" << r.Y() << ";" << r.Z() << ")" << endl;
    o << " FIT momentum                     : (" << p.X() << ";" << p.Y() << ";" << p.Z() << ")" << endl;
    o << " FIT momentum.mag                 : " << p.Mag() << endl;
    o << " FIT charge                       : " << GetCharge() << endl;
    o << " FIT energy                       : " << GetEnergy() << endl;
    const Float_t *err = GetPositionError();
    o << " FIT postion error                :   posErr(1,1) = " << err[0] << " ; posErr(2,2) = " 
	<< err[2] << " ; posErr(3,3) = " << err[5] << endl;
    o << " FIT mtm error                    :   mtmErr(1,1) = " << err[18] << " ; mtmErr(2,2) = " 
	<< err[20] << " ; mtmErr(3,3) = " << err[23] << " ; mtmErr(4,4) = " << err[27] << endl;
}


std::ostream&  operator << (std::ostream& o, const PAFAodBase& a) { a.PrintOn(o); return o; }
