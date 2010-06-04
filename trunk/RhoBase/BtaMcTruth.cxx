//////////////////////////////////////////////////////////////////////////
//                                                                      //
// BtaMcTruth								//
//                                                                      //
// Monte Carlo truth class						//
//									//
// Author List:								//
// Marcel Kunze, Bochum University, Aug. 99				//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "PDT/Pdt.hh"
#include "RhoBase/TRho.h"
#include "RhoBase/BtaMcTruth.hh"

ClassImp(BtaMcTruth)

using namespace std;

BtaMcTruth::BtaMcTruth()
{
}

BtaMcTruth::~BtaMcTruth() 
{
}

std::ostream&  operator << (std::ostream& o, const BtaMcTruth& a) { a.printOn(o); return o; }
