#ifndef TDCHCOND_H
#define TDCHCOND_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TDchCond			    					//
//                                                                      //
// DCH conditions class							//
//                                                                      //
// Author: Marcel Kunze, RUB, Nov. 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "TObject.h"
#include "RhoConditions/TConditions.h"

#define nParMax 20

class DchBetheBlochR : public RooCondData {
public:
  DchBetheBlochR();
  virtual ~DchBetheBlochR();
  void PrintOn(std::ostream& o=std::cout) const;
protected:
  Short_t  _nPar;
  Double_t _par[nParMax];
  TString  _functionName;
  ClassDef(DchBetheBlochR,1) // persistent Bethe&Bloch function
};

// standalone print
std::ostream&  operator <<(std::ostream& o, const DchBetheBlochR&);

// Class TDchCond

class TDchCond : public DchBetheBlochR
{
public:
  TDchCond() {}
  virtual ~TDchCond() {}

  Double_t Ionization(Double_t momentum, Double_t mass) const;
  Double_t* GetParameters() { return _par; }
  const char* GetFunctionName() { return _functionName.Data(); }
  ClassDef(TDchCond,1) // transient Bethe&Bloch function
};

#endif
