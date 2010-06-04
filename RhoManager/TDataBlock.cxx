//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TDataBlock								//
//                                                                      //
// Define a block of runs for use in a collection			//
//                                                                      //
// Author: TB, TUD, May. 99						//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "RhoManager/TDataBlock.h"

ClassImp(TDataBlock);

#include <iostream>
using namespace std;

TDataBlock::TDataBlock(const char* filename,Int_t indexFrom, Int_t indexTo) :
  fMin(indexFrom),
  fMax(indexTo),
  fName(filename)
{ }

void
TDataBlock::PrintOn(std::ostream &o) const
{
   o << " " << fName.Data() << " : Entries " << fMin << " - " << fMax << endl;
}

Int_t
TDataBlock::Compare(const TObject *other) const
{
  TDataBlock *otherR = (TDataBlock*) other;
  Int_t n = fName.CompareTo(otherR->fName);
  if (n==0) {
    if (fMin < otherR->fMin) {
      return -1;
    } else if  (fMin > otherR->fMin) {
      return 1;
    }
  }
  return n;
}

const char* TDataBlock::GetFilename() 
{ 
  //return TRho::Instance()->GetEventStore() + "/" + fName; 
  // TB 3/11/99 complete filename is now made in Stream
  return fName.Data();
}
