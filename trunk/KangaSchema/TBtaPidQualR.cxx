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
//	Copyright (C) 1999       LBNL, TU Dresden
//
//------------------------------------------------------------------------

#include "KangaSchema/TBtaPidQualR.h"
#include "KangaSchema/kanga.h"
#include "TMath.h"

ClassImp(TBtaPidQualR)

TBuffer &operator>>(TBuffer &buf, TBtaPidQualR *&obj)
{
   obj = (TBtaPidQualR *) buf.ReadObject(TBtaPidQualR::Class());
   return buf;
}

ComPackNonFlatFloat TBtaPidQualR::Packer(ComPackNonFlatFloat::UNSIG);
ComPackNonFlatFloat TBtaPidQualR::Packer8(ComPackNonFlatFloat::UNSIG_8);
ComPackFlatFloat TBtaPidQualR::anglePacker(0,1,16);
ComPackFlatFloat TBtaPidQualR::packX(-2.,2.,6);
ComPackFlatFloat TBtaPidQualR::thetaPack(0,TMath::Pi(),16);
ComPackFlatFloat TBtaPidQualR::phiPack(-TMath::Pi(),TMath::Pi(),16);
ComPackUFloat TBtaPidQualR::packdEdX(FRACLEN,EXPLEN,BIAS);

TBtaPidQualR::TBtaPidQualR()
{
    Reset();
}

TBtaPidQualR::~TBtaPidQualR() {}

void TBtaPidQualR::Reset()
{
    for (int i=0; i<nByteVars; i++) bytes[i]= 0;
    for (int j=0; j<nShortVars; j++) shorts[j]= 0;
    _thetaDirc= 0; 
    _eThetaDirc= 0; 
    _dEdXDch= 0;
    _dEdXSvt= 0;
    _nSamplesDch= 0;
    _nSamplesSvt= 0;
    _deltaDchMom= 0;
    _drcNumBar= 0;
    _drcExit= 0;
    _drcXPos= 0;
    _deltaDrcMom= 0;
    _trkEmcTheta= 0;
    _trkEmcPhi= 0;
    for (int k=0;k<5;k++) _nExPhot[k] = 0;
}

TBtaPidQualR & TBtaPidQualR::operator = ( const TBtaPidQualR & rhs ) {
    
    _thetaDirc= rhs._thetaDirc;
    _eThetaDirc= rhs._eThetaDirc;
    _dEdXDch= rhs._dEdXDch;
    _dEdXSvt= rhs._dEdXSvt;
    _nSamplesDch= rhs._nSamplesDch;
    _nSamplesSvt= rhs._nSamplesSvt;
    _drcNumBar= rhs._drcNumBar;
    _drcExit= rhs._drcExit;
    _drcXPos= rhs._drcXPos;
    _trkEmcTheta= rhs._trkEmcTheta;
    _trkEmcPhi= rhs._trkEmcPhi;
    for (int i=0;i<5;i++) _nExPhot[i] = rhs._nExPhot[i];
    return *this;
    
}

Float_t TBtaPidQualR::GetThetaDirc()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_shorts);
	double d;
	anglePacker.unpack(shorts[thetaDirc],d);
	_thetaDirc = d;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_thetaDirc);
    }
    return _thetaDirc;
}

Float_t TBtaPidQualR::GetEThetaDirc()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_shorts);
	double d;
	Packer.unpackAndScream(0,1,shorts[eThetaDirc],d, "eThetaDirc" );
	_eThetaDirc = d;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_eThetaDirc);
    }
    return _eThetaDirc;
}

Float_t TBtaPidQualR::GetDEdXDch()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_shorts);
	d_UShort u = shorts[dEdXDch]&0x3ff;
	double d;
	packdEdX.unpack(u,d);
	_dEdXDch = d * 10000.;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_dEdXDch);
    }
    return _dEdXDch;
}

Float_t TBtaPidQualR::GetDEdXSvt()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_shorts);
	d_UShort u = shorts[dEdXSvt]&0x3ff;
	double d;
	packdEdX.unpack(u,d);
	_dEdXSvt = d * 100.;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_dEdXSvt);
    }
    return _dEdXSvt;
}

UShort_t TBtaPidQualR::GetNSamplesDch()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_shorts);
	_nSamplesDch = (shorts[dEdXDch]>>10)&0x3f;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_nSamplesDch);
    }
    return _nSamplesDch;
}

UShort_t TBtaPidQualR::GetNSamplesSvt()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_shorts);
	_nSamplesSvt = (shorts[dEdXSvt]>>10)&0x3f;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_nSamplesSvt);
    }
    return _nSamplesSvt;
}

Int_t TBtaPidQualR::GetDrcInBar()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_shorts);
	_drcNumBar = shorts[statusDRC]&0xff;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_drcNumBar);
    }
    return _drcNumBar;
}

Int_t TBtaPidQualR::GetDrcExitBar()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_shorts);
	_drcExit = (shorts[statusDRC]>>8)&0x3;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_drcExit);
    }
    return _drcExit;
}

Float_t TBtaPidQualR::GetDrcXPos()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_shorts);
	d_ULong packedX = (shorts[statusDRC]>>10)&0x3f;
	double xPos;
	packX.unpack(packedX,xPos);
	_drcXPos = xPos;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_drcXPos);
    }
    return _drcXPos;
}

Int_t TBtaPidQualR::GetNPhot()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_bytes);
	_nPhot = 0xff&bytes[nPhot];
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_nPhot);
    }
    return _nPhot;
}

Int_t TBtaPidQualR::GetNBkgd()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_bytes);
	_nBkgd = 0xff&bytes[nBkgd];
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_nBkgd);
    }
    return _nBkgd;
}

Short_t TBtaPidQualR::GetNExPhot(UShort_t PidHypo)
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_bytes);
	_nExPhot[PidHypo] = 0xff&bytes[nExPhot0+PidHypo];
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_nExPhot);
    }
    return _nExPhot[PidHypo];
}

Float_t TBtaPidQualR::GetDeltaDchMom()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_bytes);
	double dchMom;
	d_UShort u= bytes[deltaDchMom];
	Packer8.unpackAndScream(0,0.1,u,dchMom, "dchMom" );
	_deltaDchMom = -dchMom;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_deltaDchMom);
    }
    return _deltaDchMom;
}

Float_t TBtaPidQualR::GetDeltaDrcMom()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_bytes);
	double drcMom;
	d_UShort u = bytes[deltaDrcMom];
	Packer8.unpackAndScream(0,0.1,u,drcMom, "drcMom" );
	_deltaDrcMom = -drcMom;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_deltaDrcMom);
    }
    return _deltaDrcMom;
}

Float_t TBtaPidQualR::GetDrcPhiAtEmc()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_shorts);
	double phi;
	thetaPack.unpack(shorts[trkEmcPhi],phi);
	_trkEmcPhi = phi;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_trkEmcPhi);
    }
    return _trkEmcPhi;
}

Float_t TBtaPidQualR::GetDrcThetaAtEmc()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_shorts);
	double theta;
	thetaPack.unpack(shorts[trkEmcTheta],theta);
	_trkEmcTheta = theta;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::pq_trkEmcTheta);
    }
    return _trkEmcTheta;
}
