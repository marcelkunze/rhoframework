#ifndef TFILEINFO_H
#define TFILEINFO_H
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

#include "TObject.h"
#include "TString.h"

class TFileInfo : public TObject
{
  public :
    TFileInfo() { };
    TFileInfo(const char *name,Int_t nEvents,Int_t firstRun);
    TFileInfo(const char *name);
    virtual ~TFileInfo() { };
    const char* GetFileName() { return fName; }
    Int_t GetNevents()  { return fNumberOfEvents; }
    Int_t GetFirstRun() { return fFirstRun; }
    virtual Int_t Compare(const TObject *other) const;
    virtual Bool_t IsSortable() const { return kTRUE; }
 private:
    TString fName;
    Int_t fNumberOfEvents;
    Int_t fFirstRun;
 public:
    ClassDef(TFileInfo,1) // File info
};

#endif
