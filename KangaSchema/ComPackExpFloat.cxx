//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackExpFloat.cxx,v 1.4 2004-01-28 15:41:23 marcel Exp $
//
// Description:
//      class ComPackExpFloat
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Dave Brown               10/12/00
//
// Copyright Information:
//      Copyright (C) 2000 LBNL
//
//
//--------------------------------------------------------------------------

#include "KangaSchema/ComPackExpFloat.h"
#include <math.h>

#include <iostream>
using namespace std;

ComPackExpFloat::ComPackExpFloat(unsigned nbits,
				 unsigned maxexponent,
				 double start,
				 double stop) :
  ComPackBase<double>(start,stop,nbits),
  _maxexp(maxexponent),_expsft(0),_maxnorm( (1<<(maxexponent+1))-1) {
  static double invln2(1.0/log(2.0));
// compute how many bits used by exponent
  _mansft = _maxexp>0?(unsigned)(log((double)_maxexp)*invln2+1.01):0;
  _expmsk = (1<<_mansft)-1;
  _maxman = (1<<(_bitRange-_mansft));
  _manmsk = _maxman-1;
  _invmaxnorm = 1.0/_maxnorm;
  _invmaxman = 1.0/_maxman;
}

ComPackExpFloat::~ComPackExpFloat()
{;}

ComPackBase<double>::StatusCode
ComPackExpFloat::pack (const double value, d_ULong & packdata) const {
  static double invln2(1.0/log(2.0));
  ComPackBase<double>::StatusCode retval(TAG_OK);
// first, figure out the exponent
  double renorm = 1.0+(value-_minVal)*_maxnorm/_valRange;
  if(renorm<1.0){
    renorm=1.0;
    retval = TAG_VAL_ROUND_UP;
  }
  unsigned iexp = unsigned(log(renorm)*invln2);
  unsigned iman(0);
  if(iexp<=_maxexp){
    iman = unsigned( _maxman*(renorm/(1<<iexp) - 1.0) );
// deal with floating-point rounding, which could give a 'non-physical' result
    if(iman==_maxman){
      if(iexp==_maxexp)
	iman=_maxman-1;
      else {
	iman=0;
	iexp++;
      }
    }
  } else {
    iexp=_maxexp;
    iman = _maxman-1;
    retval = TAG_VAL_ROUND_DOWN;
  }
  packdata = (iexp&_expmsk) | (iman&_manmsk)<<_mansft;
  return retval;
}

ComPackBase<double>::StatusCode 
ComPackExpFloat::unpack (const d_ULong packdata, double & value) const {
  unsigned iman = (packdata>>_mansft)&_manmsk;
  unsigned iexp = (packdata>>_expsft)&_expmsk;
  value = _minVal + _valRange*_invmaxnorm*( (1.0+(iman+0.5)*_invmaxman)*(1<<iexp) -1.0);
  return TAG_OK;
}
/*
void
ComPackExpFloat::print(ostream& os) const {
  os << "Exponential packer for range " << _minVal << " to " << _maxVal << endl;
  os << "Maximum exponent = " << _maxexp << endl;
  os << "Maximum mantissa = " << _maxman << endl;
  os << "Mantissa storage shift, mask = " << _mansft << " , " << _manmsk << endl;
}
*/
