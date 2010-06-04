//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFBitBase								//
//                                                                      //
// Definition of the Tag database (KanGA layout)			//
//                                                                      //
// Author: Marcel Kunze, Bochum University, Nov. 1999			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "PAFSchema/PAFBitBase.h"

ClassImp(PAFBitBase)

#include <iostream>
using namespace std;

//----------------
// Constructors --
//----------------
PAFBitBase::PAFBitBase()
{}

PAFBitBase::PAFBitBase(TTree *t,const unsigned & bufferSize ) : 
VAbsBit(t,bufferSize)
{}


PAFBitBase::~PAFBitBase() 
{}

std::ostream&  operator << (std::ostream& o, const PAFBitBase& a) { a.PrintOn(o); return o; }


