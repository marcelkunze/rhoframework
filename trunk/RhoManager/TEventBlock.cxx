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

#include <string.h>

#include "RhoManager/TEventBlock.h"

ClassImp(TEventBlock)

#include <iostream>
using namespace std;

void TEventBlock::ClearBuffer()
{
    for (Int_t i=0; i<1000; i++) {
	fEventNumber[i]=0;
	fMajorID[i]=0;
	fMinorID[i]=0;
    }
    fLength=0;
}


void TEventBlock::AddNumber(Int_t n, UInt_t minorId, UInt_t majorId)
{ 
    fEventNumber[fLength]=n; 
    fMajorID[fLength] = majorId;
    fMinorID[fLength] = minorId;
    fLength++; 
}


void
TEventBlock::SetFilename(const char* s)
{
    if (TString(s).Length()>255) {
	cerr << "TEventBlock: can not handle names longer than 255 chars " << s << endl;
    }
    strncpy(fFileName,s,255); 
}

const char*
TEventBlock::GetFilename()
{
    return fFileName;
}
