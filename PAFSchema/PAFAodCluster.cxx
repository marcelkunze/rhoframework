//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFAodCluster							//
//                                                                      //
// Definition of the Analysis Object Data database			//
//                                                                      //
// Author: Marcel Kunze, Bochum University, March 1999			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "PAFSchema/PAFAodCluster.h"
#include "TTree.h"
#include "TVector3.h"

ClassImp(PAFAodCluster)

#include <iostream>
using namespace std;

PAFAodCluster::PAFAodCluster() :
_EMCcrystals(0),
_EMCbumps(0), 
_EMCenergy(0), 
_EMClateralshape(0),
_EMC20(0),
_EMC31(0), 
_EMC42(0),
_EMCsecmom(0), 
_EMCconsistency(0),
_EMCcalenergy(0),
_EMCstatus(0),
_EMCpidhypo(0), 
_EMCcx(0),
_EMCcy(0),
_EMCcz(0),
_EMCcovenergy(0),
_EMCcovtheta(0),
_EMCcovphi(0), 
_EMCcovrho(0),
_IFRlayers(0),
_IFRstrips(0),
_IFRpattern(0), 
_IFRclusters(0),
_IFRfirsthit(0),
_IFRlasthit(0),
_IFRiron(0),
_IFRtrk(0),
_IFRemc(0),
_IFRtrkmatch(0),
_IFRemcmatch(0),
_IFRrpcMuProb(-1.0),
_IFRclusfitChi2(0),
_cpattern(0),
_EMCPidInfo(),
_IFRPidInfo(),
_EMCs9s25(0),
_EMCs1s9(0),
_EMClikelihood(0),
_EMCbits(0),
_IFRlastbar(0)
{
    Int_t i;    
    for (i=0;i<20;i++) _IFRlaystrips[i] = 0;    
}

PAFAodCluster::PAFAodCluster(PAFAodCluster &other)
{
    // copy Data
    _EMCcrystals = other._EMCcrystals;
    _EMCbumps = other._EMCbumps;
    _EMCenergy = other._EMCenergy;
    _EMClateralshape = other._EMClateralshape;
    _EMC20 = other._EMC20;
    _EMC31 = other._EMC31;
    _EMC42 = other._EMC42;
    _EMCsecmom = other._EMCsecmom;
    _EMCconsistency = other._EMCconsistency;
    _EMCcalenergy = other._EMCcalenergy;
    _EMCstatus = other._EMCstatus;
    _EMCpidhypo = other._EMCpidhypo;
    _EMCcx = other._EMCcx;
    _EMCcy = other._EMCcy;
    _EMCcz = other._EMCcz;
    _EMCcovenergy = other._EMCcovenergy;
    _EMCcovtheta = other._EMCcovtheta;
    _EMCcovphi = other._EMCcovphi;
    _EMCcovrho = other._EMCcovrho;
    _IFRlayers = other._IFRlayers;
    _IFRstrips = other._IFRstrips;
    _IFRpattern = other._IFRpattern;
    _IFRclusters = other._IFRclusters;
    _IFRfirsthit = other._IFRfirsthit;
    _IFRlasthit = other._IFRlasthit;
    _IFRiron = other._IFRiron;
    _IFRtrk = other._IFRtrk;
    _IFRemc = other._IFRemc;
    _IFRtrkmatch = other._IFRtrkmatch;
    _IFRemcmatch = other._IFRemcmatch;
    for (int i=0; i<20; i++) {
	_IFRlaystrips[i] = other._IFRlaystrips[i];
    }
    _IFRrpcMuProb = other._IFRrpcMuProb;
    _IFRclusfitChi2 = other._IFRclusfitChi2;
    _cpattern = other._cpattern;
    _EMCPidInfo = other._EMCPidInfo;
    _IFRPidInfo = other._IFRPidInfo;
    _EMCs9s25 = other._EMCs9s25;
    _EMCs1s9 = other._EMCs1s9;
    _EMClikelihood = other._EMClikelihood;
    _EMCbits = other._EMCbits;
    _IFRlastbar = other._IFRlastbar;
}

