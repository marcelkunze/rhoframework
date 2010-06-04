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

#include "PAFSchema/PAFAodTrack.h"

ClassImp(PAFAodTrack)

#include <iostream>
using namespace std;

PAFAodTrack::PAFAodTrack() :
_SVThitpattern(0),
_SVThits(0),
_DCHinnerlayer(0),
_DCHouterlayer(0),
_DCHhits(0),
_TRKpidhypo(0), 
_TRKndof(0), 
_TRKfitstatus(0), 
_TRKprob(0), 
_TRKchi2(0),
_TRKtlen(0),
_DRCthetaC(0),
_DRCethetaC(0),
_DCHmom(0),
_DRCmom(0),
_DRCnphot(0), 
_DRCnbkgd(0),
_DRCstatus(0), 
_DRCnumbar(0),
_DRCexit(0),
_DRCxpos(0),
_DCHdedx(0),
_SVTdedx(0),
_DCHPidInfo(),
_SVTPidInfo(),
_DRCPidInfo(),
_pattern(0),
_TRKst(0.0),
_DCHsamples(0),
_SVTsamples(0),
_EMCinterphi(0),
_EMCintertheta(0)
{
    for (int i=0;i<5;i++) _DRCnexp[i] = 0;
}

PAFAodTrack::PAFAodTrack(PAFAodTrack &other)
{
    // copy data
    for (int i=0; i<5; i++) {
      _DRCnexp[i] = other._DRCnexp[i];
    }
    _SVThitpattern = other._SVThitpattern;
    _SVThits = other._SVThits;
    _DCHinnerlayer = other._DCHinnerlayer;
    _DCHouterlayer = other._DCHouterlayer;
    _DCHhits = other._DCHhits;
    _TRKpidhypo = other._TRKpidhypo;
    _TRKndof = other._TRKndof;
    _TRKfitstatus = other._TRKfitstatus;
    _TRKprob = other._TRKprob;
    _TRKchi2 = other._TRKchi2;
    _TRKtlen = other._TRKtlen;
    _DRCthetaC = other._DRCthetaC;
    _DRCethetaC = other._DRCethetaC;
    _DCHmom = other._DCHmom;
    _DRCmom = other._DRCmom;
    _DRCnphot = other._DRCnphot;
    _DRCnbkgd = other._DRCnbkgd;
    _DRCstatus = other._DRCstatus; 
    _DRCnumbar = other._DRCnumbar;
    _DRCexit = other._DRCexit;
    _DRCxpos = other._DRCxpos;
    _DCHdedx = other._DCHdedx;
    _SVTdedx = other._SVTdedx;
    _SVTPidInfo = other._SVTPidInfo;
    _DCHPidInfo = other._DCHPidInfo;
    _DRCPidInfo = other._DRCPidInfo;
    _pattern = other._pattern; 
    _TRKst = other._TRKst;
    _DCHsamples = other._DCHsamples;
    _SVTsamples = other._SVTsamples;
    _EMCintertheta = other._EMCintertheta;
    _EMCinterphi = other._EMCinterphi;
}

PAFAodTrack::~PAFAodTrack()  
{ 
}

//Detector status ( = subsystem with hits) __________

Bool_t PAFAodTrack::SvtOk() const {
    return ((_pattern&1)==1);
}

Bool_t PAFAodTrack::DchOk() const {
    return ((_pattern&2)==2);
}

Bool_t PAFAodTrack::DrcOk() const {
    return ((_pattern&4)==4);
}


void PAFAodTrack::SetTrackStatus(Bool_t svt,Bool_t dch, Bool_t drc)
{
    _pattern=0;
    if (svt) { _pattern |=1; }
    if (dch) { _pattern |=2; }
    if (drc) { _pattern |=4; }
}

//BtaTrkQual___________________________________________________________

void PAFAodTrack::SetSvtHitPattern(Int_t t) {
    _SVThitpattern=(Byte_t)t;
}

void PAFAodTrack::SetSvtHits(Int_t t) {
    _SVThits=(Byte_t)t;
}

void PAFAodTrack::SetDchInnerLayer(Int_t t) {
    _DCHinnerlayer=(Byte_t)t;
}

void PAFAodTrack::SetDchOuterLayer(Int_t t) {
    _DCHouterlayer=(Byte_t)t;
}

void PAFAodTrack::SetDchHits(Int_t t) {
    _DCHhits=(Byte_t)t;
}

void PAFAodTrack::SetChargedPidHypo(Int_t t) {
    _TRKpidhypo=(Char_t)t;
}

void PAFAodTrack::SetDegreesOfFreedom(Int_t t) {
    _TRKndof=(Short_t)t;
}

void PAFAodTrack::SetProbability(Float_t t) {
    _TRKprob=t;
}

void PAFAodTrack::SetChiSquared(Float_t t) {
    _TRKchi2=t;
}

