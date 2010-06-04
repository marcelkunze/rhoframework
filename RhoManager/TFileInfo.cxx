//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TFileInfo								//
//                                                                      //
// File info class							//
//                                                                      //
// Author: Thorsten Brandt, Dresden University, Aug. 99			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "TString.h"
#include "RhoManager/TFileInfo.h"

ClassImp(TFileInfo)

TFileInfo::TFileInfo(const char *name,Int_t nEvents,Int_t firstRun):
fName(name),fNumberOfEvents(nEvents),fFirstRun(firstRun)
{ }

TFileInfo::TFileInfo(const char *name):
fName(name),fNumberOfEvents(-1),fFirstRun(-1)
{ }

Int_t
TFileInfo::Compare(const TObject *other) const
{
  TFileInfo *o = (TFileInfo*) other;
  return fName.CompareTo(o->fName);
}
