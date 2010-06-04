//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TRunInfo								//
//                                                                      //
// Run info class							//
//                                                                      //
// Author: Thorsten Brandt, Dresden University, Aug. 99			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "RhoConditions/TRhoTime.h"
#include "RhoManager/TRunInfo.h"

ClassImp(TRunInfo)

#include <iostream>
using namespace std;

TRunInfo::TRunInfo(Int_t number):
fNumber(number),fFirstRun(0),fLastRun(0),
fStartMajor(0), fStartMinor(0),
fStopMajor(0), fStopMinor(0)
{}

void TRunInfo::PrintOn(std::ostream &o) const
{
  TRhoTime t1(fStartMajor);
  TRhoTime t2(fStopMajor);
  o << "RUN #" << fNumber << " : " << endl; 
  o << "  Start at " << t1 << "         " ;
  o << "  Stop at " << t2 << endl;
}


Int_t
TRunInfo::Compare(const TObject *other) const
{
  TRunInfo *otherR = (TRunInfo*) other;
  if (fNumber < otherR->GetNumber()) {
    return -1;
  } else  if (fNumber > otherR->GetNumber()) {
    return 1;
  } else {
    return 0;
  }
}

std::ostream&  operator << (std::ostream& o, const TRunInfo& a) { a.PrintOn(o); return o; }


