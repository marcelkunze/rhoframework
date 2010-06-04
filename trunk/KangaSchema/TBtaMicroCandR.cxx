//------------------------------------------------------------------------
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Stefan Kluth           Originator
//      Leif Wilden
//
// Copyright Information:
//	Copyright (C) 1999     LBNL, TU Dresden
//
//------------------------------------------------------------------------

#include "KangaSchema/TBtaMicroCandR.h"

ClassImp(TBtaMicroCandR)

//------------------------------------------------------------------------

TBtaMicroCandR::TBtaMicroCandR() :
  _fitParams(),
  _subObjStatus( 0 ), 
  _trkQual(), 
  _calQual(), 
  _pidQual(), 
  _pidInfo(),
  _ifrQual()
{}

TBtaMicroCandR::~TBtaMicroCandR() {}

UChar_t TBtaMicroCandR::GetSubObjStatus() const
{ return _subObjStatus; }

TBtaFitParamsR* TBtaMicroCandR::GetFitParams()
{ return &_fitParams; }

TBtaTrkQualR* TBtaMicroCandR::GetTrkQual()
{ return &_trkQual; }

TBtaCalQualR* TBtaMicroCandR::GetCalQual()
{ return &_calQual; }

TBtaIfrQualR* TBtaMicroCandR::GetIfrQual()
{ return &_ifrQual; }

TBtaPidQualR* TBtaMicroCandR::GetPidQual()
{ return &_pidQual; }

TBtaPidInfoR* TBtaMicroCandR::GetPidInfo()
{ return &_pidInfo; }

void TBtaMicroCandR::SetSubObjStatus(UChar_t status)
{ _subObjStatus= status; }
    
void TBtaMicroCandR::Reset()
{
  _subObjStatus= 0;
  _fitParams.Reset();
  _trkQual.Reset();
  _calQual.Reset();
  _ifrQual.Reset();
  _pidQual.Reset();
  _pidInfo.Reset();
}
