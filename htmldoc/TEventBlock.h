#ifndef TEVENTBLOCK_H
#define TEVENTBLOCK_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TEventBlock								//
//                                                                      //
// Eventnumber reference class						//
//                                                                      //
// Author: Thorsten Brandt, Dresden University, Aug. 99			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "TObjString.h"


class TEventBlock : public TObject 
{
public :
    
    TEventBlock() { ClearBuffer(); };
    virtual ~TEventBlock() { };
    
    void ClearBuffer();
    void SetFilename(const char* s);
    void AddNumber(Int_t n, UInt_t minorId, UInt_t majorId);
    Int_t* GetBuffer() { return  fEventNumber; }
    Int_t GetBufLen() { return fLength; }
    const char* GetFilename();
    
private :
    
    char fFileName[255];
    Int_t fEventNumber[1000];
    UInt_t fMinorID[1000];
    UInt_t fMajorID[1000];
    Int_t fLength;
    
public :
    
    ClassDef(TEventBlock,1) // Event collection
	
};

#endif
