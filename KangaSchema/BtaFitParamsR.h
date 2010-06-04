//  	-*- Mode: c++ -*-
//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaFitParamsR.h,v 1.3 2002-02-01 03:47:41 marcel Exp $
//
// Description:
//	Class BtaFitParamsR
//
//      Persistent partner of BtaFitParams; differs from the transient class
//        in a way the data members are packed
// 
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Stefan Kluth
//
// Copyright Information:
//	Copyright (C) 1999                LBNL
//
//------------------------------------------------------------------------
#ifndef BTAFITPARAMSR_HH
#define BTAFITPARAMSR_HH

#include <iosfwd>

//----------------------
// Base Class Headers --
//----------------------

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Rtypes.h"

class TFitParams;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class BtaFitParamsR {
  //--------------------
  // Instance Members --
  //--------------------
public:

  BtaFitParamsR();
  //BtaFitParamsR( const TFitParams & );

  virtual ~BtaFitParamsR();

  TFitParams* Transient() const;

  //virtual void PrintOn( ostream & ) const;

private:

  // the params
  Short_t _q;
  Float_t _pos[3];
  Float_t _p4[4];
  
  // the 7X7 covariance
  Float_t _cov7[28];

  // disallow
  //BtaFitParamsR( const BtaFitParamsR & );
  //BtaFitParamsR & operator= ( const BtaFitParamsR & );

  ClassDef(BtaFitParamsR,1)

};

#endif




