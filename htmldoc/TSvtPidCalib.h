#ifndef TSVTPIDCALIB_H
#define TSVTPIDCALIB_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TSvtPidCalib				    				//
//                                                                      //
// SVT conditions class							//
//                                                                      //
// Author: Marcel Kunze, RUB, Nov. 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "TObject.h"
#include "RhoConditions/TConditions.h"

/** This class is a ROOT persistent version of the transient SvtPidSimpleTMCalib
 * class defined in the SvtPidCalib package, similar to the Objectivity persistent
 * SvtPidSimpleTMCalibP class in the SvtPidCalibP package.
 */
class SvtPidSimpleTMCalibR : public RooCondData {
public:
  /** ROOT I/O requires a default constructor. Create an object based
   * on the defaults specified by the transient default constructor. */
  SvtPidSimpleTMCalibR();
  /** Construct a persistent object corresponding to the specified
   * transient object. */
  //SvtPidSimpleTMCalibR(const SvtPidSimpleTMCalib& transient);
  virtual ~SvtPidSimpleTMCalibR();
  /** Create a transient object on the heap initialized with this persistent
   * object's data. */
  //SvtPidSimpleTMCalib* transient() const;
  Double_t Beta2dedx(Double_t beta) const;
  Double_t Dedx2beta(Double_t dedx) const;
  Double_t Beta2sig1(Double_t beta) const;
  Double_t Beta2sig2(Double_t beta) const;
  virtual void PrintOn(std::ostream& o=std::cout) const;
protected:
  Float_t _aMean;
  Float_t _pMean;
  Float_t _pgb;
  Float_t _cMean;
  Float_t _aSig1;
  Float_t _bSig1;
  Float_t _pSig1;
  Float_t _cSig1;
  Float_t _aSig2;
  Float_t _bSig2;
  Float_t _pSig2;
  Float_t _cSig2;

  ClassDef(SvtPidSimpleTMCalibR,1) // persistent SVT PID calibration
};

// standalone print
std::ostream&  operator <<(std::ostream& o, const SvtPidSimpleTMCalibR&);

// Class TSvtPidCalib

class TSvtPidCalib : public SvtPidSimpleTMCalibR
{
public:
  TSvtPidCalib() {}
  virtual ~TSvtPidCalib() {}
  ClassDef(TSvtPidCalib,1) // transient SVT PID calibration
};

#endif
