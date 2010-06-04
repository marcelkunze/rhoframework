//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackExpFloat.h,v 1.3 2002-02-01 03:47:48 marcel Exp $
//
// Description:
//      class ComPackExpFloat; pack a floating (double) into a fixed range
//      mantissa with an exponential (power of 2) representation.  With this class, the values
//      stored at the start of the range have the best precision, which
//      gradually decreases to the worst precision when going to the opposite
//      range.  This is equivalent to flat packing of the log of the value,
//      but because the exponent storage space is variable, this class allows
//      the equivalent of scaling the (log) value.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Dave Brown         10/12/00
//
// Copyright Information:
//      Copyright (C) 2000 LBNL
//
//
//--------------------------------------------------------------------------

#ifndef COMPACKEXPFLOAT_HH
#define COMPACKEXPFLOAT_HH

#include "KangaSchema/ComPackBase.h"
#include <iosfwd>

class ComPackExpFloat:public ComPackBase<double> {

public:
// specify the storage space (number of total bits) and the maximum exponent.
// The mantissa will be stored in the bits left over after storing the exponent
// Note that the exponent is -UNSIGNED-.  Note that the stop and start values do
// _not_ have to be monotonic; the order simply defines which end of the range
// has the fine precision (start) vs coarse (stop).
  ComPackExpFloat(unsigned nbits,
	       unsigned maxexponent,
	       double start,
	       double stop);
  virtual ~ComPackExpFloat();
// ComPackBase functions
  virtual StatusCode pack (const double, d_ULong &) const;
  virtual StatusCode unpack (const d_ULong, double &) const;
  //void print(ostream& os) const;
private:
  unsigned _maxexp; // maximum value of the exponent (+1)
  unsigned _expsft; // exponent storage shift
  unsigned _expmsk; // exponent storage mask
  unsigned _mansft; // mantissa storage shift
  unsigned _manmsk; // mantissa storage mask
  unsigned _maxnorm; // maximum integer value
  unsigned _maxman; // maximum integer mantissa
  double _invmaxnorm;
  double _invmaxman; // inverses, to speed unpacking
};

#endif






