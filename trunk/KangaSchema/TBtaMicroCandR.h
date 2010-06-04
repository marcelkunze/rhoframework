//--------------------------------------------------------------------------
//
// Description:
//      Class BtaMicroCandR - The persistent version of the BtaMicroCand class.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Stefan Kluth           Originator
//      Leif Wilden (adapted as ROOT standalone version)
//
// Copyright Information:
//	Copyright (C) 1999     LBNL, TU Dresden
//
//------------------------------------------------------------------------
#ifndef TBTAMICROCANDR_HH
#define TBTAMICROCANDR_HH

#include "TObject.h"

#include "KangaSchema/TBtaFitParamsR.h"
#include "KangaSchema/TBtaTrkQualR.h"
#include "KangaSchema/TBtaCalQualR.h"
#include "KangaSchema/TBtaPidQualR.h"
#include "KangaSchema/TBtaPidInfoR.h"
#include "KangaSchema/TBtaIfrQualR.h"

class TBtaMicroCandR : public TObject {

public:

  // Constructors
  TBtaMicroCandR();

  // Destructor
  virtual ~TBtaMicroCandR();

  // Getters:

  UChar_t         GetSubObjStatus() const;
  TBtaFitParamsR* GetFitParams();
  TBtaTrkQualR*   GetTrkQual();
  TBtaCalQualR*   GetCalQual();
  TBtaIfrQualR*   GetIfrQual();
  TBtaPidQualR*   GetPidQual();
  TBtaPidInfoR*   GetPidInfo();

  void            SetSubObjStatus(UChar_t status);
    
  virtual void    Reset();
    
private:

  // Data members:
  TBtaFitParamsR _fitParams; // Fit parameters
  UChar_t _subObjStatus;     // Status flags for contained objects
  TBtaTrkQualR _trkQual;     // Track quality
  TBtaCalQualR _calQual;     // Calorimeter quality 
  TBtaPidQualR _pidQual;     // PID quality
  TBtaPidInfoR _pidInfo;     // PID info
  TBtaIfrQualR _ifrQual;     // Ifr quality

  // disallowed:
  TBtaMicroCandR( const TBtaMicroCandR & );
  const TBtaMicroCandR & operator = ( const TBtaMicroCandR & );
  
  ClassDef(TBtaMicroCandR,1) // Beta MicroCandidate (Rho Kanga implementation)

};

#endif