void PAFAodTrack::SetTrackLength(Float_t t) {
    _TRKtlen=t;
}

void PAFAodTrack::SetTrackStartFoundRange(Float_t t) {
    _TRKst=t;
}

Int_t PAFAodTrack::GetSvtHitPattern() const {
    return _SVThitpattern;
}

Int_t PAFAodTrack::GetSvtHits() const {
    return _SVThits;
}

Int_t PAFAodTrack::GetDchInnerLayer() const {
    return _DCHinnerlayer;
}

Int_t PAFAodTrack::GetDchOuterLayer() const {
    return _DCHouterlayer;
}

Int_t PAFAodTrack::GetDchHits() const {
    return _DCHhits;
}

Int_t PAFAodTrack::GetChargedPidHypo() const {
    return _TRKpidhypo;
}

Int_t PAFAodTrack::GetDegreesOfFreedom() const {
    return _TRKndof;
}

Int_t PAFAodTrack::GetFitStatus() const {
    return _TRKfitstatus;
}

Float_t PAFAodTrack::GetProbability() const {
    return _TRKprob;
}

Float_t PAFAodTrack::GetChiSquared() const {
    return _TRKchi2;
}

Float_t PAFAodTrack::GetTrackLength() const {
    return _TRKtlen;
}

Float_t PAFAodTrack::GetTrackStartFoundRange() const {
    return _TRKst;
}


//BtaPidQual___________________________________________________________

void PAFAodTrack::SetDrcThetaC(Float_t t) {
    _DRCthetaC=t;
}

void PAFAodTrack::SetDrcThetaCError(Float_t t) {
    _DRCethetaC=t;
}

void PAFAodTrack::SetFitStatus(Int_t stat) {
    _TRKfitstatus = (Short_t) stat;
}

// TB 21.02.00 :  'DRCstatus' exchanged by DRCnumbar,DRCexit and DRCxpos (for 8.5.x) 
//void PAFAodTrack::SetDrcStatus(Int_t t) {
//    _DRCstatus=(Short_t)t;
//}

void PAFAodTrack::SetDrcNumBar(Int_t t) {
  _DRCnumbar = (Short_t) t;
}

void PAFAodTrack::SetDrcExit(Int_t t) {
  _DRCexit = (Short_t) t;
}

void PAFAodTrack::SetDrcXpos(Float_t t) {
  _DRCxpos = t;
}

void PAFAodTrack::SetDrcNumberOfPhotons(Int_t nphot) {
    _DRCnphot = nphot; 
}

void PAFAodTrack::SetDrcNumberOfBackgroundPhotons(Int_t nphot) {
    _DRCnbkgd = nphot; 
}

void PAFAodTrack::SetDrcNumberOfExpectedPhotons(Int_t nphot, Int_t hypo) {
    _DRCnexp[hypo] =  (Short_t) nphot; 
}

void PAFAodTrack::SetDchDeltaMomentum(Float_t t) {
    _DCHmom=t;
}

void PAFAodTrack::SetDrcDeltaMomentum(Float_t t) {
    _DRCmom=t;
}

void PAFAodTrack::SetDrcPhiAtEmc(Float_t t) {
    _EMCinterphi=t;
}

void PAFAodTrack::SetDrcThetaAtEmc(Float_t t) {
    _EMCintertheta=t;
}
 
void PAFAodTrack::SetDchMeanDEdx(Float_t t) {
    _DCHdedx=t;
}

void PAFAodTrack::SetSvtMeanDEdx(Float_t t) {
    _SVTdedx=t;
}

void PAFAodTrack::SetDchSamples(Int_t t) {
    _DCHsamples=t;
}

void PAFAodTrack::SetSvtSamples(Int_t t) {
    _SVTsamples=t;
}

void PAFAodTrack::SetEmcInterTheta(Float_t f) {
  _EMCintertheta=f;
}

void PAFAodTrack::SetEmcInterPhi(Float_t f) {
  _EMCinterphi=f;
}

Float_t PAFAodTrack::GetDrcThetaC() const {
    return _DRCthetaC;
}

Float_t PAFAodTrack::GetDrcThetaCError() const {
    return _DRCethetaC;
}

Float_t PAFAodTrack::GetDchDeltaMomentum() const {
    return _DCHmom;
}

Float_t PAFAodTrack::GetDrcDeltaMomentum() const {
    return _DRCmom;
}

Int_t PAFAodTrack::GetDrcNumberOfPhotons() const {
    return _DRCnphot;
}

Int_t PAFAodTrack::GetDrcNumberOfBackgroundPhotons() const {
     return _DRCnbkgd;
}

Int_t PAFAodTrack::GetDrcNumBar() const {
    return _DRCnumbar;
}

Int_t PAFAodTrack::GetDrcExit() const {
    return _DRCexit;
}

Float_t PAFAodTrack::GetDrcXpos() const {
    return _DRCxpos;
}

