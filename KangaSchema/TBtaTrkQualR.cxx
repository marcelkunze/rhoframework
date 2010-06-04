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
//	Copyright (C) 1999       LBNL, TU-Dresden
//
//------------------------------------------------------------------------

#include "KangaSchema/TBtaTrkQualR.h"
#include "KangaSchema/kanga.h"

ClassImp(TBtaTrkQualR)

TBuffer &operator>>(TBuffer &buf, TBtaTrkQualR *&obj)
{
   obj = (TBtaTrkQualR *) buf.ReadObject(TBtaTrkQualR::Class());
   return buf;
}

ComPackNonFlatFloat TBtaTrkQualR::Packer(ComPackNonFlatFloat::UNSIG);
ComPackNonFlatFloat TBtaTrkQualR::Packer8(ComPackNonFlatFloat::UNSIG_8);
ComPackFlatFloat TBtaTrkQualR::startPacker(-10,100,8);

TBtaTrkQualR::TBtaTrkQualR()
{
    Reset();
}

TBtaTrkQualR::~TBtaTrkQualR() {}

TBtaTrkQualR & TBtaTrkQualR::operator = ( const TBtaTrkQualR & rhs ) 
{
    _svtPattern= rhs._svtPattern;
    _nSvtHits= rhs._nSvtHits;
    _firstDch= rhs._firstDch;
    _lastDch= rhs._lastDch;
    _nDchHits= rhs._nDchHits;
    _pidHypo= rhs._pidHypo;
    _fitStatus= rhs._fitStatus;
    _chi2= rhs._chi2;
    _nDof= rhs._nDof;
    _tLen= rhs._tLen;
    return *this;
}

void TBtaTrkQualR::Reset()
{
    for (int i=0; i<nByteVars; i++) bytes[i]= 0;
    for (int j=0; j<nShortVars; j++) shorts[j]= 0;
    _svtPattern= 0; 
    _nSvtHits= 0;
    _firstDch= 0; 
    _lastDch= 0;
    _nDchHits= 0;
    _pidHypo= 0;
    _fitStatus= 0;
    _chi2= 0;
    _nDof= 0; 
    _tLen= 0;
}

UInt_t TBtaTrkQualR::GetSvtPattern()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::tq_shorts);
	_svtPattern = shorts[svtPattern]&0x3ff;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::tq_svtPattern);
    }
    return _svtPattern;
}

UShort_t TBtaTrkQualR::GetNSvtHits()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::tq_bytes);
	_nSvtHits = ( bytes[svtPidStatus] >> 4) & 15;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::tq_nSvtHits);
    }
    return _nSvtHits;
}

UShort_t TBtaTrkQualR::GetFirstDch()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::tq_bytes);
	_firstDch = bytes[firstDch];
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::tq_firstDch);
    }
    return _firstDch;
}

UShort_t TBtaTrkQualR::GetLastDch()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::tq_bytes);
	_lastDch = bytes[lastDch];
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::tq_lastDch);
    }
    return _lastDch;
}

UShort_t TBtaTrkQualR::GetNDchHits()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::tq_bytes);
	_nDchHits = bytes[nDchHits];
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::tq_nDchHits);
    }
    return _nDchHits;
}

UShort_t TBtaTrkQualR::GetPidHypo()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::tq_bytes);
	_pidHypo = ((( bytes[svtPidStatus] >> 1) & 7) - 1);
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::tq_pidHypo);
    }
    return _pidHypo;
}

UShort_t TBtaTrkQualR::GetFitStatus()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::tq_bytes);
	_fitStatus = (bytes[svtPidStatus] & 1);
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::tq_fitStatus);
    }
    return _fitStatus;
}

Float_t TBtaTrkQualR::GetChi2()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::tq_shorts);
	double d;
	Packer.unpackAndScream(0,1000,shorts[chi2],d, "Chi2" );
	_chi2 = d;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::tq_chi2);
    }
    return _chi2;
}

Int_t TBtaTrkQualR::GetNDof()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::tq_bytes);
	_nDof = 0xff&bytes[nDof];
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::tq_nDof);
    }
    return _nDof;
}

Float_t TBtaTrkQualR::GetTLen()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::tq_bytes);
	double d;
	d_UShort u= bytes[tLen];
	Packer8.unpackAndScream(0,200,u,d, "tLen" );
	_tLen = d;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::tq_tLen);
    }
    return _tLen;
}

Float_t TBtaTrkQualR::GetStFound()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::tq_bytes);
	double d;
	startPacker.unpack(bytes[stFound],d);
	_stFound = d;
    }
    else {
	_stFound = 0.0;
    }
    return _stFound;
}
