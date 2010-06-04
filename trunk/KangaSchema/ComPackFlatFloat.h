//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackFlatFloat.h,v 1.1.1.1 2001-05-15 07:01:43 marcel Exp $
//
// Description:
//      Class ComPackFlatFloat
//      - Maps a float onto an integer range - useful if you know that
//      a value will always be in a certain range.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      D.E.Azzopardi               Originator. 
//
// Copyright Information:
//      Copyright (C) 1999  DESVA Research & Design, for Hedgehog Concepts
//
// Quote:
//      "Do, or do not, there is no try."
//                         - Jedi Master Yoda
//
//--------------------------------------------------------------------------

#ifndef COMPACKFLATFLOAT_HH
#define COMPACKFLATFLOAT_HH

//
// Bass class includes
//
#include "KangaSchema/ComPackBase.h"

class ComPackFlatFloat:public ComPackBase<double> {

public:
  ///////////////////
  // Constructors: //
  ///////////////////
  ComPackFlatFloat(const double, const double, size_t); // linearly packed float

  ///////////////////
  // Destructor:   //
  ///////////////////
  virtual ~ComPackFlatFloat();

  ///////////////////
  // Packers:      //
  ///////////////////
  StatusCode pack (const double, d_ULong &) const;

  ///////////////////
  // Unpackers:    //
  ///////////////////
  StatusCode unpack (const d_ULong, double &) const;

  ///////////////////
  // Accessors:    //
  ///////////////////

  ///////////////////
  // Diagnostic:   //
  ///////////////////
  void testMe ( size_t, double & );

private:
  double _pacfac;
  double _upacfac; // precomputed values to speed packing/unpacking
};

#endif
