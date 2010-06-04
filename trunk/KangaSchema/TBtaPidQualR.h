//--------------------------------------------------------------------------
//
// Description:
//	Class TBtaPidQualR
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//     Yury Kolomensky
//     Leif Wilden (adapted as ROOT standalone version)
//
// Copyright Information:
//	Copyright (C) 1999                Caltech, TU Dresden
//
//------------------------------------------------------------------------
#ifndef TBtaPidQualR_HH
#define TBtaPidQualR_HH

#include "TObject.h"
#include "KangaSchema/ComPackUFloat.h"
#include "KangaSchema/ComPackFlatFloat.h"
#include "KangaSchema/ComPackNonFlatFloat.h"

class TBtaPidQualR {

  enum ByteVars  { nPhot, nBkgd, nExPhot0, nExPhot1, nExPhot2, nExPhot3, nExPhot4, 
		    deltaDchMom, deltaDrcMom, nByteVars };
  enum ShortVars { thetaDirc, statusDRC, eThetaDirc, dEdXDch, dEdXSvt, trkEmcTheta, trkEmcPhi, 
                    nShortVars };

public:

  // Constructors:
  TBtaPidQualR();

  // Destructor:
  virtual ~TBtaPidQualR();

  virtual void Reset();

  // Getters:

  Float_t  GetThetaDirc();
  Float_t  GetEThetaDirc();
  Float_t  GetDEdXDch();
  Float_t  GetDEdXSvt();
  UShort_t GetNSamplesDch();
  UShort_t GetNSamplesSvt();
  Int_t    GetDrcInBar();
  Int_t    GetDrcExitBar();
  Float_t  GetDrcXPos();
  Int_t    GetNPhot();
  Int_t    GetNBkgd();
  Short_t  GetNExPhot(UShort_t PidHypo);
  Float_t  GetDeltaDchMom();
  Float_t  GetDeltaDrcMom();
  Float_t  GetDrcPhiAtEmc();
  Float_t  GetDrcThetaAtEmc();

  // Setters:

  void     SetThetaDirc(Float_t td) { _thetaDirc= td; }
  void     SetEThetaDirc(Float_t etd) { _eThetaDirc= etd; }
  void     SetDEdXDch(Float_t dedx) { _dEdXDch= dedx; }
  void     SetDEdXSvt(Float_t dedx) { _dEdXSvt= dedx; }
  void     SetNSamplesDch(UShort_t ndch) { _nSamplesDch= ndch; }
  void     SetNSamplesSvt(UShort_t nsvt) { _nSamplesSvt= nsvt; }
  void     SetDrcNumBar(Int_t dnb) { _drcNumBar= dnb; }
  void     SetDrcExitBar(Int_t dxit) { _drcExit= dxit; }
  void     SetDrcXPos(Float_t dxp) { _drcXPos= dxp; }
  void     SetNPhot(Int_t np) { _nPhot= np; }
  void     SetNBkgd(Int_t nb) { _nBkgd= nb; }
  void     SetNExPhot(UShort_t PidHypo, Short_t xp) { _nExPhot[PidHypo]= xp; }
  void     SetDeltaDchMom(Float_t ddchmom) { _deltaDchMom= ddchmom; }
  void     SetDeltaDrcMom(Float_t ddrcmom) { _deltaDrcMom= ddrcmom; }
  void     SetDrcPhiAtEmc(Float_t x) { _trkEmcPhi= x; }
  void     SetDrcThetaAtEmc(Float_t x) { _trkEmcTheta= x; }

private:

  // Limits
  enum {
    NHITLEN=6,
    FRACLEN=6,
    EXPLEN=4,
    BIAS=-5
  };

  Float_t  _thetaDirc;
  Short_t  _drcNumBar;
  Short_t  _drcExit;
  Float_t  _drcXPos;
  Short_t  _nPhot;
  Short_t  _nBkgd;
  Short_t  _nExPhot[5];
  Float_t  _eThetaDirc;
  Float_t  _dEdXDch;
  Float_t  _dEdXSvt;
  Float_t  _deltaDchMom;  // delta of momentum in the middle of the  Dch wrt the vertex
  Float_t  _deltaDrcMom;  // delta of momentum in the middle of the  Dch wrt the vertex
  Short_t  _nSamplesDch;
  Short_t  _nSamplesSvt;
  
  // new beginning with schema 8.6.x:
  Float_t  _trkEmcTheta;  // theta of the track at EMC level
  Float_t  _trkEmcPhi;    // phi of the track at EMC level

  Char_t   bytes[nByteVars];
  UShort_t shorts[nShortVars];

  static ComPackNonFlatFloat Packer;
  static ComPackNonFlatFloat Packer8;
  static ComPackFlatFloat anglePacker;
  static ComPackFlatFloat packX;
  static ComPackFlatFloat thetaPack;
  static ComPackFlatFloat phiPack;
  static ComPackUFloat packdEdX;

  // disallowed
  TBtaPidQualR( const TBtaPidQualR & );
  TBtaPidQualR & operator = ( const TBtaPidQualR & );

  friend class TBtaMicroCandR;

  ClassDef(TBtaPidQualR,2) // PID measurement (Rho Kanga implementation)

};

#endif