PAFAodCluster::~PAFAodCluster()  
{ 
}

//Detector status ( = subsystem with hits) __________


Bool_t PAFAodCluster::EmcOk() const {
    return ((_cpattern&8)==8);
}

Bool_t PAFAodCluster::IfrOk() const {
    return ((_cpattern&16)==16);
}

void PAFAodCluster::SetClusterStatus(Bool_t emc, Bool_t ifr) {
    _cpattern=0;
    if (emc) { _cpattern |=8; }
    if (ifr) { _cpattern |=16; }
}


//BtaCalQual___________________________________________________________
void PAFAodCluster::SetEmcNumberOfCrystals(Int_t t) {
    _EMCcrystals=(Short_t)t;
}

void PAFAodCluster::SetEmcNumberOfBumps(Int_t t) {
    _EMCbumps=(Byte_t)t;
}

void PAFAodCluster::SetEmcRawEnergy(Float_t t) {
    _EMCenergy=t;
}

void PAFAodCluster::SetEmcCalEnergy(Float_t t) {
    _EMCcalenergy=t;
}

void PAFAodCluster::SetEmcCovarianceEnergy(Float_t t) {
    _EMCcovenergy=t;
}

void PAFAodCluster::SetEmcCovarianceTheta(Float_t t) {
    _EMCcovtheta=t;
}

void PAFAodCluster::SetEmcCovariancePhi(Float_t t) {
    _EMCcovphi=t;
}

void PAFAodCluster::SetEmcCovarianceRho(Float_t t) {
    _EMCcovrho=t;
}

void PAFAodCluster::SetEmcS9S25(Float_t t) {
    _EMCs9s25=t;
}

void PAFAodCluster::SetEmcS1S9(Float_t t) {
    _EMCs1s9=t;
}

void PAFAodCluster::SetEmcLateralShape(Float_t t) {
    _EMClateralshape=t;
}

void PAFAodCluster::SetEmcZernikeMoment20(Float_t t) {
    _EMC20=t;
}

void PAFAodCluster::SetEmcZernikeMoment31(Float_t t) {
    _EMC31=t;
}

void PAFAodCluster::SetEmcStatus(Int_t s)  {
    _EMCstatus=(Byte_t) (s&255);
}

void PAFAodCluster::SetEmcZernikeMoment42(Float_t t) {
    _EMC42=t;
}

void PAFAodCluster::SetEmcSecondMoment(Float_t t) {
    _EMCsecmom=t;
}

void PAFAodCluster::SetEmcCentroid(TVector3 t) {
    _EMCcx=(Float_t)t.X(); _EMCcy=(Float_t)t.Y(); _EMCcz=(Float_t)t.Z();
}

void PAFAodCluster::SetEmcConsistencyValue(Float_t t) {
    _EMCconsistency=t;
}

void PAFAodCluster::SetEmcConsistencyLikelihood(Float_t t) {
    _EMClikelihood=t;
}

void PAFAodCluster::SetEmcConsistencyBits(Int_t t) {
    _EMCbits=(Char_t)t;
}

void PAFAodCluster::SetNeutralPidHypo(Int_t t) {
    _EMCpidhypo=(Char_t)t;
}


Int_t PAFAodCluster::GetEmcNumberOfCrystals() const {
    return _EMCcrystals;
}

Int_t PAFAodCluster::GetEmcNumberOfBumps() const {
    return _EMCbumps;
}


Float_t PAFAodCluster::GetEmcRawEnergy() const  {
    return _EMCenergy;
}

Float_t PAFAodCluster::GetEmcCalEnergy() const  {
    return _EMCcalenergy;
}

Float_t PAFAodCluster::GetEmcCovarianceEnergy() const {
    return _EMCcovenergy;
}

Float_t PAFAodCluster::GetEmcCovarianceTheta() const {
    return _EMCcovtheta;
}

Float_t PAFAodCluster::GetEmcCovariancePhi() const {
    return _EMCcovphi;
}

Float_t PAFAodCluster::GetEmcCovarianceRho() const {
    return _EMCcovrho;
}

