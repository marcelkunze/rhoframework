//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFMicroCandidate							//
//                                                                      //
// Implementation of the PAF Micro candidate	    			//
//                                                                      //
// Author: Marcel Kunze, RUB, Mar 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "PAFSchema/PAFMicroCandidate.h"

ClassImp(PAFMicroCandidate)

#include <iostream>
using namespace std;

PAFMicroCandidate::PAFMicroCandidate(PAFAodBase *a,PAFAodCluster *b,PAFAodTrack *c) :
fAodBase(a),fAodCluster(b),fAodTrack(c)
{}

std::ostream&  operator << (std::ostream& o, const PAFMicroCandidate& a) { a.PrintOn(o); return o; }
