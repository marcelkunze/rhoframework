//--------------------------------------------------------------------------
//
// Description:
//      summarizes the information on the quality of the IFR
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

#include "KangaSchema/TBtaIfrQualR.h"
#include "KangaSchema/kanga.h"
#include "TMath.h"

ClassImp(TBtaIfrQualR)

TBuffer &operator>>(TBuffer &buf, TBtaIfrQualR *&obj)
{
   obj = (TBtaIfrQualR *) buf.ReadObject(TBtaIfrQualR::Class());
   return buf;
}

ComPackNonFlatFloat TBtaIfrQualR::Packer(ComPackNonFlatFloat::UNSIG_8);
ComPackFlatFloat TBtaIfrQualR::intlPack(0,10,8);
ComPackFlatFloat TBtaIfrQualR::intlPack2(0,3,8);
ComPackFlatFloat TBtaIfrQualR::Packer2(0,10,8);
ComPackNonFlatFloat TBtaIfrQualR::Packer8(ComPackNonFlatFloat::UNSIG_8);

TBtaIfrQualR::TBtaIfrQualR()
{
    Reset();
}

TBtaIfrQualR::~TBtaIfrQualR() {}

TBtaIfrQualR & TBtaIfrQualR::operator = ( const TBtaIfrQualR & rhs ) 
{
    for( unsigned i= 0; i < 20; i++ ) _IfrLayStrips[i]= rhs._IfrLayStrips[i];
    _IfrNstrip= rhs._IfrNstrip;
    _IfrBits= rhs._IfrBits;
    _expectedInteractionLengths= rhs._expectedInteractionLengths;
    _interactionLengthsBeforeIron= rhs._interactionLengthsBeforeIron;
    _measuredInteractionLengths= rhs._measuredInteractionLengths;
    _IfrTrkMatch= rhs._IfrTrkMatch;
    _IfrEmcMatch= rhs._IfrEmcMatch;
    _lastBarrel= rhs._lastBarrel;
    return *this;
}

void TBtaIfrQualR::Reset()
{
    for ( unsigned i= 0; i < 11; i++ ) _IfrPattern[i]= 0;
    for ( int j=0; j<nByteVars; j++) bytes[j]= 0;
    _IfrNstrip= 0;
    _IfrBits= 0;
    _expectedInteractionLengths= 0;
    _interactionLengthsBeforeIron= 0;
    _measuredInteractionLengths= 0;
    _IfrTrkMatch= 0;
    _IfrEmcMatch= 0;
    _lastBarrel= 0; 
    for( unsigned k= 0; k < 20; k++ ) _IfrLayStrips[k]= 0;
}

// Getters:

void TBtaIfrQualR::GetIfrLayStrips(UShort_t laystrips[20])
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_pattern);
	int ioff = 0;
	for(int layer=0;layer<20;layer++){
	int ipos = (layer+ioff)/2;
	int ishift = layer+ioff-2*ipos;
	int ival=(ishift == 0)?
	  _IfrPattern[ipos]&0xf :
	  (_IfrPattern[ipos]>>4)&0xf;
	// the strips is set to 15 at the end of the barrel
	if(ioff == 0 &&  (GetIfrBits() & 0x2) && ival == 15 )
	  { 
	    SetLastBarrel(layer);
	    layer-- ;
	    ioff=1;
	    continue;
	  }
	laystrips[layer] = ival;
	}
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_IfrLayStrips);
	for (Int_t i= 0; i<20; i++) laystrips[i]= _IfrLayStrips[i];
    }
}

UShort_t TBtaIfrQualR::GetIfrNstrip()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_bytes);
	_IfrNstrip = bytes[IfrNstrip] & 0xFF;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_IfrNstrip);
    }
    return _IfrNstrip;
}

UShort_t TBtaIfrQualR::GetIfrDigis()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_bytes);
	_IfrDigis = bytes[ifrDigis] & 0xFF;
    }
    else {
	_IfrDigis = 0;
    }
    return _IfrDigis;
}

UChar_t TBtaIfrQualR::GetIfrBits()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_bytes);
	_IfrBits = bytes[IfrBits];
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_IfrBits);
    }
    return _IfrBits;
}

Float_t TBtaIfrQualR::GetInteractionLengthsBeforeIron()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_bytes);
	double d;
	d_ULong l= bytes[intLenBI];
	intlPack2.unpack(l,d);
	_interactionLengthsBeforeIron = d;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_interactionLengthsBeforeIron);
    }
    return _interactionLengthsBeforeIron;
}

Float_t TBtaIfrQualR::GetExpectedInteractionLengths()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_bytes);
	double d;
	d_ULong l= bytes[expInt];
	intlPack.unpack(l,d);
	_expectedInteractionLengths = d;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_expectedInteractionLengths);
    }
    return _expectedInteractionLengths;
}

Float_t TBtaIfrQualR::GetMeasuredInteractionLengths()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_bytes);
	double d;
	d_ULong l= bytes[intLen];
	intlPack.unpack(l,d);
	_measuredInteractionLengths = d;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_measuredInteractionLengths);
    }
    return _measuredInteractionLengths;
}

Float_t TBtaIfrQualR::GetIfrTrkMatch()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_bytes);
	double d;
	d_UShort u= bytes[trkMatchCons];
	Packer.unpackAndScream(0,50,u,d, "trk Match" );
	_IfrTrkMatch = d;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_IfrTrkMatch);
    }
    return _IfrTrkMatch;
}

Float_t TBtaIfrQualR::GetIfrEmcMatch()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_bytes);
	double d;
	d_UShort u= bytes[emcMatchCons];
	Packer.unpackAndScream(0,1.1,u,d, "emc Match" );
	_IfrEmcMatch = d;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_IfrEmcMatch);
    }
    return _IfrEmcMatch;
}

UShort_t TBtaIfrQualR::GetLastBarrel()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	UShort_t laystrips[20];
	GetIfrLayStrips(laystrips);
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_lastBarrel);
    }
    return _lastBarrel;
}

Float_t TBtaIfrQualR::GetIfrClusterFit()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_bytes);
	double chi2;
	d_UShort u = bytes[trkMatchChi2];
	Packer8.unpackAndScream(0,50,u,chi2, "trk Chi2" );
	_IfrTrkMatchChi2 = chi2;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_IfrTrkMatchChi2);
    }
    return _IfrTrkMatchChi2;
}

Float_t TBtaIfrQualR::GetIfrRpcMuonProbability()
{
    if (TBtaMicroCandRDir::GetSchema()>3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_bytes);
	double d;
	d_ULong l= bytes[rpcProb];
	Packer2.unpack( l,d );
	d = (d!=10) ? pow(10,-d) : 0;
	_rpcProb = d;
    }
    else {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::iq_rpcProb);
    }
    return _rpcProb;
}

// Setters:

void TBtaIfrQualR::SetIfrLayStrips(UShort_t laystrips[20])
{
    for (Int_t i= 0; i<20; i++) _IfrLayStrips[i]= laystrips[i];
}
