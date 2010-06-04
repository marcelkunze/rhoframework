#ifndef TDATABLOCK_H
#define TDATABLOCK_H
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

#include <iostream>

#include <stdlib.h>
#include "TObject.h"
#include "TString.h"

class TDataBlock : public TObject 
{
  public :
    TDataBlock() { };
    virtual ~TDataBlock() { };
    TDataBlock(const char* filename,Int_t indexFrom, Int_t indexTo);
    Int_t Min() { return fMin; }
    Int_t Max() { return fMax; }
    const char* GetFilename();
    virtual Bool_t IsSortable() const { return kTRUE; }
    virtual Int_t Compare(const TObject *other) const;
    void PrintOn( std::ostream& o=std::cout ) const;

 private:
    Int_t fMin,fMax;
    TString fName;

 public :
    ClassDef(TDataBlock,1) // Event collection
};



#endif
