//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackNonFlatFloat.h,v 1.4 2001-12-17 17:45:16 Marcel Exp $
//
// Description:
//      ComPackNonFlatFloat interface
//      - See class for more information
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      R.Faccini               Originator. 
//
// Copyright Information:
//      Copyright (C) 1999      U.C.S.D.
//
// Quote:
//      "It ain't the way that you do it, it's what you do"
//
//--------------------------------------------------------------------------

#ifndef ComPackNonFlatFloat_HH
#define ComPackNonFlatFloat_HH
#include "KangaSchema/ComPackBase.h"

//
// Bass class includes
//
class TString;

class ComPackNonFlatFloat {

public:
  // unsigned?
  enum signType{SIG,UNSIG,SIG_8,UNSIG_8};

  ///////////////////
  // Constructors: //
  ///////////////////
  ComPackNonFlatFloat(signType sigT=SIG);

  ///////////////////
  // Destructor:   //
  ///////////////////
  ~ComPackNonFlatFloat();

  ///////////////////
  // Packers:      //
  ///////////////////
  ComPackBase<double>::StatusCode pack (double xmin, double xmax, double, d_UShort &) const;
  ComPackBase<double>::StatusCode pack (double xmin, double xmax, double, d_Octet &) const;

  Bool_t packAndScream(double xmin, double xmax ,  double x, d_UShort&y, const char* name) ;
  Bool_t packAndScream(double xmin, double xmax ,  double x, d_Octet&y, const char* name) ;

  ///////////////////
  // Unpackers:    //
  ///////////////////
  ComPackBase<double>::StatusCode unpack (double xmin,double xmax , d_UShort , double&) const;

  Bool_t unpackAndScream(double xmin, double xmax, const  d_UShort&x, double &y, const char* name) const ;
  double getMinVal(){return _engine->getMinVal();}
  double getMaxVal(){return _engine->getMaxVal();}

  ///////////////////
  // Accessors:    //
  ///////////////////


private:
  // Limits
  enum {
    FRACLEN_8=5,
    EXPLEN_8=3,
    BIAS_8=-2,

    FRACLEN_16=10,
    EXPLEN_16=6,
    BIAS_16=-30
  };
  ComPackBase<double>*_engine;
};

#endif