Float_t PAFAodCluster::GetEmcS9S25() const {
    return _EMCs9s25;
}

Float_t PAFAodCluster::GetEmcS1S9() const {
    return _EMCs1s9;
}

Float_t PAFAodCluster::GetEmcLateralShape() const {
    return _EMClateralshape;
}

Float_t PAFAodCluster::GetEmcZernikeMoment20() const {
    return _EMC20;
}

Float_t PAFAodCluster::GetEmcZernikeMoment31() const {
    return _EMC31;
}

Float_t PAFAodCluster::GetEmcZernikeMoment42() const {
   return _EMC42;
}

Int_t PAFAodCluster::GetEmcStatus() const {
    return _EMCstatus;
}

Float_t PAFAodCluster::GetEmcSecondMoment() const {
    return _EMCsecmom;
}

Float_t PAFAodCluster::GetEmcConsistencyValue() const {
   return _EMCconsistency;
}

Float_t PAFAodCluster::GetEmcConsistencyLikelihood() const {
    return _EMClikelihood;
}

Int_t PAFAodCluster::GetEmcConsistencyBits() const {
    return _EMCbits;
}

Int_t PAFAodCluster::GetNeutralPidHypo() const {
    return _EMCpidhypo;
}

TVector3
PAFAodCluster::GetEmcCentroid() const {
    return TVector3(_EMCcx,_EMCcy,_EMCcz);
}

//BtaIfrQual___________________________________________________________
void PAFAodCluster::SetIfrPattern(Int_t t) {
    _IFRpattern=(Byte_t)t;
}

void PAFAodCluster::SetIfrHitLayers(Int_t t) {
    _IFRlayers=(Byte_t)t;
}

void PAFAodCluster::SetIfrNumberOfStrips(Int_t t) {
    _IFRstrips=(Byte_t)t;
}

void PAFAodCluster::SetIfrNumberOfStrips(Int_t t,Int_t l) {
    _IFRlaystrips[l]=(Byte_t)t;
}

void PAFAodCluster::SetIfrFirstHit(Int_t t) {
    _IFRfirsthit=(Byte_t)t;
}

void PAFAodCluster::SetIfrLastHit(Int_t t) {
    _IFRlasthit=(Byte_t)t;
}

void PAFAodCluster::SetIfrClusters(Int_t t) {
    _IFRclusters=(Byte_t)t;
}

void PAFAodCluster::SetIfrMeasuredInteractionLengths(Float_t t) {
    _IFRiron=t;
}

void PAFAodCluster::SetIfrExpectedInteractionLengths(Float_t t) {
    _IFRtrk=t;
}

void PAFAodCluster::SetIfrInteractionLengthsBeforeIron(Float_t t) {
    _IFRemc=t;
}

void PAFAodCluster::SetIfrTrackMatchConsistency(Float_t t) {
    _IFRtrkmatch=t;
}

void PAFAodCluster::SetIfrEmcMatchConsistency(Float_t t) {
    _IFRemcmatch=t;
}

void PAFAodCluster::SetIfrLastBarrel(Int_t t) {
    _IFRlastbar=(Char_t)t;
}

void PAFAodCluster::SetIfrRpcMuProb(Float_t f) {
  _IFRrpcMuProb = f;
}

void PAFAodCluster::SetIfrClusfitChi2(Float_t f) {
  _IFRclusfitChi2 = f;
}

Int_t PAFAodCluster::GetIfrHitLayers() const {
    return _IFRlayers;
}

Int_t PAFAodCluster::GetIfrNumberOfStrips() const {
    return _IFRstrips;
}

Int_t PAFAodCluster::GetIfrPattern() const {
    return _IFRpattern;
}

Int_t PAFAodCluster::GetIfrClusters() const {
    return _IFRclusters;
}

Int_t PAFAodCluster::GetIfrFirstHit() const {
    return _IFRfirsthit;
}

Int_t PAFAodCluster::GetIfrLastHit() const {
    return _IFRlasthit;
}

Float_t PAFAodCluster::GetIfrMeasuredInteractionLengths() const {
    return _IFRiron;
}