Int_t PAFAodTrack::GetDrcNumberOfExpectedPhotons(Int_t hypo) const {
    return _DRCnexp[hypo];
}

Float_t PAFAodTrack::GetDrcPhiAtEmc() const {
    return _EMCinterphi;
}

Float_t PAFAodTrack::GetDrcThetaAtEmc() const {
    return _EMCintertheta;
}

Float_t PAFAodTrack::GetDchMeanDEdx() const {
    return _DCHdedx;
}

Float_t PAFAodTrack::GetSvtMeanDEdx() const {
    return _SVTdedx;
}

Int_t PAFAodTrack::GetDchSamples() const {
    return _DCHsamples;
}

Int_t PAFAodTrack::GetSvtSamples() const {
    return _SVTsamples;
}

Float_t PAFAodTrack::GetEmcInterPhi() const {
    return _EMCinterphi;
}


Float_t PAFAodTrack::GetEmcInterTheta() const {
    return _EMCintertheta;
}

//PidInfo___________________________________________________________
void
PAFAodTrack::SetDrcPidInfo(PAFAbsPidInfo& p)
{
    _DRCPidInfo = p;
}

void
PAFAodTrack::SetDchPidInfo(PAFAbsPidInfo& p)
{
    _DCHPidInfo = p;
}

void 
PAFAodTrack::SetSvtPidInfo(PAFAbsPidInfo& p)
{
    _SVTPidInfo = p;
}

const PAFAbsPidInfo&
PAFAodTrack::GetDchPidInfo() const
{
    return _DCHPidInfo;
}

const PAFAbsPidInfo&
PAFAodTrack::GetDrcPidInfo() const
{  
    return _DRCPidInfo;
}

const PAFAbsPidInfo&
PAFAodTrack::GetSvtPidInfo() const
{
    return _SVTPidInfo;
}

void
PAFAodTrack::PrintOn(std::ostream &o) const {
    if (_pattern&1) {
	o << " SVT #hits                        : " << (Int_t) GetSvtHits() << endl;
	o << " SVT pattern                      : " << (Int_t) GetSvtHitPattern() << endl;
	o << " SVT dE/dx                        : " << GetSvtMeanDEdx() << endl;
    } else {
	o << " SVT : no track " << endl;
    }
    
    if (_pattern&2) {
	o << " DCH #hits                        : " << (Int_t) GetDchHits() << endl;
	o << " DCH inner/outer layer            : " << (Int_t) GetDchInnerLayer() << " / " << (Int_t) GetDchOuterLayer()  << endl;
	o << " DCH dE/dx                        : " << GetDchMeanDEdx() << endl; 
	o << " DCH momentum correction          : " << GetDchDeltaMomentum() << endl; 
    } else {
	o << " DCH : no track  " << endl;
    }
    
    o << " TRK pid hypo                     : " << (Int_t) GetChargedPidHypo() << endl;
    o << " TRK ndof                         : " << GetDegreesOfFreedom() << endl;
    o << " TRK chi**2                       : " << GetChiSquared() << endl;
    o << " TRK length                       : " << GetTrackLength() << endl;
    o << " TRK start found range            : " << GetTrackStartFoundRange() << endl;
    
    if (_pattern&4) {
	o << " DRC thetaC                     : " << GetDrcThetaC() << endl;
	o << " DRC thetaC error               : " << GetDrcThetaCError() << endl;
	o << " DRC number of photons          : " << GetDrcNumberOfPhotons() << endl;
	o << " DRC number of bkgd photons     : " << GetDrcNumberOfBackgroundPhotons() << endl;
	o << " DRC expected Photons (e)       : " << GetDrcNumberOfExpectedPhotons(0) << endl;
	o << " DRC expected Photons (mu)      : " << GetDrcNumberOfExpectedPhotons(1) << endl;
	o << " DRC expected Photons (pi)      : " << GetDrcNumberOfExpectedPhotons(2) << endl;
	o << " DRC expected Photons (k)       : " << GetDrcNumberOfExpectedPhotons(3) << endl;
	o << " DRC expected Photons (p)       : " << GetDrcNumberOfExpectedPhotons(4) << endl;
	o << " DRC Entance Bar                : " << GetDrcNumBar() << endl;
	o << " DRC Exit    Bar                : " << GetDrcExit() << endl;
	o << " DRC xpos                       : " << GetDrcXpos() << endl;
	o << " DRC delta Momentum             : " << GetDrcDeltaMomentum() << endl;
    } else {
	o << " DRC : no track " << endl;
    }
    o << " EMC - TRK intersection : theta = " << GetDrcThetaAtEmc() << " phi = " << _EMCinterphi << endl;
    GetDchPidInfo().PrintOn(cout);
    GetSvtPidInfo().PrintOn(cout);
    GetDrcPidInfo().PrintOn(cout);
    
    
}

std::ostream&  operator << (std::ostream& o, const PAFAodTrack& a) { a.PrintOn(o); return o; }



