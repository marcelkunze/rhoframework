//--------------------------------------------------------------------------
//
// Description:
//      summarizes the information on the quality of the track reconstruction
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

#include "KangaSchema/TBtaCalQualR.h"
#include "KangaSchema/kanga.h"

ClassImp(TBtaCalQualR)

TBuffer &operator>>(TBuffer &buf, TBtaCalQualR *&obj)
{
   obj = (TBtaCalQualR *) buf.ReadObject(TBtaCalQualR::Class());
   return buf;
}

#include <iostream>
using namespace std;

ComPackNonFlatFloat TBtaCalQualR::Packer(ComPackNonFlatFloat::UNSIG);
ComPackFlatFloat TBtaCalQualR::PackerToOne(0,1,8);
ComPackFlatFloat TBtaCalQualR::PackerToLess(0,0.05,8);
ComPackFlatFloat TBtaCalQualR::cntPack(-500,500,16);
ComPackExpFloat TBtaCalQualR::xpack( 8, 3, 0., 1. );

TBtaCalQualR::TBtaCalQualR()
{
    Reset();
}

TBtaCalQualR::~TBtaCalQualR() {}

TBtaCalQualR & TBtaCalQualR::operator = ( const TBtaCalQualR & rhs ) 
{
    _pidHypo= rhs._pidHypo;
    _nBumps= rhs._nBumps;
    _nCrystals= rhs._nCrystals;
    _lat= rhs._lat;
    _absZern42= rhs._absZern42;
    _absZern20= rhs._absZern20;
    _absZern31= rhs._absZern31;
    _status= rhs._status;
    _secondMomentTP= rhs._secondMomentTP;
    _energy= rhs._energy;
    _rawEnergy= rhs._rawEnergy;
    _cdPhiValue= rhs._cdPhiValue;
    _cdPhiLhood= rhs._cdPhiLhood;
    _cdPhiBits= rhs._cdPhiBits;
    _centroidX= rhs._centroidX;
    _centroidY= rhs._centroidY;
    _centroidZ= rhs._centroidZ;
    for( unsigned i= 0; i < 10; i++ ) _cov[i]= rhs._cov[i];
    return *this;
}

void TBtaCalQualR::Reset()
{
    for (int i=0; i<nByteVars; i++) bytes[i]= 0;
    for (int j=0; j<nShortVars; j++) shorts[j]= 0;
    _pidHypo= 0;
    _nBumps= 0;
    _nCrystals= 0;
    _lat= 0;
    _absZern42= 0;
    _absZern20= 0;
    _absZern31= 0;
    _status= 0;
    _secondMomentTP= 0;
    _energy= 0;
    _rawEnergy= 0;
    _cdPhiValue= 0;
    _cdPhiLhood= 0;
    _cdPhiBits= 0;
    _centroidX= 0;
    _centroidY= 0;
    _centroidZ= 0;
    for( unsigned k= 0; k < 10; k++ ) _cov[k]= 0.0;
}

// Getters:

UShort_t TBtaCalQualR::GetPidHypo()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_bytes);
	_pidHypo = (bytes[bumpsPID] & 7) - 1;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_pidHypo);
    }
    return _pidHypo;
}

UShort_t TBtaCalQualR::GetNBumps()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_bytes);
	_nBumps = ((bytes[bumpsPID] >> 4) & 15);
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_nBumps);
    }
    return _nBumps;
}

UShort_t TBtaCalQualR::GetNCrystals()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_bytes);
	_nCrystals = bytes[nCrystals];
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_nCrystals);
    }
    return _nCrystals;
}

Float_t TBtaCalQualR::GetLat()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_bytes);
	double dummy;
	PackerToOne.unpack(bytes[lat],dummy);
	_lat = dummy;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_lat);
    }
    return _lat;
}

Float_t TBtaCalQualR::GetAbsZern42()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_bytes);
	double dummy;
	PackerToOne.unpack(bytes[absZern42],dummy);
	_absZern42 = dummy;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_absZern42);
    }
    return _absZern42;
}

Float_t TBtaCalQualR::GetAbsZern20()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_bytes);
	double dummy;
	PackerToOne.unpack(bytes[absZern20],dummy);
	_absZern20 = dummy;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_absZern20);
    }
    return _absZern20;
}

Float_t TBtaCalQualR::GetAbsZern31()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	_absZern31 = 0.0; // N/A
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_absZern31);
    }
    return _absZern31;
}

Float_t TBtaCalQualR::GetStatus()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_bytes);
	_nCrystals = bytes[emcStatus];
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_status);
    }
    return _status;
}

Float_t TBtaCalQualR::GetSecondMomentTP()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_bytes);
	double dummy;
	PackerToLess.unpack( bytes[secondMomentTP],dummy);
	_secondMomentTP = dummy;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_secondMomentTP);
    }
    return _secondMomentTP;
}

Float_t TBtaCalQualR::GetEnergy()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_shorts);
	double dummy;
	Packer.unpackAndScream(0,50,shorts[energy] ,dummy , "Energy" );
	_energy = dummy;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_energy);
    }
    return _energy;
}

Float_t TBtaCalQualR::GetRawEnergy()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_shorts);
	double dummy;
	Packer.unpackAndScream(0,50,shorts[rawEnergy] ,dummy , "Raw Energy" );
	_rawEnergy = dummy;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_rawEnergy);
    }
    return _rawEnergy;
}

Float_t TBtaCalQualR::GetCdPhiValue()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_bytes);
	double cons= 0;
	d_ULong l= bytes[cdPhiValue];
	xpack.unpack( l , cons );
	_cdPhiValue = cons;
    }
    else {
        TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_cdPhiValue);
    }
    return _cdPhiValue;
}

Float_t TBtaCalQualR::GetCdPhiLhood()
{
    TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_cdPhiLhood);
    return _cdPhiLhood;
}

UChar_t TBtaCalQualR::GetCdPhiBits()
{
    TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_cdPhiBits);
    return _cdPhiBits;
}

Float_t TBtaCalQualR::GetCentroidX()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_shorts);
	double dummy;
	cntPack.unpack(shorts[centroidX],dummy);
	_centroidX = dummy;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_centroidX);
    }
    return _centroidX;
}

Float_t TBtaCalQualR::GetCentroidY()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_shorts);
	double dummy;
	cntPack.unpack(shorts[centroidY],dummy);
	_centroidY = dummy;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_centroidY);
    }
    return _centroidY;
}

Float_t TBtaCalQualR::GetCentroidZ()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_shorts);
	double dummy;
	cntPack.unpack(shorts[centroidZ],dummy);
	_centroidZ = dummy;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_centroidZ);
    }
    return _centroidZ;
}

void TBtaCalQualR::GetCov(Float_t cov[10])
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	for (Int_t i=0; i<10; i++) cov[i] = 0.0; // N/A
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_cov);
	for (Int_t i=0; i<10; i++) cov[i]= _cov[i];
    }
}

Float_t TBtaCalQualR::GetS9S25()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_bytes);
	double dummy;
	PackerToOne.unpack(bytes[s9s25],dummy);
	_s9s25 = dummy;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_s9s25);
    }
    return _s9s25;
}

Float_t TBtaCalQualR::GetS1S9()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_bytes);
	double dummy;
	PackerToOne.unpack(bytes[s1s9],dummy);
	_s1s9 = dummy;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::cq_s1s9);
    }
    return _s1s9;
}

// Setters:

void TBtaCalQualR::SetCov(const Float_t cov[10])
{
    for (Int_t i=0; i<10; i++) _cov[i]= cov[i];
}