Float_t PAFAodCluster::GetIfrExpectedInteractionLengths() const {
    return _IFRtrk;
}

Float_t PAFAodCluster::GetIfrInteractionLengthsBeforeIron() const {
    return _IFRemc;
}

Float_t PAFAodCluster::GetIfrTrackMatchConsistency() const {
    return _IFRtrkmatch;
}

Float_t PAFAodCluster::GetIfrEmcMatchConsistency() const {
    return _IFRemcmatch;
}

Int_t PAFAodCluster::GetIfrNumberOfStrips(Int_t n) const {
    return _IFRlaystrips[n];
}

Int_t PAFAodCluster::GetIfrLastBarrel() const {
    return _IFRlastbar;
}

Float_t PAFAodCluster::GetIfrRpcMuProb() const {
    return _IFRrpcMuProb;
}

Float_t PAFAodCluster::GetIfrClusfitChi2() const {
    return _IFRclusfitChi2;
}

// PidInfo ________________________________________________

const PAFAbsPidInfo& 
PAFAodCluster::GetEmcPidInfo() const {
    return _EMCPidInfo;
}

const PAFAbsPidInfo& 
PAFAodCluster::GetIfrPidInfo() const {
    return _IFRPidInfo;
}

void
PAFAodCluster::SetEmcPidInfo(PAFAbsPidInfo &p) {
    _EMCPidInfo = p;
}

void
PAFAodCluster::SetIfrPidInfo(PAFAbsPidInfo &p) {
    _IFRPidInfo = p;
}

void PAFAodCluster::PrintOn(std::ostream &o) const 
{
    if (_cpattern&8) {
	o << " EMC raw energy :                     : " << GetEmcRawEnergy() << endl;
	o << " EMC cal energy :                     : " << GetEmcCalEnergy() << endl;
	o << " Number of Crystals in EmcCluster : " << GetEmcNumberOfCrystals() << endl;
	o << " EMC lateral shape                : " << GetEmcLateralShape() << endl;
	o << " EMC Zernike moment (2,0)         : " << GetEmcZernikeMoment20() << endl;
	o << " EMC Zernike moment (3,1)         : " << GetEmcZernikeMoment31() << endl;
	o << " EMC Zernike moment (4,2)         : " << GetEmcZernikeMoment42() << endl;
	o << " EMC second moment                : " << GetEmcSecondMoment() << endl;
	TVector3 r = GetEmcCentroid();
	o << " EMC cluster position :           : (" << r.X() << ";" << r.Y() << ";" << r.Z() <<")" << endl;
    } else {
	o << " EMC : no Cluster " << endl;
    }
    
    if (_cpattern&16) {
	o << " IFR #layers hit                  : " << (Int_t) GetIfrHitLayers() << endl;
	o << " IFR #strips                      : " << (Int_t) GetIfrNumberOfStrips() << endl;
	o << " IFR clusters                     : " << (Int_t) GetIfrClusters() << endl;
	o << " IFR first/last hit layer         : " << (Int_t) GetIfrFirstHit() << " / " << (Int_t) _IFRlasthit << endl;
	o << " IFR expected                     : " << GetIfrExpectedInteractionLengths() << endl;
	o << " IFR measured                     : " << GetIfrMeasuredInteractionLengths() << endl;
	o << " IFR before iron                  : " << GetIfrInteractionLengthsBeforeIron() << endl;
	o << " IFR track match                  : " << GetIfrTrackMatchConsistency() << endl;
	o << " IFR EMC match                    : " << GetIfrEmcMatchConsistency() << endl;
	o << " IFR rpc muon probability         : " << GetIfrRpcMuProb() << endl;
	o << " IFR chi**2 of cluster fit        : " << GetIfrClusfitChi2() << endl;
	o << " IFR last barrel                  : " << GetIfrLastBarrel() << endl;
    } else {
	o << " IFR : no Cluster " << endl;
    }
    _EMCPidInfo.PrintOn(cout);
    _IFRPidInfo.PrintOn(cout);

} 

std::ostream&  operator << (std::ostream& o, const PAFAodCluster& a) { /* a.PrintOn(o) */ ; return o; }

